#ifndef LAUNCHERDIALOG_H
#define LAUNCHERDIALOG_H

#include <QDialog>
#include "../Base/Helpers.hpp"
#include "mainwindow.h"
#include "gamemodel.hpp"

#define LD_CONNECT_TYPE_LOCAL    0
#define LD_CONNECT_TYPE_ONLINE   1

#define LD_INIT   -1
#define LD_OK      0
#define LD_CANCEL  1

#define LD_ACTION_INIT        -1
#define LD_ACTION_CREATE_GAME  0
#define LD_ACTION_REQUEST_GAME 1
#define LD_ACTION_LEAVE_GAME   2
#define LD_ACTION_RELOAD       3

#define VOLUME_SLIDER_RANGE  100.


namespace Ui {
class LauncherDialog;
}

class LauncherDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LauncherDialog(QWidget *parent = nullptr);
    ~LauncherDialog();

    MainWindow *w;

    void Init();
    void SetMainWindow(MainWindow *w);

    int Result;
    int Action;

    int      ConnectType;

    int      NumberofPlayers;
    int      Credit;
    QString  GameName;
    int      StrenghOfBots;
    int      BigBlind;
    int      RaiseBlindMinutes;

    //QList<QString> GameKeys;

    TGameModel GameModel;
    TGameModel emptyGameModel;

    QString UserName;
    QString Password;
    QString Server;
    QString NickName;

    QString GameKey;

    QString  CreateGameName;
    int      connecttoserver;

    void ClearAvailableGames();
    void AddAvailableGame(QString name,int Players,int ActivePlayers,int Spotters, int Bots,int Strength,int Credit,
                                          int BigBlind,int Raise,QString Key);
    void ShowAvailableGames(QString GameKey);

    void SetRoundTrip(double roundtrip);

protected:
    void closeEvent (QCloseEvent *event);
    void showEvent(QShowEvent *event);

public slots:
    void onOKclicked();
    void onCancelclicked();
    void onReloadclicked();
    void OnPBLoginClicked();
    void OnPBCreateGameClicked();
    void OnPBLeaveGameClicked();
    //void OnCBGamecurrentIndexChanged(int index);
    //void OnCBtextActivated(const QString &text);
    void OnGameTableClicked(const QModelIndex &index);
    void OnRBToggled(bool checked);
    void OnCBShowKeyBindsChecked();
    void OnCBShowMouseBindsChecked();
    void onCBActionByKeyChecked();
    void onCBActionByMouseChecked();
    void onhSLPlayersTurnVolumeReleased();
    void onhSLActionWarningVolumeReleased();
    void onCBActionPercentWarningcurrentTextChanged(const QString &text);

private:
    Ui::LauncherDialog *ui;
};

#endif // LAUNCHERDIALOG_H
