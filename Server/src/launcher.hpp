#ifndef LAUNCHER_HPP
#define LAUNCHER_HPP

#include <QObject>
#include <QDebug>
#include <QTimer>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QSettings>

#include "ws_server/ws_server.h"
#include "../../Base/Game.h"
#include "../../Base/user.hpp"
#include "../../Base/DbQuery.hpp"

class Launcher : public QObject
{
    Q_OBJECT
public:
    Launcher();

    TWS_Server *WS_Server;


protected:

    QTimer        *WorkTimer;

    qint64        WorkTimerInterIntervall;
    qint64        WorkTimerInterIntervallDesired;

    QElapsedTimer LoadTimer;

    QElapsedTimer ResponseTimer;
    qint64        ResponseTimeTotal;

    QList<TUser *> ActiveUsers;
    int    CheckUsersInterval;
    int    CheckUsersTOis;
    qint64 UserInactiveTimeout;
    void CheckUsers(void);

    bool           LoadSettings(QString FileName);
    void           SaveSettings(QString FileName);

    //TDbQuery       *DbQuery;

    QList<TGame *> Games;
    TGame* CreateGame(int CountPlayers, int StrengthOfBots, int Credit, int BigBlind, int RaiseBlindMinutes, QString Name);

    void LogOff(TUser *User);
    int    RemoveUserFromGame(TUser* User);

    TGame* GetGameByKey(QString GameKey);
    TGame* GetGameByUserKey(QString UserKey, TPlayer **PLayer, int *iPlayer);
    TGame* GetGameByUserKeySpotter(QString UserKey);
    TGame* GetGameByName(QString GameName);
    int    GetPlayerIndex(TGame* Game, QString UserKey);

    void   CopyGameToJsonObject(TGame *Game,int iPlayer,QJsonObject *O);
    void   CopyGameStatsToJsonObject(TGame *Game,QJsonObject *O);

    void   PrepareAvailableGames(QString UserKey, QJsonObject *jsonObj_out);

    void   ProcessKeepAliveMessage(QJsonObject *jsonObj_in,QJsonObject *jsonObj_out);
    void   ProcessContinueMessage(QString UserKey,QJsonObject *jsonObj_in,QJsonObject *jsonObj_out);
    void   ProcessExternalBetMessage(QString UserKey, QJsonObject *jsonObj_in,QJsonObject *jsonObj_out);
    TUser* ProcessLoginMessage(QWebSocket *pClient,QJsonObject *jsonObj_in,QJsonObject *jsonObj_out);
    void   ProcessPlayerStateMessage(QString UserKey,QJsonObject *jsonObj_in,QJsonObject *jsonObj_out);
    void   ProcessAvailableGames(QString UserKey,QJsonObject *jsonObj_in,QJsonObject *jsonObj_out);
    void   ProcessGameStateMessage(QString UserKey, QJsonObject *jsonObj_in,QJsonObject *jsonObj_out);
    void   ProcessGameStatsMessage(QString UserKey, QJsonObject *jsonObj_in,QJsonObject *jsonObj_out);
    void   ProcessRequestGameMessage(QString UserKey,int iUser,QJsonObject *jsonObj_in,QJsonObject *jsonObj_out);
    void   ProcessRequestPlayerSeat(QString UserKey,int iUser,QJsonObject *jsonObj_in,QJsonObject *jsonObj_out);
    void   ProcessRequestRebuy(QString UserKey,int iUser,QJsonObject *jsonObj_in,QJsonObject *jsonObj_out);
    void   ProcessRequestCreateGame(QString UserKey, int iUser,QJsonObject *jsonObj_in,QJsonObject *jsonObj_out);
    void   ProcessRequestLeaveGame(int iUser,QJsonObject *jsonObj_in,QJsonObject *jsonObj_out);

    int    ServerMessageId;
    int    ServerMessageIdSent;
    void   SendJsonDocument(QWebSocket *pClient, QString MessageType, int MessageId, QJsonObject* jsonObj_out);

public slots:
    void onTimer();
    void on_WS_MessageReceived(QString data);
    void on_WS_BinaryMessageReceived(QWebSocket* pClient,QByteArray message);
    void on_WS_Socketdisonnected(QWebSocket* pClient);
};

#endif // LAUNCHER_HPP
