#ifndef BILIBILIROOMINITAPI_H
#define BILIBILIROOMINITAPI_H

#include <QObject>
#include <QNetworkAccessManager>

class BilibiliRoomInitApi : public QObject
{
    Q_OBJECT
public:
    explicit BilibiliRoomInitApi(QObject *parent = nullptr);
    void verifyRoomId(const QString &inputRoomId);

signals:
    void roomVerifyOk(QString realRoomId, QString displayName);
    void roomVerifyFail(QString errMsg);

private:
    QNetworkAccessManager*  m_networkManager;
};

#endif // BILIBILIROOMINITAPI_H
