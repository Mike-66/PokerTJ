#ifndef GAME_H
#define GAME_H

#include <QTime>

#include "Deck.h"
#include "Player.h"
#include "Board.h"
#include "QDebug"

#include <QObject>
#include <QElapsedTimer>

#include "DbQuery.hpp"

#define    MAXPLAYERS         10

#define    GS_WAITFORPLAYERS    -3
#define    GS_PAUSE             -2
#define    GS_INIT              -1
#define    GS_BET_FOR_BLINDS     0
#define    GS_START              1
#define    GS_BET_FOR_PREFLOP    2
#define    GS_FLOP               3
#define    GS_BET_FOR_FLOP       4
#define    GS_TURN               5
#define    GS_BET_FOR_TURN       6
#define    GS_RIVER              7
#define    GS_BET_FOR_RIVER      8
#define    GS_BET_FOR_SHOWDOWN   9
#define    GS_ROUND_FINISH      10
#define    GS_CHECK_PLAYERS     11
#define    GS_CLEAR             12
#define    GS_FINISH            13

#define    BET_FOR_BLINDS  -1
#define    BET_FOR_PREFLOP  0
#define    BET_FOR_FLOP     1
#define    BET_FOR_TURN     2
#define    BET_FOR_RIVER    3
#define    BET_FOR_SHOWDOWN 4

#define    MAX_PLAYER_STAGES 5

#define    COUNT_BLINDS     2

#define    GAME_MAXMESSAGES 30

#define    GAME_DONTSENDDATA -2
#define    GAME_SENDDATA     -1
#define    GAME_SENDDATAUSER  0

#define    BLINDS_ARRAY_MAX   10

class TGame : public QObject
{
   Q_OBJECT

private:
protected:
    void SetNextPlayerRec(int inactive_mask);

    int  Blinds_Array[BLINDS_ARRAY_MAX];
    void RaiseBlinds(void);

public:
    TGame();
    TGame(int CountPlayers_Default, int StrengthOfBots, int Credit, int BigBlind, int RaiseBlindMinutes, QString Name, QString Key);
    ~TGame();

    int Init(int CountPlayers_Default, int StrengthOfBots, int Credit, int BigBlind, int RaiseBlindMinutes, QString Name, QString Key);

    QJsonObject AsJsonObject(void);
    void        FromJsonObject(QJsonObject* O);
    void        BoardFromJsonObject(QJsonObject *O);
    int         PlayerFromJsonObject(QJsonObject *O);

    void        PrepareRefresh(void);
    void        CheckRefresh(void);

    int         isLocalGame;

    int         iWindowPlayer;
    int         iPlayerSeatRequested;
    int         iRebuyRequested;
    QString     ExternalBet_UserKey;

    TDeck       *Deck;
    TBoard      *Board;

    QString Name;
    QString Key;
    int     Credit;

    int     StrengthOfBots;

    int  RoundCount;

    int  Status_No_Mask;
    int  Status_Bet_Inactive_Player;
    int  Status_Showdown_Inactive_Player;

    int  Continue;
    int  RunMode;

    int CountPlayers_Default;
    int CountPlayers;

    TPlayer *Player[MAXPLAYERS];

    int PlayerArray[4][2];
    int BoardArray[5];

    int     Blind[COUNT_BLINDS];
    int     RaiseBlindMinutes;
    int     BlindsRequested;

    int     iDealer;
    int     iPlayer;
    int     iBetRoundFinishPlayer;

    int     Round;
    int     Stage;
    int     StageOld;
    QString DisplayStageName;

    int     Pot;

    int     HighestBet;

    int     CallBet;
    int     RaiseBet;

    int     CountMessages;
    QString Message[GAME_MAXMESSAGES];
    void    AddMessage(QString msg);

    QString StageName[MAX_PLAYER_STAGES];

    int     iLastRaisePlayer;
    int     iShowDownInit;

    //Rest bei geteilten Pots;
    int     BankWin;

    int     iSendDataTo;
    int     iSendDataTo_EXTERN;

    QList<TUser *> Spotters;
    void    AddSpotter(TUser* User);

    void    NextStep();

    int  AddPlayer(int index, TUser *User, int Credit);
    int  PerformRebuy(int index, int amount);
    void RemovePlayer(TPlayer *p);
    void RemovePlayer(int iplayer);
    void GiveCardsToPlayers(void);
    void GiveCardsToFlop(void);
    void GiveCardsToTurn(void);
    void GiveCardsToRiver(void);
    void Clear(void);

    int  Bet(int whatfor);
    void ClearBets(void);

    int GetRandomCardIndex(void);
    int SetCardLocation(int Location);
    int GetRaiseBlindsIdx(void);

    int  SetNextPlayer(int inactive_mask);
    void SetNextDealer(void);
    void SetPlayer(int inactive_mask);
    void InitRound(void);
    void RotatePlayer(int *iplayer);
    void SetBetRoundFinishPlayer(void);
    int  GetPlayerPosition(int iplayer);

    int  CheckActivePlayersJump(int stagefrom, int stageto);

    int  GetLowestSidePot(void);
    int  RoundFinish(void);
    void CheckPlayers(void);

    void SetKownCardsForDebugging(void);

    void FillWithBots(void);
    int  GetCountBots(void);

    QElapsedTimer  PauseTimer;
    QElapsedTimer  RaiseBlindsTimer;
    int            Continue_Old;

//EXTERNAL CONFIG ++
    int  ContinueAllFolded;
    int  PauseBetweenRounds;
//EXTERNAL CONFIG --

public slots:
    void SlotDataReceived(QString data);
};

#endif // GAME_H
