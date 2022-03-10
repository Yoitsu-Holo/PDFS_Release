#ifndef USERINFO_H
#define USERINFO_H

#include <QString>
#include <QByteArray>

struct UserInfo
{
    QString username;
    QString password;
    QByteArray key;
};


#endif // USERINFO_H

