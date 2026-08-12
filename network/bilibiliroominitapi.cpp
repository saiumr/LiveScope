#include "bilibiliroominitapi.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QList>

BilibiliRoomInitApi::BilibiliRoomInitApi(QObject *parent)
    : QObject{parent},
    m_networkManager{new QNetworkAccessManager(this)}
{}


void BilibiliRoomInitApi::verifyRoomId(const QString &inputRoomId)
{
    QString fake_api { "https://api.live.bilibili.com/room/v1/Room/room_init?id=" };  // use fake room id find ture that
    QUrl    fake_url { fake_api + inputRoomId };
    QNetworkRequest fake_request { fake_url };
    QNetworkReply* fake_reply { m_networkManager->get(fake_request) };

    connect(fake_reply, &QNetworkReply::finished, this, [fake_reply, inputRoomId, this]() {
        // Network check
        if (fake_reply->error() != QNetworkReply::NoError) {
            emit roomVerifyFail(QString("Network error: %1").arg(fake_reply->errorString()));
            fake_reply->deleteLater();
            return;
        }

        int httpCode { fake_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() };
        if (httpCode != 200) {
            emit roomVerifyFail(QString("Network error: http error code %1").arg(httpCode));
            fake_reply->deleteLater();
            return;
        }

        // room info
        QByteArray      raw_data { fake_reply->readAll() };
        QJsonParseError error;
        QJsonDocument   document { QJsonDocument::fromJson(raw_data, &error) };
        if (error.error != QJsonParseError::NoError) {
            qDebug() << "JSON parse error(fake room):" << error.errorString();
            fake_reply->deleteLater();
            return;
        }
        QJsonObject root { document.object() };
        if (root["code"].toInt() != 0) {
            emit roomVerifyFail(QString("The room(id=%1) is not exist!").arg(inputRoomId));
            fake_reply->deleteLater();
            return;
        }
        QJsonObject object { root["data"].toObject() };
        // qDebug() << object["room_id"];
        QString realRoomId { QString::number(static_cast<qlonglong>(object["room_id"].toDouble())) };
        // qDebug() << "real room id: " << realRoomId;

        emit roomVerifyOk(realRoomId, inputRoomId);

        fake_reply->deleteLater();
    });

}