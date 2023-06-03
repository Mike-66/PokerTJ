#include "Player.h"
#include "Game.h"

TPlayer::TPlayer(TDeck *Deck, TBoard *Board)
{
 Hand=new THand(Deck,Board);
 User=NULL;
 Credit=1000;
 Clear();
}

//-----------------------------------------------------

TPlayer::TPlayer(TDeck *Deck, TBoard* Board, TUser* User, int Credit)
{
 Hand=new THand(Deck, Board);
 this->User=User;
 this->Credit=Credit;
 Status=0;
 ActualBet=0;
 TotalBet=0;
 SidePot=0;

 ExternalBet=EXTERNALBET_INIT;
 Clear();
}

//-----------------------------------------------------

TPlayer::~TPlayer()
{
 delete(Hand);
 if(User!=NULL && User->AI_Class!=AI_HUMAN)
 delete(User);
}

//-----------------------------------------------------

QString TPlayer::GetNickName(void)
{
 QString Name="NULLUser";
 if(User!=NULL)
 Name=User->NickName;
 return(Name);
}

//-----------------------------------------------------

int TPlayer::GetAI_Class(void)
{
 int AI_Class=AI_COMPUTER_1;
 if(User!=NULL)
 AI_Class=User->AI_Class;
 return(AI_Class);
}

//-----------------------------------------------------

QJsonObject TPlayer::AsJsonObject(int index, int detail)
{
    QJsonObject O;
    QJsonObject o;
    QJsonArray  a;
    O.insert("Index",index);
    if(User!=NULL)
    {
     o=User->AsJsonObject(detail);
     O.insert("User",o);
    }
    O.insert("Credit",Credit);
    O.insert("Status",Status);
    O.insert("Winner",Winner);
    O.insert("ActualBet",ActualBet);
    O.insert("TotalBet",TotalBet);
    O.insert("ExternalBet",ExternalBet);
    O.insert("SidePot",SidePot);
    O.insert("validcards",validcards);

    if( GetStatus(PLAYER_STATUS_SHOW_CARDS)>0 )
    detail=1;

    O.insert("detail",detail);
    for(int i=0;i<validcards;i++)
    {
     QJsonObject c;
     if(detail>0)
     c.insert("iCard",iCard[i]);
     else
     c.insert("iCard",-1);
     a.append(c);
    }
    O.insert("Cards",a);


    return(O);
}

//-----------------------------------------------------

void TPlayer::FromJsonObject(QJsonObject *O)
{
    QJsonObject o;

    //Load the user
    o = O[0]["User"].toObject();
    if(!o.isEmpty() && User==NULL)
    {
     User=new TUser;
     User->FromJsonObject(&o);
    }

    //Here comes the player
    Credit=O->value("Credit").toInt();
    Status=O->value("Status").toInt();
    Winner=O->value("Winner").toInt();
    ActualBet=O->value("ActualBet").toInt();
    TotalBet=O->value("TotalBet").toInt();
    ExternalBet=O->value("ExternalBet").toInt();
    SidePot=O->value("SidePot").toInt();
    validcards=O->value("validcards").toInt();
    int detail=O->value("detail").toInt();
    QJsonArray cardArray;
    cardArray = O[0]["Cards"].toArray();
    for(int i = 0; i < cardArray.size(); i++)
    {
     QJsonObject o=cardArray[i].toObject();
     iCard[i]=o.value("iCard").toInt();
    }
}

//-----------------------------------------------------

QJsonObject TPlayer::StatAsJsonObject(int index)
{
    QJsonObject O;
    QJsonArray a;
    QJsonObject o;
    O.insert("index",index);
    O.insert("NickName",GetNickName());
    o.insert("c",11);
    a.append(o);
    for(int i=NUM_POKER_HANDS-1;i>=0;i--)
    {
     o.empty();
     o.insert("c",i);
     a.append(o);
    }
    O.insert("PlayerStats",a);

    return(O);
}

//-----------------------------------------------------

void TPlayer::Clear(void)
{
 validcards=0;
 ActualBet=0;
 TotalBet=0;
 SidePot=0;
 Result=0;
 for(int i=0;i<NUM_PLAYER_CARDS;i++)
 iCard[i]=-1;

 Status&=~(PLAYER_STATUS_FOLDED |
           PLAYER_STATUS_ALL_IN |
           PLAYER_STATUS_ALL_IN_CHECKED |
           PLAYER_STATUS_SIDEPOT_CHECKED |
           PLAYER_STATUS_SHOW_CARDS |
           PLAYER_STATUS_WINNER |
           PLAYER_STATUS_WINNER_CHECKED);
}

//-----------------------------------------------------

void TPlayer::SetCard(int cardindex)
{
 iCard[validcards++]=cardindex;
}

