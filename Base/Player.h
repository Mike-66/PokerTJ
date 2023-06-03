#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include <QDebug>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "Hand.h"
#include "Board.h"
#include "user.hpp"

#define NUM_PLAYER_CARDS 2

#define PLAYER_STATUS_NO_CREDIT       0x00000001
#define PLAYER_STATUS_FOLDED          0x00000002
#define PLAYER_STATUS_ALL_IN          0x00000004
#define PLAYER_STATUS_ALL_IN_CHECKED  0x00000008
#define PLAYER_STATUS_SHOW_CARDS      0x00000010
#define PLAYER_STATUS_SIDEPOT_CHECKED 0x00000020
#define PLAYER_STATUS_WINNER          0x00000040
#define PLAYER_STATUS_WINNER_CHECKED  0x00000080

#define AI_HUMAN                      0
#define AI_COMPUTER_1                 1
#define AI_COMPUTER_2                 2
#define AI_COMPUTER_3                 3
#define AI_COMPUTER_4                 4
#define AI_COMPUTER_5                 5

#define  EXTERNALBET_FOLD             -4
#define  EXTERNALBET_INIT             -3
#define  EXTERNALBET_REQUESTED        -2
#define  EXTERNALBET_SENT             -1

#define  EXTERNALBET_CALLBET_FLIP_CARDS -1

class TPlayer
{
    private:
    protected:
      unsigned long long Result;

    public:
      TUser    *User;
      THand    *Hand;

      int      refreshed;

      int Credit;
      int ActualBet;
      int TotalBet;
      int SidePot;

      int ExternalBet;

      TPlayer(TDeck *Deck, TBoard *Board);
      TPlayer(TDeck *Deck, TBoard *Board, TUser *User, int AI_Class);
      ~TPlayer();

      QString GetNickName(void);
      int     GetAI_Class(void);

      int validcards;
      int iCard[NUM_PLAYER_CARDS];

      int Status;
      int Winner;

      void Clear(void);
      void SetCard(int cardindex);

      void CheckHand(void);

      int  Bet(int whatfor, int position, int blind, int minimum);
      void SetStatus(int status);
      int  GetStatus(int status);

      unsigned long long GetResult(void);

      QJsonObject AsJsonObject(int index,int detail);
      void        FromJsonObject(QJsonObject *O);

      QJsonObject StatAsJsonObject(int index);
};

#endif // PLAYER_H
