#include "bilibiliuserapi.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

BilibiliUserApi::BilibiliUserApi(QObject *parent)
    : QObject{parent},
    m_manager{new QNetworkAccessManager(this)}
{}

void BilibiliUserApi::getUserInfo(QString uid)
{
    QUrl url { QString("https://api.live.bilibili.com/live_user/v1/Master/info?uid=%1").arg(uid) };
    QNetworkRequest request { url };
    QNetworkReply* reply { m_manager->get(request) };
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        QByteArray raw_byte_data { reply->readAll() };
        QJsonParseError error;
        QJsonDocument doc { QJsonDocument::fromJson(raw_byte_data, &error) };
        if (error.error != QJsonParseError::NoError) {
            qDebug() << "User JSON parse error:" << error.errorString();
            return;
        }
        QJsonObject obj { doc["data"].toObject() };

        BilibiliUser user;
        QJsonObject user_info { obj["info"].toObject() };
        user.uid      = QString::number( static_cast<qlonglong>(user_info["uid"].toDouble()) );
        user.name     = user_info["uname"].toString();
        // user.sex      = user_info["gender"].toString();
        user.face_url = user_info["face"].toString();
        // user.followingCount = obj["following"].toInt();
        user.followerCount = obj["follower_num"].toInt();

        emit userInfoReceived(user);

        reply->deleteLater();
    });

}
