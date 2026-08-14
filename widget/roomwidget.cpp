#include "roomwidget.h"
#include "widget/danmakuitemwidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDesktopServices>
#include <QMouseEvent>
#include <QScrollBar>

RoomWidget::RoomWidget(const QString& roomId, QWidget *parent)
    : QWidget{parent},
    m_roomId{roomId}
{
    setupUi();
    setupApi();
    setupConnections();

    // api get room info
    m_liveApi->getLiveRoomInfo(m_roomId);
    m_timer->start(kRequestIntervalRT);
}

bool RoomWidget::eventFilter(QObject *watched, QEvent *event)
{
    auto me { static_cast<QMouseEvent*>(event) };
    if (watched == m_titleLabel) {
        if (event->type() == QEvent::Enter) {
            m_descriptionLabel->setVisible(true);
        } else if (event->type() == QEvent::Leave) {
            m_descriptionLabel->setVisible(false);
        }
        if (event->type() == QEvent::MouseButtonRelease && me->button() == Qt::LeftButton) {
            QDesktopServices::openUrl(QUrl(QStringLiteral("https://live.bilibili.com/%1").arg(m_roomId)));
        }
    } else if (watched == m_streamerLabel) {
        if (event->type() == QEvent::MouseButtonRelease && me->button() == Qt::LeftButton) {
            QDesktopServices::openUrl(QUrl(QStringLiteral("https://space.bilibili.com/%1").arg(m_uid)));
        }
    }

    return QWidget::eventFilter(watched, event);
}

void RoomWidget::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::PaletteChange) {
        updateLabelStyle();
    }
    QWidget::changeEvent(event);
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
    auto topLayout  { new QVBoxLayout() };
    auto firstLine  { new QHBoxLayout() };  // status
    auto secondLine { new QHBoxLayout() };  // title

    // ui layout design
    topLayout->addLayout(firstLine);
    topLayout->addLayout(secondLine);
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(m_danmakuList);

    // live room top info ui
    m_streamerLabel->setCursor(Qt::PointingHandCursor);
    // m_streamerLabel->setForegroundRole(QPalette::WindowText);
    m_streamerLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    m_avatarLabel->setScaledContents(true);
    m_avatarLabel->setFixedSize(48, 48);
    m_statusLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    firstLine->addWidget(m_avatarLabel);
    firstLine->addWidget(m_streamerLabel);
    m_statusIndicator->setFixedSize(10, 10);
    firstLine->addWidget(m_statusIndicator);
    firstLine->addWidget(m_statusLabel);
    firstLine->setSpacing(10);
    firstLine->addStretch();

    m_titleLabel->setCursor(Qt::PointingHandCursor);
    // m_titleLabel->setForegroundRole(QPalette::WindowText);
    secondLine->addWidget(m_titleLabel);
    secondLine->addStretch();
    m_descriptionLabel->setWordWrap(true);
    topLayout->addWidget(m_descriptionLabel);

    // danmaku list ui
    m_danmakuList->setResizeMode(QListView::Adjust);
    m_danmakuList->setWordWrap(true);

    // style
    updateLabelStyle();

    // ui event
    m_descriptionLabel->setVisible(false);
    m_titleLabel->installEventFilter(this);
    m_streamerLabel->installEventFilter(this);
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

