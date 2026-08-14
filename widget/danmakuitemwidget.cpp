#include "danmakuitemwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QDesktopServices>

DanmakuItemWidget::DanmakuItemWidget(const Danmaku& danmaku, QWidget *parent)
    : QWidget{parent},
    m_uid{danmaku.uid},
    m_levelLabel{new QLabel("Lv" + QString::number(danmaku.userLevel))},
    m_adminLabel{new QLabel("Admin")},
    m_scLabel{new QLabel("SuperChat!")},
    m_nameLabel{new QLabel(danmaku.nickname)},
    m_timeLabel{new QLabel(danmaku.time.toString("hh:mm:ss"))},
    m_textLabel{new QLabel(danmaku.text)}
{
    m_levelLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    m_nameLabel->installEventFilter(this);
    m_textLabel->setWordWrap(true);
    m_textLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    if (danmaku.userLevel == 0) {
        m_levelLabel->setVisible(false);
    }
    if (!danmaku.isAdmin) {
        m_adminLabel->setVisible(false);
    }
    if (!danmaku.isSc) {
        m_scLabel->setVisible(false);
    }

    QVBoxLayout* vLayout       { new QVBoxLayout(this) };
    QHBoxLayout* hLineLayout   { new QHBoxLayout() };
    vLayout->addLayout(hLineLayout);
    vLayout->addWidget(m_textLabel);

    hLineLayout->addWidget(m_levelLabel);
    hLineLayout->addWidget(m_adminLabel);
    hLineLayout->addWidget(m_scLabel);
    hLineLayout->addWidget(m_nameLabel);
    hLineLayout->addStretch();
    hLineLayout->addWidget(m_timeLabel);

    m_nameLabel->setCursor(Qt::PointingHandCursor);
    m_levelLabel->setStyleSheet(
    R"(
        QLabel {
            background-color: #5686fe;
            color: #ffffff;
            font-weight:bold;
            padding: 1px 1px 1px 1px;
            border-radius: 8px;
        }
    )");
    m_adminLabel->setStyleSheet(
        R"(
        QLabel {
            background-color: #009cfe;
            color: #ffffff;
            font-weight:bold;
            padding: 1px 1px 1px 1px;
            border-radius: 4px;
        }
    )");
    m_scLabel->setStyleSheet(
        R"(
        QLabel {
            background-color: #ff4444;
            color: #ffffff;
            font-weight:bold;
            padding: 1px 1px 1px 1px;
            border-radius: 4px;
        }
    )");
    m_timeLabel->setStyleSheet(
        R"(
        QLabel {
            color: #7A7A7A;
        }
    )");
    updateLabelStyle();
}

void DanmakuItemWidget::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::PaletteChange) {
        updateLabelStyle();
    }
    QWidget::changeEvent(event);
}

bool DanmakuItemWidget::eventFilter(QObject *watched, QEvent *event)
{
    auto me { static_cast<QMouseEvent*>(event) };
    if (watched == m_nameLabel) {
        if (event->type() == QEvent::MouseButtonRelease && me->button() == Qt::LeftButton) {
            QDesktopServices::openUrl(QUrl(QStringLiteral("https://space.bilibili.com/%1").arg(m_uid)));
        }
    }

    return QWidget::eventFilter(watched, event);
}

void DanmakuItemWidget::updateLabelStyle()
{
    QPalette pal            { palette() };
    QColor   normalColor    { pal.color(QPalette::WindowText) };
    QColor   secondaryColor { normalColor };

    if (normalColor.lightness() > 128) {
        secondaryColor = secondaryColor.darker(150);
    } else {
        secondaryColor = secondaryColor.lighter(150);
    }

    m_nameLabel->setStyleSheet(
    QString(R"(
        QLabel {
            color: %1;
            font-size: 12px;
        }
        QLabel:hover {
            color: #0078d4;
            font-weight: bold;
        }
    )").arg(secondaryColor.name())
    );

    m_textLabel->setStyleSheet(
    QString(R"(
        QLabel {
            color: %1;
            font-size: 14px;
        }
        QLabel:hover {
            font-weight: bold;
        }
    )").arg(normalColor.name())
    );
}
