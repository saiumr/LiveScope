#ifndef LIVEROOM_H
#define LIVEROOM_H

#include <QString>

struct LiveRoom {
    QString uid;
    QString roomId;
    QString title;
    QString description;
    enum class Status {
        Offline,
        Live,
        RoundPlay
    };
    Status status {Status::Offline};
};

#endif // LIVEROOM_H