void RoomWidget::updateLabelStyle()
{
    QPalette pal         { this->palette() };
    QColor   normalColor { pal.windowText().color() };
    QColor   hoverColor  { getBeautifulHoverColor(normalColor) };
    // qDebug() << normalColor.name() << "     " << hoverColor.name();
    m_streamerLabel->setStyleSheet(
        QString(R"(
        QLabel {
            font-size:16px;
            font-weight:bold;
            color:%1;
        }
        QLabel:hover {
            color:%2;
        }
    )").arg(normalColor.name(), hoverColor.name()));

    m_titleLabel->setStyleSheet(
        QString(R"(
        QLabel {
            font-size:12px;
            font-weight:bold;
            color:%1;
        }
        QLabel:hover {
            color:%2;
        }
    )").arg(normalColor.name(), hoverColor.name()));
}

// getBeautifulHoverColor is from deepseek
QColor RoomWidget::getBeautifulHoverColor(const QColor &normal)
{
    int r { normal.red() };
    int g { normal.green() };
    int b { normal.blue() };
    int delta { 35 }; // 控制变化强度，建议范围 25~50

    if (normal.lightness() > 128) {
        // ========== 浅色主题（白底黑字）：悬停时变暗，并微微发蓝 ==========
        // 减掉亮度，但少减一点蓝色，相当于多保留/增加蓝色分量
        r = qMax(0, r - delta);
        g = qMax(0, g - delta);
        b = qMax(0, b - delta + 15); // 关键：让蓝色掉得少一点，产生冷色调
    } else {
        // ========== 深色主题（黑底白字）：悬停时变亮，并微微发蓝/紫 ==========
        // 增加亮度，但额外给蓝色多加一点偏移
        r = qMin(255, r + delta);
        g = qMin(255, g + delta);
        b = qMin(255, b + delta + 20); // 关键：让蓝色长得更快，产生现代科技感
    }
    return QColor(r, g, b);
}

bool RoomWidget::isAtBottom() const
{
    auto* scrollBar { m_danmakuList->verticalScrollBar() };
    return scrollBar->value() == scrollBar->maximum();
}

void RoomWidget::onDanmakuReceived(const QList<Danmaku> &danmakus)
{
    const bool shouldScrollToBottom { isAtBottom() };
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

        // QString item {
        //     QString("[%1] %2: %3")
        //         .arg(danmaku.time.toString("hh:mm:ss"),
        //             danmaku.nickname,
        //             danmaku.text)
        // };

        auto item   { new QListWidgetItem(m_danmakuList) };
        auto widget { new DanmakuItemWidget(danmaku) };

        m_danmakuList->setItemWidget(item, widget);
        item->setSizeHint(widget->sizeHint());       // absolutly need to set it!
        if (shouldScrollToBottom) {
            m_danmakuList->scrollToBottom();
        }
    }
}

void RoomWidget::onUserInfoReceived(const BilibiliUser &user)
{
    m_uid = user.uid;
    m_name = user.name;

    m_streamerLabel->setText(QString(R"( %1 )").arg(user.name));

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

void RoomWidget::onLiveRoomReceived(const LiveRoom &room)
{
    m_livingStartTime = room.liveTime;
    m_title = room.title;

    // init m_titleLable and m_descriptionLabel
    m_titleLabel->setText(QString(R"([%1] )").arg(room.title));
    m_descriptionLabel->setText(QString("%1").arg(room.description));

    switch (room.status) {
    case LiveRoom::Status::Offline:
        m_statusIndicator->setStyleSheet(
            "background-color: #7A7A7A;"
            "border-radius: 5px;"
            );
        m_statusLabel->setStyleSheet(
            "color: #7A7A7A;"
            );
        m_statusLabel->setText("[未开播]");
        break;
    case LiveRoom::Status::Live:
        m_statusIndicator->setStyleSheet(
            "background-color: #5B8C6F;"
            "border-radius: 5px;"
            );
        m_statusLabel->setStyleSheet(
            "color: #5B8C6F;"
            );
        m_statusLabel->setText("[直播中]");
        break;
    case LiveRoom::Status::RoundPlay:
        m_statusIndicator->setStyleSheet(
            "background-color: #C29A3B;"
            "border-radius: 5px;"
            );
        m_statusLabel->setStyleSheet(
            "color: #C29A3B;"
            );
        m_statusLabel->setText("[轮播中]");
        break;
    default:
        break;
    }

    m_userApi->getUserInfo(room.uid);
    // qDebug() << room.uid;
}