//------------------------------------------------------------
void TPlayer::SetStatus(int status)
{
 Status|=status;

 if( GetStatus( PLAYER_STATUS_NO_CREDIT ) > 0)
 Log("SetStatus " + GetNickName() + " PLAYER_STATUS_NO_CREDIT");
 if( GetStatus( PLAYER_STATUS_FOLDED ) > 0)
 Log("SetStatus " + GetNickName() + " PLAYER_STATUS_FOLDED");
 if( GetStatus( PLAYER_STATUS_ALL_IN ) > 0)
 Log("SetStatus " + GetNickName() + " PLAYER_STATUS_ALL_IN");
 if( GetStatus( PLAYER_STATUS_ALL_IN_CHECKED ) > 0)
 Log("SetStatus " + GetNickName() + " PLAYER_STATUS_ALL_IN_CHECKED");
 if( GetStatus( PLAYER_STATUS_SIDEPOT_CHECKED ) > 0)
 Log("SetStatus " + GetNickName() + " PLAYER_STATUS_SIDEPOT_CHECKED");
 if( GetStatus( PLAYER_STATUS_WINNER ) > 0)
 Log("SetStatus " + GetNickName() + " PLAYER_STATUS_WINNER");
 if( GetStatus( PLAYER_STATUS_WINNER_CHECKED ) > 0)
 Log("SetStatus " + GetNickName() + " PLAYER_STATUS_WINNER_CHECKED");

}
//------------------------------------------------------------
int TPlayer::GetStatus(int status)
{
 if(status==0)
 return(Status);

 int statusbit=1;
 for(int i=0;i<8;i++)
 {
  if( (status & statusbit) == statusbit )
  if( (Status & statusbit) == statusbit )
  return(1);
  statusbit=statusbit<<1;
 }
 return(0);
}
//------------------------------------------------------------
int TPlayer::Bet(int whatfor, int position, int blind, int minimum)
{
 int bet=0;

 switch(whatfor)
 {
  case BET_FOR_BLINDS:
  {
   bet=minimum;
  }break;
  case BET_FOR_PREFLOP:
  case BET_FOR_FLOP:
  case BET_FOR_TURN:
  case BET_FOR_RIVER:
  {
   switch(GetAI_Class())
   {
    case AI_HUMAN:
    {
     if(ExternalBet==EXTERNALBET_FOLD)
     {
      SetStatus(PLAYER_STATUS_FOLDED);
     }
     else if(ExternalBet<=EXTERNALBET_SENT)
     {
      if(ExternalBet==EXTERNALBET_INIT)
      {
       Hand->Check(this);
       ExternalBet=EXTERNALBET_REQUESTED;
       return(1);
      }
      return(2); //wait
     }
     else
     {
      if( ExternalBet> 0 && ExternalBet + ActualBet < minimum )
      {
       //correcting bets for call
       ExternalBet=minimum-ActualBet;
       Log("in Player::Bet Call correct to : " + QString().setNum(minimum) + GetNickName());

      }
      if( ActualBet + ExternalBet > minimum && ActualBet + ExternalBet < minimum + blind )
      {
       //correcting bets for minimum raise
       ExternalBet = minimum-ActualBet+blind;
       Log("in Player::Bet Raise correct to : " + QString().setNum(ExternalBet) + GetNickName());
      }

      bet=ExternalBet;
     }
    }break;

    case AI_COMPUTER_1:
    {
       //check on zero else fold
       Hand->Check(this);
    }  break;
    case AI_COMPUTER_2:
    {
       Hand->Check(this);
       //allways call check
       bet=minimum-ActualBet;
    }  break;
    case AI_COMPUTER_3:
    {
       Hand->Check(this);
    }  break;
    case AI_COMPUTER_4:
    {

    }  break;
    case AI_COMPUTER_5:
    {
       Hand->Check(this);
    }  break;


   } //switch GetAI_Class()

  }break;
  case BET_FOR_SHOWDOWN:
  {
   if(GetAI_Class() > AI_HUMAN)
   {
    Hand->Check(this);
    //TODO: need access to former players which dind't fold and decide
    //for now: dont' fold
    SetStatus(PLAYER_STATUS_SHOW_CARDS);
    return(0);
   }
   else
   {
    if(ExternalBet==EXTERNALBET_FOLD)
    {
     SetStatus(PLAYER_STATUS_FOLDED);
     goto finish;
    }
    else if(ExternalBet<=EXTERNALBET_SENT)
    {
     if(ExternalBet==EXTERNALBET_INIT)
     {
      Hand->Check(this);
      ExternalBet=EXTERNALBET_REQUESTED;
      return(1);
     }
     return(2); //wait
    }
    else
    {
     SetStatus(PLAYER_STATUS_SHOW_CARDS);
     goto finish;
    }
   }
  }break;
 }

 if(bet>=Credit)
 {
  bet=Credit;

  Log(GetNickName() + " ALL IN");
  SetStatus(PLAYER_STATUS_ALL_IN);
 }
 else if(ActualBet+bet<minimum)
 SetStatus(PLAYER_STATUS_FOLDED);

 ActualBet+=bet;
 TotalBet+=bet;
 Credit-=bet;

finish:
 ExternalBet=EXTERNALBET_INIT;
 return(0);
}
//------------------------------------------------------------

unsigned long long TPlayer:: GetResult(void)
{
 if(Result==0)
 {
  Hand->Check(this);
  Result=Hand->GetResult();
 }
 return(Result);
}

//------------------------------------------------------------
