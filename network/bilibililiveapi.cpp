#include "bilibililiveapi.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

BilibiliLiveApi::BilibiliLiveApi(QObject *parent)
    : QObject{parent},
    m_manager{new QNetworkAccessManager(this)}
{

}

void BilibiliLiveApi::getLiveRoomInfo(const QString &roomId)
{
    QUrl url { QString("https://api.live.bilibili.com/room/v1/Room/get_info?room_id=%1").arg(roomId)};
    QNetworkRequest request { url };
    QNetworkReply* reply { m_manager->get(request) };
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        QByteArray raw_byte_data { reply->readAll() };
        QJsonParseError error;
        QJsonDocument doc { QJsonDocument::fromJson(raw_byte_data, &error) };
        if (error.error != QJsonParseError::NoError) {
            qDebug() << "Live romm JSON parse error:" << error.errorString();
            return;
        }
        QJsonObject obj { doc["data"].toObject() };

        // qDebug().noquote() << doc.toJson(QJsonDocument::Indented);
        // qDebug() << obj["uid"];

        LiveRoom room;
        room.uid = QString::number(static_cast<qlonglong>(obj["uid"].toDouble()));
        room.roomId = QString::number(static_cast<qlonglong>(obj["room_id"].toDouble()));
        room.title = obj["title"].toString();
        room.description = obj["description"].toString();
        int status { obj["live_status"].toInt() };
        switch (status) {
        case 1:
            room.status = LiveRoom::Status::Live;
            break;
        case 2:
            room.status = LiveRoom::Status::RoundPlay;
            break;
        default:
            room.status = LiveRoom::Status::Offline;
            break;
        }

        emit liveRoomReceived(room);

        reply->deleteLater();
    });
}
