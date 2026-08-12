#ifndef BILIBILILIVEAPI_H
#define BILIBILILIVEAPI_H

#include <QObject>
#include <QNetworkAccessManager>
#include "model/LiveRoom.h"

class BilibiliLiveApi : public QObject
{
    Q_OBJECT
public:
    explicit BilibiliLiveApi(QObject *parent = nullptr);
    void getLiveRoomInfo(const QString& roomId);

signals:
    void liveRoomReceived(const LiveRoom& room);

private:
    QNetworkAccessManager* m_manager;
};

#endif // BILIBILILIVEAPI_H
