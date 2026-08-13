#include "bilibilidanmakuapi.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QList>
#include <QMessageBox>

BilibiliDanmakuApi::BilibiliDanmakuApi(QObject *parent) :
    QObject{parent},
    m_networkManager(new QNetworkAccessManager(this))
{
}

//
// todo: official api maybe need cookies, which need a tool widget add it
// get ture room id
// https://api.live.bilibili.com/room/v1/Room/room_init?id=<short id <- input roomId param>
// get live room info
// https://api.live.bilibili.com/room/v1/Room/get_info?room_id=<true room id
// get user info (need uid)
// https://api.live.bilibili.com/live_user/v1/Master/info?uid=<uid>
// get super chat
// https://api.live.bilibili.com/av/v1/SuperChat/getMessageList?room_id=xxxxx
// get normal danmakus
// http://api.live.bilibili.com/ajax/msg?roomid=<true room id>
//
void BilibiliDanmakuApi::getRoomDanmaku(const QString &roomId)
{
    QString sc_api { "https://api.live.bilibili.com/av/v1/SuperChat/getMessageList?room_id=" };
    QUrl sc_url { sc_api + roomId };
    QNetworkRequest sc_request { sc_url };
    QNetworkReply* sc_reply { m_networkManager->get(sc_request) };
    connect(sc_reply, &QNetworkReply::finished, this,
            [sc_reply, roomId, this]() {

        QList<Danmaku> danmaku_list { parseScNetworkReply(sc_reply, roomId) };

        emit danmakuReceived(danmaku_list);
        sc_reply->deleteLater();
    });

    QString api { "http://api.live.bilibili.com/ajax/msg?roomid=" };
    QUrl url { api + roomId };
    QNetworkRequest request { url };
    QNetworkReply* reply { m_networkManager->get(request) };

    connect(reply, &QNetworkReply::finished, this,
        [reply, roomId, this]() {

        QList<Danmaku> danmaku_list { parseNormalNetworkReply(reply, roomId) };

        emit danmakuReceived(danmaku_list);
        reply->deleteLater();
    });
}

Danmaku BilibiliDanmakuApi::parseDanmaku(const QJsonObject &object, const QString &roomId, bool isSc)
{
    Danmaku danmaku;
    danmaku.roomId          = roomId;
    if (!isSc) {
        danmaku.id          = object["id_str"].toString();
        danmaku.isAdmin     = (object["isadmin"].toInt() == 1);
        danmaku.isSc        = false;
        danmaku.nickname    = object["nickname"].toString();
        danmaku.text        = object["text"].toString();
        danmaku.uid         = object["uid"].toString();
        danmaku.time        = QDateTime::fromString(
            object["timeline"].toString(),
            "yyyy-MM-dd hh:mm:ss"
            );
        QJsonObject userInfo { object["user"].toObject()["base"].toObject() };
        danmaku.face_url    = userInfo["face"].toString();
    } else {
        danmaku.id          = object["id"].toString();
        danmaku.isAdmin     = false;  // todo: sc maybe admin
        danmaku.isSc        = true;
        danmaku.text        = object["message"].toString();
        danmaku.uid         = object["uid"].toString();
        danmaku.time        =  QDateTime::fromSecsSinceEpoch(
                object["ts"].toInteger(-1)
            );
        QJsonObject userInfo { object["user_info"].toObject() };
        danmaku.nickname    = userInfo["uname"].toString();
        danmaku.face_url    = userInfo["face"].toString();
    }

    return danmaku;
}

QList<Danmaku> BilibiliDanmakuApi::parseScNetworkReply(QNetworkReply *reply, const QString &roomId)
{
    QList<Danmaku> danmaku_list;
    QByteArray raw_data { reply->readAll() };
    // qDebug().noquote() << QString::fromUtf8(data);  // remove '\'
    QJsonParseError error;
    QJsonDocument document { QJsonDocument::fromJson(raw_data, &error) };

    if (error.error != QJsonParseError::NoError) {
        qDebug() << "JSON parse error(ture room):" << error.errorString();
        return danmaku_list;
    }

    if (!document["message"].toString().isEmpty()) {
        QJsonObject root { document.object() };
        QJsonObject data { root["data"].toObject() };
        QJsonArray  sc   = data["list"].toArray();

        for (const QJsonValue& value : std::as_const(sc)) {
            Danmaku danmaku {
                parseDanmaku(
                    value.toObject(),
                    roomId,
                    true
                    )
            };
            danmaku_list.append(danmaku);
        }
    }

    return danmaku_list;
}

QList<Danmaku> BilibiliDanmakuApi::parseNormalNetworkReply(QNetworkReply *reply, const QString &roomId)
{
    QList<Danmaku> danmaku_list;
    QByteArray raw_data { reply->readAll() };
    // qDebug().noquote() << QString::fromUtf8(data);  // remove '\'
    QJsonParseError error;
    QJsonDocument document { QJsonDocument::fromJson(raw_data, &error) };

    if (error.error != QJsonParseError::NoError) {
        qDebug() << "JSON parse error(real room):" << error.errorString();
        return danmaku_list;
    }

    // qDebug().noquote() << document.toJson(QJsonDocument::Indented);
    QJsonObject root { document.object() };
    QJsonObject data { root["data"].toObject() };
    // !! Array could save Array, do not use "{}" initializer, otherwise there is just one array element(size = 1)
    // QJsonArray  admin { data["admin"].toArray() };
    // QJsonArray  room  { data["room"].toArray() };
    QJsonArray  admin  = data["admin"].toArray();
    QJsonArray  room  = data["room"].toArray();
    // qDebug() << "admin Danmaku count:" << admin.size();
    // qDebug() << "room Danmaku count:" << room.size();

    if (!room.isEmpty()) {
        for (const QJsonValue& value : std::as_const(admin)) {
            Danmaku danmaku {
                parseDanmaku(
                    value.toObject(),
                    roomId
                    )
            };
            danmaku_list.append(danmaku);
        }
        for (const QJsonValue& value : std::as_const(room)) {
            Danmaku danmaku {
                parseDanmaku(
                    value.toObject(),
                    roomId
                    )
            };
            danmaku_list.append(danmaku);
        }

        // for (const Danmaku& item : list) {
        //     qDebug()
        //     << item.nickname
        //     << ":"
        //     << item.text;
        // }
    }

    return danmaku_list;
}
