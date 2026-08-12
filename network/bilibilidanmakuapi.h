#ifndef BILIBILIDANMAKUAPI_H
#define BILIBILIDANMAKUAPI_H

#include <QObject>
#include <QNetworkAccessManager>
#include "model/Danmaku.h"

class BilibiliDanmakuApi : public QObject
{
    Q_OBJECT

public:
    explicit BilibiliDanmakuApi(QObject *parent = nullptr);
    void getRoomDanmaku(const QString& roomId);

signals:
    void danmakuReceived(const QList<Danmaku>& danmakus);

private:
    QNetworkAccessManager* m_networkManager;
    Danmaku parseDanmaku(
        const QJsonObject& object,
        const QString& roomId,
        bool isSc = false
    );

    QList<Danmaku> parseScNetworkReply(QNetworkReply* reply, const QString& roomId);
    QList<Danmaku> parseNormalNetworkReply(QNetworkReply* reply, const QString& roomId);
};

#endif // BILIBILIDANMAKUAPI_H
