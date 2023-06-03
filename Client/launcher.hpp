#ifndef LAUNCHER_HPP
#define LAUNCHER_HPP

#include <QObject>
#include <QDebug>
#include <QTimer>
#include <QElapsedTimer>

#include <QJsonDocument>
#include <QJsonObject>

#include "mainwindow.h"
#include "wss_client.h"
#include "../Base/user.hpp"
#include "../Base/Helpers.hpp"
#include "launcherdialog.hpp"
#include "QMessageBox"

#define STAGE_LD_ENTERING   0
#define STAGE_LD_VISIBLE    1
#define STAGE_GAME_ENTERING 2
#define STAGE_GAME_RUNNING  3


class Launcher : public QObject
{
    Q_OBJECT
public:
    Launcher();
    ~Launcher();

    QString        IniFileName;
    void           LoadSettings(void);
    void           SaveSettings(void);

    void           ShowMessageBox(QString Text);
    int            MessageBoxVisible;

    void           Work();
    int            doExit;

protected:
    TWSS_Client    *WSS_Client;
    MainWindow     *w;
    TGame          *g;
    LauncherDialog *ld;

    int            stage;

    int            EnterGameTimeout;
    int            EnterGameTimeoutIs;

    int            WSS_CheckTimeout;
    int            WSS_CheckTimeoutIs;

    int            WSS_ServerTimeout;
    int            WSS_ServerTimeoutIs;

    QString        ServerRoute;
    QString        ServerPrefix;

    QElapsedTimer  ResponseTimer;

    QElapsedTimer  DebugTimer;

    void           LoadAvailableGames(QJsonObject *jsonObj_in);
    int            StartGame(int online);
    void           LogOff(void);

    TUser          *MeMySelf;

    //QString        UserKey;
    //QString        UserType;
    //QString        GameKey;

    int            MessageId;
    int            MessageIdSent;
    void           SendRequest(QJsonObject *jsonObj);

    void           RequestKeepAlive();
    void           RequestPlayerSeat(void);
    void           RequestRebuy(void);
    void           RequestLeaveGame(void);
    void           RequestGame();
    void           RequestCreateGame();
    void           RequestContinue(void);
    void           RequestExternalBet(void);
    void           RequestPlayerStateUnsol(void);
    void           RequestLogin(QJsonObject *jsonObj_out);
    void           RequestPlayerState(QJsonObject *jsonObj_out);
    void           RequestAvailableGames(void);
    void           RequestGameState(QJsonObject *jsonObj_out);
    void           RequestGameStats(TGame *Game);

    void           ProcessInternalMessage(QJsonObject *jsonObj_in,QJsonObject *jsonObj_out);
    void           ProcessKeepAliveMessage(QJsonObject *jsonObj_in,QJsonObject *jsonObj_out);
    void           ProcessLoginMessage(QJsonObject *jsonObj_in,QJsonObject *jsonObj_out);
    void           ProcessPlayerStateMessage(QJsonObject *jsonObj_in,QJsonObject *jsonObj_out);
    void           ProcessAvailableGames(QJsonObject *jsonObj_in,QJsonObject *jsonObj_out);
    void           ProcessGameStateMessage(QJsonObject *jsonObj_in,QJsonObject *jsonObj_out);
    void           ProcessRequestGame(QJsonObject *jsonObj_in, QJsonObject *jsonObj_out);
    void           ProcessRequestPlayerSeat(QJsonObject *jsonObj_in,QJsonObject *jsonObj_out);
    void           ProcessRequestRebuy(QJsonObject *jsonObj_in,QJsonObject *jsonObj_out);
    void           ProcessRequestCreateGame(QJsonObject *jsonObj_in,QJsonObject *jsonObj_out);
    void           ProcessGameStats(QJsonObject *jsonObj_in,QJsonObject *jsonObj_out);

public slots:

    void on_WSS_MessageReceived(QString data);
    void on_WSS_BinaryMessageReceived(QByteArray message);
};

#endif // LAUNCHER_HPP
