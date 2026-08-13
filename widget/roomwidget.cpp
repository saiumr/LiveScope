#include "roomwidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QNetworkRequest>
#include <QNetworkReply>

RoomWidget::RoomWidget(const QString& roomId, QWidget *parent)
    : QWidget{parent},
    m_roomId{roomId}
{
    setupUi();
    setupApi();
    setupConnections();

    // api get room info
    m_liveApi->getLiveRoomInfo(m_roomId);
    m_timer->start(kRequestInterval);
}

void RoomWidget::onUserInfoReceived(const BilibiliUser &user)
{
    m_streamerLabel->setText(QString(R"(
        <a style="color:#FFFFFF; text-decoration:none;"
        href="https://space.bilibili.com/%1"> %2</a>)").arg(user.uid, user.name));
    m_streamerLabel->setOpenExternalLinks(true);
    emit roomTitleChanged(user.name);

    QNetworkReply* reply { m_imageManager->get(QNetworkRequest(QUrl(user.face_url))) };
    connect(reply, &QNetworkReply::finished, this, [reply, this]() {
        QByteArray image_raw_data { reply->readAll() };
        reply->deleteLater();

        QPixmap pixmap;
        if (pixmap.loadFromData(image_raw_data)) {
            m_avatarLabel->setPixmap(pixmap);
        }
    });

}

void RoomWidget::requestDanmaku()
{
    m_danmakuApi->getRoomDanmaku(m_roomId);
}

void RoomWidget::setupUi()
{
    m_danmakuList = new QListWidget(this);
    m_streamerLabel = new QLabel("主播: Unknown", this);
    m_titleLabel = new QLabel("[标题: Unknown] ", this);
    m_descriptionLabel = new QLabel("简介: Unknown", this);
    m_statusIndicator = new QLabel(this);
    m_statusLabel = new QLabel("[直播状态: Unknown]", this);
    m_avatarLabel = new QLabel(this);
    m_imageManager = new QNetworkAccessManager(this);

    //
    // ======== ui design ========
    //
    auto mainLayout { new QVBoxLayout(this) };
    auto topLayout { new QVBoxLayout() };
    auto firstLine { new QHBoxLayout() };

    // ui layout design
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(m_danmakuList);

    // live room top info ui
    m_streamerLabel->setStyleSheet(
        "font-size:16px;font-weight:bold;"
        );
    m_streamerLabel->setCursor(Qt::PointingHandCursor);
    m_avatarLabel->setScaledContents(true);
    m_avatarLabel->setFixedSize(48, 48);
    m_streamerLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    m_statusLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    firstLine->addWidget(m_avatarLabel);
    firstLine->addWidget(m_streamerLabel);
    m_statusIndicator->setFixedSize(10, 10);
    firstLine->addWidget(m_statusIndicator);
    firstLine->addWidget(m_statusLabel);
    firstLine->setSpacing(10);
    firstLine->addStretch();
    m_descriptionLabel->setWordWrap(true);
    topLayout->addLayout(firstLine);
    m_titleLabel->setStyleSheet(
        "font-size:12px; font-weight:bold;"
        );
    topLayout->addWidget(m_titleLabel);
    topLayout->addWidget(m_descriptionLabel);

    // danmaku list ui
    m_danmakuList->setWordWrap(true);
}

void RoomWidget::setupApi()
{
    m_danmakuApi = new BilibiliDanmakuApi(this);
    m_liveApi = new BilibiliLiveApi(this);
    m_userApi = new BilibiliUserApi(this);
    m_timer = new QTimer(this);
}

void RoomWidget::setupConnections()
{
    connect(m_timer, &QTimer::timeout, this, &RoomWidget::requestDanmaku);
    connect(m_danmakuApi, &BilibiliDanmakuApi::danmakuReceived, this, &RoomWidget::onDanmakuReceived);
    connect(m_liveApi, &BilibiliLiveApi::liveRoomReceived, this, &RoomWidget::onLiveRoomReceived);
    connect(m_userApi, &BilibiliUserApi::userInfoReceived, this, &RoomWidget::onUserInfoReceived);
}

void RoomWidget::onDanmakuReceived(const QList<Danmaku> &danmakus)
{
    for (const Danmaku& danmaku: danmakus) {
        if(m_receivedIds.size() > 10000) {  // about 320 KB
            m_receivedIds.clear();
            m_danmakuList->clear();         // todo: amount limited, but need to store into sql
        }
        if (m_livingStartTime > danmaku.time || m_receivedIds.contains(danmaku.id)) {
            continue;
        }
        // qDebug() << "[ " << m_livingStartTime.toString("yyyy-MM-dd hh:mm:ss") << ", " << danmaku.time.toString("yyyy-MM-dd hh:mm:ss") << " ]";
        m_receivedIds.insert(danmaku.id);  // danmaku id_str

        QString item {
            QString("[%1] %2: %3")
                .arg(danmaku.time.toString("hh:mm:ss"),
                    danmaku.nickname,
                    danmaku.text)
        };

        m_danmakuList->addItem(item);
        m_danmakuList->scrollToBottom();
    }
}

void RoomWidget::onLiveRoomReceived(const LiveRoom &room)
{
    m_livingStartTime = room.liveTime;
    m_titleLabel->setText(QString(R"(
        <a style="color:#FFFFFF; text-decoration:none;"
        href="https://live.bilibili.com/%1"> [%2] </a>)").arg(room.roomId, room.title));
    m_titleLabel->setOpenExternalLinks(true);
    m_descriptionLabel->setText(QString("%1").arg(room.description));

    switch (room.status) {
    case LiveRoom::Status::Offline:
        m_statusIndicator->setStyleSheet(
            "background-color: lightgray;"
            "border-radius: 5px;"
            );
        m_statusLabel->setStyleSheet(
            "color: lightgray;"
            );
        m_statusLabel->setText("[未开播]");
        break;
    case LiveRoom::Status::Live:
        m_statusIndicator->setStyleSheet(
            "background-color: lightgreen;"
            "border-radius: 5px;"
            );
        m_statusLabel->setStyleSheet(
            "color: lightyellow;"
            );
        m_statusLabel->setText("[直播中]");
        break;
    case LiveRoom::Status::RoundPlay:
        m_statusIndicator->setStyleSheet(
            "background-color: lightyellow;"
            "border-radius: 5px;"
            );
        m_statusLabel->setStyleSheet(
            "color: lightyellow;"
            );
        m_statusLabel->setText("[轮播中]");
        break;
    default:
        break;
    }

    m_userApi->getUserInfo(room.uid);
    // qDebug() << room.uid;
}
