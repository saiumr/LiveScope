#ifndef BILIBILIUSERAPI_H
#define BILIBILIUSERAPI_H

#include <QObject>
#include <QNetworkAccessManager>
#include "model/BilibiliUser.h"

class BilibiliUserApi : public QObject
{
    Q_OBJECT
public:
    explicit BilibiliUserApi(QObject *parent = nullptr);
    void getUserInfo(QString uid);

signals:
    void userInfoReceived(const BilibiliUser& user);

private:
    QNetworkAccessManager* m_manager;
    void verifyRoomId(const QString &inputRoomId);
};

#endif // BILIBILIUSERAPI_H
