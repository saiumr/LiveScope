#ifndef ROOMWIDGET_H
#define ROOMWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QTimer>
#include <QSet>
#include <QLabel>
#include <QDateTime>
#include <QEvent>
#include <QFocusFrame>
#include "model/Danmaku.h"
#include "model/LiveRoom.h"
#include "model/BilibiliUser.h"
#include "network/bilibilidanmakuapi.h"
#include "network/bilibililiveapi.h"
#include "network/bilibiliuserapi.h"

const int kRequestInterval { 3000 };   // ms

class RoomWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RoomWidget(const QString& roomId, QWidget *parent = nullptr);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
    void changeEvent(QEvent *event) override;

signals:
    void roomTitleChanged(const QString& title);

private slots:
    void onDanmakuReceived(const QList<Danmaku>& danmakus);
    void onLiveRoomReceived(const LiveRoom &room);
    void onUserInfoReceived(const BilibiliUser& user);
    void requestDanmaku();

private:
    QString                m_roomId;
    QString                m_uid;
    QString                m_name;
    QString                m_title;
    QDateTime              m_livingStartTime;
    BilibiliDanmakuApi*    m_danmakuApi;
    BilibiliLiveApi*       m_liveApi;
    BilibiliUserApi*       m_userApi;
    QTimer*                m_timer;
    QSet<QString>          m_receivedIds;
    QListWidget*           m_danmakuList;
    QLabel*                m_streamerLabel;
    QLabel*                m_titleLabel;
    QLabel*                m_descriptionLabel;
    QLabel*                m_statusIndicator;
    QLabel*                m_statusLabel;

    QNetworkAccessManager* m_imageManager;
    QLabel*                m_avatarLabel;

    void setupUi();
    void setupApi();
    void setupConnections();
    void updateLabelStyle();
    QColor getBeautifulHoverColor(const QColor &normal);
};

#endif // ROOMWIDGET_H
