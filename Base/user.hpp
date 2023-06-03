#ifndef TUSER_HPP
#define TUSER_HPP


#include <QObject>
#include <QtWebSockets/QWebSocket>
#include <QJsonObject>
#include <QJsonArray>

#include "Helpers.hpp"

class TUser : public QObject
{
    Q_OBJECT
public:
    explicit TUser( QObject *parent = nullptr);
    explicit TUser( QString UserName, QString NickName, QString Type, int AI_Class, QString Key, QString GameKey, QWebSocket *pClient, QObject *parent = nullptr);

    QString    UserName;
    QString    NickName;
    QString    Type;
    QString    Key;
    QString    GameKey;
    QWebSocket *pClient;
    qint64     LastSeen;
    int        AI_Class;

    qint64     BannedUntil;

    QJsonObject AsJsonObject(int detail);
    void        FromJsonObject(QJsonObject *O);

    void       UpdateLastSeen();


signals:

};

TUser* CreateUserObject( QString UserName, QString NickName, QString Type, int AI_Class, QString Key, QString GameKey, QWebSocket *pClient);

#endif // USER_HPP
