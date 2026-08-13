#ifndef LIVEROOM_H
#define LIVEROOM_H

#include <QString>
#include <QDateTime>

struct  LiveRoom {
    QString uid;
    QString roomId;
    QString title;
    QString description;
    QDateTime   liveTime;
    enum class Status {
        Offline,
        Live,
        RoundPlay
    };
    Status status {Status::Offline};
};

#endif // LIVEROOM_H
