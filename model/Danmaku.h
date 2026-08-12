#ifndef DANMAKU_H
#define DANMAKU_H

#include <QString>
#include <QDateTime>

// this api data has many peoperty, here just a part of them

struct Danmaku
{
    QString   id;
    QString   roomId;
    bool      isAdmin;
    bool      isSc;
    QString   face_url;
    QString   nickname;
    QString   text;
    QString   uid;
    QDateTime time;
};

#endif // DANMAKU_H
