#ifndef BILIBILIUSER_H
#define BILIBILIUSER_H

#include <QString>

struct BilibiliUser
{
    QString uid;
    QString name;
    // QString sex;
    QString face_url;
    // QString sign;
    // int     level;
    // int     followingCount;
    int     followerCount;
};

#endif // BILIBILIUSER_H
