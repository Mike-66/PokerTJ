#include "Game.h"

int SetNextPlayerRounds;

/*TODO:
 * Netzwerkspiel
  * Ki
  */

TGame::TGame()
{
 Init(0,0,0,2,0,"","");
 isLocalGame=0;
}
//------------------------------------------------------------
TGame::TGame(int CountPlayers_Default, int StrengthOfBots, int Credit, int BigBlind, int RaiseBlindMinutes, QString Name, QString Key)
{
 Init(CountPlayers_Default,StrengthOfBots,Credit,BigBlind,RaiseBlindMinutes,Name,Key);
 isLocalGame=1;
}
//------------------------------------------------------------
TGame::~TGame()
{
 for(int i=0;i<MAXPLAYERS;i++)
 if(Player[i]!=NULL)
 delete(Player[i]);
 delete(Board);
 delete(Deck);
 Spotters.clear();
}
//------------------------------------------------------------
int TGame::Init(int CountPlayers_Default, int StrengthOfBots, int Credit, int BigBlind, int RaiseBlindMinutes, QString Name, QString Key)
{
 this->Name=Name;
 this->Key=Key;
 this->CountPlayers_Default=CountPlayers_Default;
 for(int i=0;i<MAXPLAYERS;i++)
 Player[i]=NULL;

 CountPlayers=0;
 this->CountPlayers_Default=CountPlayers_Default;

 this->StrengthOfBots=StrengthOfBots;
 this->Credit=Credit;

 Blinds_Array[0]=2;
 Blinds_Array[1]=4;
 Blinds_Array[2]=10;
 Blinds_Array[3]=20;
 Blinds_Array[4]=50;
 Blinds_Array[5]=100;
 Blinds_Array[6]=200;
 Blinds_Array[7]=500;
 Blinds_Array[8]=1000;
 Blinds_Array[9]=2000;

 ExternalBet_UserKey="";
 iWindowPlayer=-1;

 Deck=new TDeck();
 Board=new TBoard();

 Round=0;
 Stage=GS_INIT;
 //Stage=GS_PAUSE;
 Stage=GS_WAITFORPLAYERS;

 StageOld=Stage;

 iDealer=-1;
 iPlayer=0;
 Blind[1]=BigBlind;
 Blind[0]=Blind[1]/2;
 this->RaiseBlindMinutes=RaiseBlindMinutes;

 Pot=0;
 iBetRoundFinishPlayer=-2;
 BlindsRequested=0;
 HighestBet=0;
 iLastRaisePlayer=-1;

 BankWin=0;

 Continue=0;
 RunMode=0; // Continuous;
 RunMode=1; //click to continue;

 CountMessages=0;
 for(int i=0;i<GAME_MAXMESSAGES;i++)
 Message[i]="";

 StageName[BET_FOR_PREFLOP]="Preflop";
 StageName[BET_FOR_FLOP]="Flop";
 StageName[BET_FOR_TURN]="Turn";
 StageName[BET_FOR_RIVER]="River";
 StageName[BET_FOR_SHOWDOWN]="Gentlemen, trousers down please";

 DisplayStageName="";

 RoundCount=0;

 Status_No_Mask=PLAYER_STATUS_NO_CREDIT;
 Status_Bet_Inactive_Player=PLAYER_STATUS_NO_CREDIT | PLAYER_STATUS_ALL_IN | PLAYER_STATUS_FOLDED;
 Status_Showdown_Inactive_Player=PLAYER_STATUS_NO_CREDIT | PLAYER_STATUS_FOLDED;

 iSendDataTo=GAME_DONTSENDDATA;
 iSendDataTo_EXTERN=GAME_DONTSENDDATA;

//EXTERNAL CONFIG ++
 ContinueAllFolded=0;
 PauseBetweenRounds=10;
//EXTERNAL CONFIG --

 return(0);
}
//--------------------------------------------
QJsonObject TGame::AsJsonObject(void)
{
    QJsonObject O;
    QJsonObject o;
    QJsonArray  a;
    O.insert("Name",Name);
    O.insert("Key",Key);
    O.insert("CountPlayers_Default",CountPlayers_Default);
    O.insert("CountPlayers",CountPlayers);
    O.insert("StrengthOfBots",StrengthOfBots);
    O.insert("Credit",Credit);
    O.insert("BigBlind",Blind[1]);
    O.insert("RaiseBlindMinutes",RaiseBlindMinutes);
    O.insert("Round",Round);
    O.insert("Stage",Stage);
    O.insert("DisplayStageName",DisplayStageName);
    O.insert("iDealer",iDealer);
    O.insert("iPlayer",iPlayer);
    O.insert("Pot",Pot);
    O.insert("iBetRoundFinishPlayer",iBetRoundFinishPlayer);
    O.insert("BlindsRequested",BlindsRequested);
    O.insert("HighestBet",HighestBet);
    O.insert("RaiseBet",RaiseBet);
    O.insert("CallBet",CallBet);
    O.insert("BankWin",BankWin);
    O.insert("Continue",Continue);
    O.insert("RunMode",RunMode);
    O.insert("CountMessages",CountMessages);
    for(int i=0;i<CountMessages;i++)
    {
        o.insert("Message",Message[i]);
        a.append(o);
    }
    O.insert("Messages",a);

    return(O);
}

//------------------------------------------------------------

void TGame::FromJsonObject(QJsonObject* O)
{
    QJsonArray  a;
    QJsonObject o;
    Name=O->value("Name").toString();
    Key=O->value("Key").toString();
    CountPlayers_Default=O->value("CountPlayers_Default").toInt();
    CountPlayers=O->value("CountPlayers").toInt();
    StrengthOfBots=O->value("StrengthOfBots").toInt();
    Credit=O->value("Credit").toInt();
    Blind[1]=O->value("BigBlind").toInt();
    Blind[0]=Blind[1]/2;
    RaiseBlindMinutes=O->value("RaiseBlindMinutes").toInt();
    Round=O->value("Round").toInt();
    Stage=O->value("Stage").toInt();
    DisplayStageName=O->value("DisplayStageName").toString();
    iDealer=O->value("iDealer").toInt();
    iPlayer=O->value("iPlayer").toInt();
    Pot=O->value("Pot").toInt();
    iBetRoundFinishPlayer=O->value("iBetRoundFinishPlayer").toInt();
    BlindsRequested=O->value("BlindsRequested").toInt();
    HighestBet=O->value("HighestBet").toInt();
    RaiseBet=O->value("RaiseBet").toInt();
    CallBet=O->value("CallBet").toInt();
    BankWin=O->value("BankWin").toInt();
    Continue=O->value("Continue").toInt();
    RunMode=O->value("RunMode").toInt();
    CountMessages=O->value("CountMessages").toInt();
    a = O[0]["Messages"].toArray();
    for(int i = 0; i < a.size(); i++)
    {
     o=a[i].toObject();
     Message[i]=o.value("Message").toString();
    }
}

//------------------------------------------------------------

/*
void TGame::DeckFromJsonObject(QJsonObject *O)
{
    if(Board==NULL)
    Board=new TBoard();
    Board->FromJsonObject(O);
}
*/

//------------------------------------------------------------

void TGame::BoardFromJsonObject(QJsonObject *O)
{
    if(Board==NULL)
    Board=new TBoard();
    Board->FromJsonObject(O);
}

//------------------------------------------------------------

void TGame::PrepareRefresh(void)
{
 //Mark Players as not refreshed
 iWindowPlayer=-1;
 for(int i=0;i<MAXPLAYERS;i++)
 if(Player[i]!=NULL)
 Player[i]->refreshed=0;
}

//------------------------------------------------------------

void TGame::CheckRefresh(void)
{
 //delete if not refreshed
 for(int i=0;i<MAXPLAYERS;i++)
 if(Player[i]!=NULL)
 if(Player[i]->refreshed==0)
 {
  RemovePlayer(i);
 }
}

//------------------------------------------------------------

int TGame::PlayerFromJsonObject(QJsonObject *O)
{
 int idx=O->value("Index").toInt();
 if(idx>=0 && idx<MAXPLAYERS)
 {
  if(Player[idx]==NULL)
  {
   Log("TGame::PlayerFromJsonObject create Player #" +QString().setNum(idx));
   Player[idx]=new TPlayer(Deck,Board);
  }
  Player[idx]->FromJsonObject(O);
  Player[idx]->refreshed=1;
 }

 return(idx);
}

//------------------------------------------------------------
void TGame::AddMessage(QString msg)
{
 if(CountMessages>=GAME_MAXMESSAGES)
 {
  Message[CountMessages-1]="Message Overrun !!!";
  return;
 }
 Message[CountMessages]=msg;
 CountMessages++;

 //WSSTODO::Inform Players
}
//------------------------------------------------------------
void TGame::AddSpotter(TUser* User)
{
 int found=0;
 for(int i=0;i<Spotters.count();i++)
 if(Spotters[i]==User)
 {
  Log("Spotter already in Game "+Key);
  found=1;
  break;
 }
 if(found==0)
 {
  Log("Adding Spotter to Game "+Key);
  Spotters << User;
 }
}
//------------------------------------------------------------
int TGame::AddPlayer(int index, TUser *User, int Credit)
{
 if(CountPlayers<MAXPLAYERS && index>=0 && index<MAXPLAYERS && Player[index]==NULL)
 {
  //Morph the Spotter to Player
  for(int i=0;i<Spotters.count();i++)
  if(Spotters[i]==User)
  Spotters.removeAll(User);

  //++DEBUGG++
  //if(User->NickName=="+Jason+")
  //Credit=10+index*10;
  //--DEBUGG--

  Player[index] =  new TPlayer(Deck,Board,User,Credit);
  CountPlayers++;
  AddMessage("Say Hello to "+Player[index]->GetNickName());

  if( isLocalGame && User!=NULL && User->AI_Class==AI_HUMAN )
  iWindowPlayer=index;

  return(0);
 }
 return(-1);
}
//---------------------------------------------
int TGame::PerformRebuy(int index, int amount)
{
 if(index>=0 && index<MAXPLAYERS && Player[index]!=NULL)
 {
    Player[index]->Credit+=amount;
    AddMessage("Rebuy of Player "+Player[index]->GetNickName()+": "+QString().setNum(amount));
    return(-1);
 }
 return(-1);

}
//---------------------------------------------

void TGame::FillWithBots(void)
{
 int freaseats;
 int seattotake;
 int rounds=0;

 do
 {
    rounds++;
    freaseats=0;
    seattotake=-1;
    for(int i=0;i<MAXPLAYERS;i++)
    if(Player[i]==NULL)
    freaseats++;

    if(freaseats > MAXPLAYERS-CountPlayers_Default )
    {
        //++DEBUGG++
        int random=RG->bounded(0,freaseats);
        //int random=0;
        //--DEBUGG--
        int strength=StrengthOfBots;
        TUser *bot;
        QString Name="bot_";
        QString NickName="Nick_";
        QString H;

        int idx=0;
        for(int i=0;i<MAXPLAYERS;i++)
        if(Player[i]==NULL)
        {
         if(random==idx)
         {
          H.setNum(i);
          Name.append(H);
          NickName.append(H);
          bot=CreateUserObject( Name, NickName, "No_Type", strength, "", "",NULL);
          //++DEBUGG++
          AddPlayer(i,bot,Credit);
          //if(i<5)
          //AddPlayer(i,bot,10);
          //else
          //AddPlayer(i,bot,10+i*10);
          //--DEBUGG--
         }

         idx++;
        }
    }
    else
    {
        break;
    }

 }while(rounds<MAXPLAYERS);
}
//------------------------------------------------------------
int TGame::GetCountBots(void)
{
 int countbots=0;
 for(int i=0;i<MAXPLAYERS;i++)
 if(Player[i]!=NULL && Player[i]->GetAI_Class()!=AI_HUMAN)
 countbots++;
 return(countbots);
}
//------------------------------------------------------------
void TGame::RemovePlayer(TPlayer *p)
{
 if(p!=NULL)
 for(int i=0;i<MAXPLAYERS;i++)
 if(p==Player[i])
 RemovePlayer(i);
}
//------------------------------------------------------------
void TGame::RemovePlayer(int iplayer)
{
 if( CountPlayers>=1 )
 {
  if(isLocalGame)
  AddMessage(Player[iplayer]->User->NickName+" leaves table");

  if(Player[iplayer]->GetAI_Class()==AI_HUMAN)
  Spotters << Player[iplayer]->User;

  delete(Player[iplayer]);
  Player[iplayer]=NULL;
  CountPlayers--;
 }
}
//------------------------------------------------------------
int TGame::GetRandomCardIndex(void)
{
 return(RG->bounded(0,Deck->CardsAvailable));
}
//------------------------------------------------------------
int TGame::SetCardLocation(int Location)
{
 int cardnum=Deck->SetCardLocation(Location,GetRandomCardIndex());
 if(Location<0)
 Board->SetCard(cardnum);
 else
 Player[Location]->SetCard(cardnum);

 //WSSTODO::Inform Players
 return(0);
}
//------------------------------------------------------------
void TGame::GiveCardsToPlayers(void)
{
 for(int i=0;i<NUM_PLAYER_CARDS;i++)
 for(int j=0;j<MAXPLAYERS;j++)
 if(Player[j]!=NULL)
 SetCardLocation(j);
}
//------------------------------------------------------------
void TGame::GiveCardsToFlop(void)
{
 for(int i=0;i<3;i++)
 SetCardLocation(LOCATION_FLOP);
}
//------------------------------------------------------------
void TGame::GiveCardsToTurn(void)
{
 SetCardLocation(LOCATION_TURN);
}
//------------------------------------------------------------
void TGame::GiveCardsToRiver(void)
{
 SetCardLocation(LOCATION_RIVER);
}
//------------------------------------------------------------
void TGame::ClearBets(void)
{
 for(int i=0;i<MAXPLAYERS;i++)
 if(Player[i]!=NULL)
 if(Player[i]->GetStatus( PLAYER_STATUS_ALL_IN ) > 0 )
 if(Player[i]->GetStatus( PLAYER_STATUS_ALL_IN_CHECKED ) == 0 )
 {
  Player[i]->SidePot=Pot;
  for(int j=0;j<MAXPLAYERS;j++)
  if(Player[j]!=NULL)
  {
   Log(Player[j]->GetNickName() + " ClearBets Act " + QString().setNum(Player[j]->ActualBet));
   if(Player[i]->ActualBet>Player[j]->ActualBet)
   Player[i]->SidePot+=Player[j]->ActualBet;
   else
   Player[i]->SidePot+=Player[i]->ActualBet;
  }
  Player[i]->SetStatus( PLAYER_STATUS_ALL_IN_CHECKED );

  AddMessage(Player[i]->GetNickName()+" ALL IN sidepot is "+ QString().setNum(Player[i]->SidePot));
 }

 for(int i=0;i<MAXPLAYERS;i++)
 if(Player[i]!=NULL)
 {
  Pot+=Player[i]->ActualBet;
  Player[i]->ActualBet=0;
 }
 HighestBet=0;
 iBetRoundFinishPlayer=-2;

 AddMessage("Pot now "+QString().setNum(Pot)+", Bank has "+QString().setNum(BankWin));
}
//------------------------------------------------------------
void TGame::Clear(void)
{
 for(int i=0;i<MAXPLAYERS;i++)
 if(Player[i]!=NULL && Player[i]->Credit <= 0 )
 RemovePlayer(i);

 Deck->Clear();
 Board->Clear();
 for(int i=0;i<MAXPLAYERS;i++)
 if(Player[i]!=NULL)
 Player[i]->Clear();
 Pot=0;
 BlindsRequested=0;
 iLastRaisePlayer=-1;
 iShowDownInit=0;
}
//------------------------------------------------------------
void TGame::SetPlayer(int inactive_mask)
{
 iPlayer=iDealer+1;
 RotatePlayer(&iPlayer);
 if(Player[iPlayer]==NULL || Player[iPlayer]->GetStatus( inactive_mask ) > 0 )
 SetNextPlayer( inactive_mask );
}
//------------------------------------------------------------
int TGame::SetNextPlayer(int inactive_mask)
{
 SetNextPlayerRounds=0;
 SetNextPlayerRec(inactive_mask);
 if(SetNextPlayerRounds==MAXPLAYERS)
 {
  //Log("TGame::SetNextPlayer could not find any active player";
  return(-1);
 }
 //Log("IsNextPlayer" << Player[iPlayer]->Name;
 return(0);
}
//------------------------------------------------------------
void TGame::SetNextPlayerRec(int inactive_mask)
{
 int setibetroundfinishplayer=0;

 iPlayer++;
 SetNextPlayerRounds++;
 RotatePlayer(&iPlayer);

 Log("TGame::SetNextPlayerRec " +QString().setNum(SetNextPlayerRounds));

 QString N="none";
 if(iBetRoundFinishPlayer>=0)
 {
  if(Player[iBetRoundFinishPlayer]!=NULL)
  {
   N=Player[iBetRoundFinishPlayer]->GetNickName();
  }
  else
  {
   //N=-100;
  }
 }
 if(Player[iPlayer]!=NULL)
 {
  if(Player[iPlayer]->GetStatus( PLAYER_STATUS_NO_CREDIT ) > 0 )
  Log(Player[iPlayer]->GetNickName() + " PLAYER_STATUS_NO_CREDIT Ibrfp " +N);
  if(Player[iPlayer]->GetStatus( PLAYER_STATUS_FOLDED ) > 0 )
  Log(Player[iPlayer]->GetNickName() + " PLAYER_STATUS_FOLDED Ibrfp " + N);
  if(Player[iPlayer]->GetStatus( PLAYER_STATUS_ALL_IN ) > 0 )
  Log(Player[iPlayer]->GetNickName() + "PLAYER_STATUS_ALL_IN Ibrfp " + N);
 }

 if(SetNextPlayerRounds<MAXPLAYERS)
 if( Player[iPlayer]==NULL || Player[iPlayer]->GetStatus( inactive_mask ) > 0 )
 {
  if( iBetRoundFinishPlayer == iPlayer )
  setibetroundfinishplayer=1;
  SetNextPlayerRec(inactive_mask);
  if(setibetroundfinishplayer==1)
  {
   SetBetRoundFinishPlayer();
   Log("Ibrfp moved to " + Player[iPlayer]->GetNickName());
  }
 }
}
//------------------------------------------------------------
void TGame::RotatePlayer(int *iplayer)
{
 if(*iplayer>=MAXPLAYERS)
 *iplayer=0;
 if(*iplayer<0)
 *iplayer=MAXPLAYERS-1;
}
//------------------------------------------------------------
void TGame::SetBetRoundFinishPlayer(void)
{
 iBetRoundFinishPlayer=iPlayer;
 RotatePlayer(&iBetRoundFinishPlayer);

 Log("*****************************************BetRoundFinishPlayer " + Player[iPlayer]->GetNickName());
}
//------------------------------------------------------------
void TGame::SetNextDealer(void)
{
 if(iDealer==-1)
 {
  iDealer=RG->bounded(0,CountPlayers);
  //iDealer=2;
  int itmp=0;
  for(int i=0;i<MAXPLAYERS;i++)
  if(Player[i]!=NULL)
  {
   if(itmp==iDealer)
   iDealer=i;
   itmp++;
  }
 }
 else
 {
  iDealer++;
  RotatePlayer(&iDealer);
  if(Player[iDealer]==NULL || Player[iDealer]->GetStatus(Status_No_Mask)>0)
  SetNextDealer();
 }
 SetPlayer(Status_No_Mask);
}
//------------------------------------------------------------
void TGame::RaiseBlinds(void)
{
 int idx=0;
 for(int i=0;i<BLINDS_ARRAY_MAX-1;i++)
 {
  idx++;
  if(Blind[1]<=Blinds_Array[i])
  break;
 }
 if(idx<BLINDS_ARRAY_MAX)
 {
  Blind[1]=Blinds_Array[idx];
  Blind[0]=Blind[1]/2;
 }
}
//------------------------------------------------------------
void TGame::InitRound(void)
{
 SetNextDealer();
 RoundCount++;
 AddMessage("Start Round "+QString().setNum(RoundCount)+":");
 if(RaiseBlindMinutes>0)
 {
  if(!RaiseBlindsTimer.isValid())
  {
   RaiseBlindsTimer.start();
   AddMessage("Raise Blinds every "+QString().setNum(RaiseBlindMinutes)+" minutes");
  }
  else
  {
   if(RaiseBlindsTimer.elapsed()/60000>=RaiseBlindMinutes)
   {
    RaiseBlinds();
    AddMessage("Blinds Raised to Small/Big "+QString().setNum(Blind[0])+"/"+QString().setNum(Blind[1]));
    RaiseBlindsTimer.restart();
   }
  }
 }
}
//------------------------------------------------------------
int TGame::GetPlayerPosition(int iplayer)
{
 //first player after dealer is position 0
 /*
 int pos=iplayer-iDealer-1;
 if(pos<0)
 pos=CountPlayers+pos;
 return(pos);
 */
 return(0);
}
//------------------------------------------------------------
int TGame::Bet(int whatfor)
{
 QString msg;
 int ret;

 switch(whatfor)
 {
  case BET_FOR_BLINDS:
  {
   if(BlindsRequested<COUNT_BLINDS)
   {
    Player[iPlayer]->Bet(whatfor,GetPlayerPosition(iPlayer),Blind[COUNT_BLINDS-1], Blind[BlindsRequested]);
    //WSSTODO::Inform Players
    if(Player[iPlayer]->ActualBet>HighestBet)
    HighestBet=Player[iPlayer]->ActualBet;

    QString msg=Player[iPlayer]->GetNickName();
    if(BlindsRequested==0)
    msg+=" sets Small Blind ";
    else
    msg+=" sets Big Blind ";
    msg+=QString().setNum(Player[iPlayer]->ActualBet);
    if( Player[iPlayer]->GetStatus( PLAYER_STATUS_NO_CREDIT ) > 0)
    msg+=" NO CREDIT";
    else
    msg+=" Credits";
    AddMessage(msg);

    SetBetRoundFinishPlayer();
    if(SetNextPlayer(Status_Bet_Inactive_Player)!=0)
    return(0);
    if(++BlindsRequested<COUNT_BLINDS)
    return(1);
   }
  }break;
  case BET_FOR_PREFLOP:
  case BET_FOR_FLOP:
  case BET_FOR_TURN:
  case BET_FOR_RIVER:
  {
   if( iBetRoundFinishPlayer!=iPlayer )
   {
    if ( Player[iPlayer]==NULL || Player[iPlayer]->GetStatus( Status_Bet_Inactive_Player) != 0)
    goto nextplayer1;

    ret=Player[iPlayer]->Bet(whatfor,GetPlayerPosition(iPlayer),Blind[COUNT_BLINDS-1],HighestBet);
    if( ret > 0)
    {
     //amount to bring for call
     CallBet=HighestBet-Player[iPlayer]->ActualBet;
     //minimum amount for raise
     RaiseBet=CallBet+Blind[COUNT_BLINDS-1];
     //waiting for external bet on mainwin or ws

     //ret=1: Inform the player
     //ret=2: Wait
     if(ret==1)
     iSendDataTo=GAME_SENDDATAUSER+iPlayer;
     if(ret==2)
     iSendDataTo=GAME_DONTSENDDATA;

     return(ret);
    }
    //messages***************************************************************
    msg=Player[iPlayer]->GetNickName();

    if(Player[iPlayer]->GetStatus(PLAYER_STATUS_FOLDED) != 0)
    {
     msg+=" folded";
    }
    else if (Player[iPlayer]->GetStatus(PLAYER_STATUS_ALL_IN) != 0)
    {
     if(Player[iPlayer]->ActualBet>HighestBet)
     msg+=" raised ";
     else
     msg+=" called ";

     msg+=QString().setNum(Player[iPlayer]->ActualBet)+" ALL IN";
    }
    else if(Player[iPlayer]->ActualBet>HighestBet)
    {
     msg+=" raised by "+QString().setNum(Player[iPlayer]->ActualBet-HighestBet)+" total "+QString().setNum(Player[iPlayer]->ActualBet);
    }
    else if (HighestBet>0 && Player[iPlayer]->ActualBet==HighestBet)
    {
     msg+=" called ";//+QString().setNum(Player[iPlayer]->ActualBet-HighestBet);
    }
    //else if ( ( HighestBet>0 && Player[iPlayer]->ActualBet<HighestBet ) || Player[iPlayer]->GetStatus(PLAYER_STATUS_FOLDED) != 0)
    else if (HighestBet==0 || Player[iPlayer]->ActualBet==HighestBet )
    {
     msg+=" checked";
    }
    else
    {
     msg+=" unknown playeraction !!!";
    }
    AddMessage(msg);
    //messages***************************************************************

    if(Player[iPlayer]->ActualBet>HighestBet)
    {
     HighestBet=Player[iPlayer]->ActualBet;
     if ( whatfor==BET_FOR_RIVER )
     iLastRaisePlayer=iPlayer;
     BlindsRequested=0; //to not again
     SetBetRoundFinishPlayer();
    }
    else if( (BlindsRequested==COUNT_BLINDS || iBetRoundFinishPlayer==-2) && Player[iPlayer]->GetStatus( Status_Bet_Inactive_Player) == 0 )
    {
     BlindsRequested=0;
     SetBetRoundFinishPlayer();
    }

nextplayer1:
    //Status_Showdown_Inactive_Player
    if(SetNextPlayer(Status_Bet_Inactive_Player)!=0)
    {
     //Here, a Bet round is finished, all players should have placed the same bets or folded
     //we can collect the bets and add it to pot
     ClearBets();
     //indipendant for the last bet, next round starts according to dealer
     if(whatfor==BET_FOR_RIVER)
     SetPlayer(Status_Showdown_Inactive_Player);
     else
     SetPlayer(Status_Bet_Inactive_Player);

     return(0);
    }

    return(1);
   }
   else
   {
    //Here, a Bet round is finished, all players should have placed the same bets or folded
    //we can collect the bets and add it to pot
    ClearBets();
    //indipendant for the last bet, next round starts according to dealer
    if(whatfor==BET_FOR_RIVER)
    SetPlayer(Status_Showdown_Inactive_Player);
    else
    SetPlayer(Status_Bet_Inactive_Player);
   }
  }break;
  case BET_FOR_SHOWDOWN:
  {
   if(iLastRaisePlayer>=0)
   {
    if( Player[iLastRaisePlayer]!=NULL && Player[iLastRaisePlayer]->GetStatus(PLAYER_STATUS_FOLDED) == 0 )
    iPlayer=iLastRaisePlayer;
    iLastRaisePlayer=-1;
   }

   if(iBetRoundFinishPlayer==iPlayer)
   {
    Log(   "Game::Bet::BET_FOR_SHOWDOWN leaving cause of iBetRoundFinishPlayer");
    return(0);
   }

   if ( Player[iPlayer]==NULL || Player[iPlayer]->GetStatus( PLAYER_STATUS_FOLDED ) != 0)
   goto nextplayer2;

   ret=Player[iPlayer]->Bet(whatfor,GetPlayerPosition(iPlayer),Blind[COUNT_BLINDS-1],HighestBet);
   if( ret > 0)
   {
    CallBet=EXTERNALBET_CALLBET_FLIP_CARDS;
    RaiseBet=-1;

    //ret=1: Inform the player
    //ret=2: Wait
    if(ret==1)
    iSendDataTo=GAME_SENDDATAUSER+iPlayer;
    if(ret==2)
    iSendDataTo=GAME_DONTSENDDATA;

    return(ret);
   }

   msg=Player[iPlayer]->GetNickName();
   if(Player[iPlayer]->GetStatus(PLAYER_STATUS_FOLDED) > 0 )
   {
       msg+=" is a showdown folder";
   }
   else
   {
       msg+=" yessSir, you'r welcome";
   }
   AddMessage(msg);

   if(iShowDownInit>0)
   {
    SetBetRoundFinishPlayer();
    iShowDownInit=0;
   }

nextplayer2:
   if(SetNextPlayer(Status_Showdown_Inactive_Player)!=0)
   {
    Log(   "Game::Bet::BET_FOR_SHOWDOWN leaving cause of SetNextPlayer");
    return(0);
   }

   return(1);
  }break;

  /*
  case BET_FOR_RIVER:
  {

  }break;
  case BET_FOR_WIN:
  {

  }break;
  */
 }

 return(0);
}

//---------------------------------------------------

int TGame::GetLowestSidePot(void)
{
 int sidepot=Pot;
 for(int i=0;i<MAXPLAYERS;i++)
 if( Player[i]!=NULL)
 if( Player[i]->GetStatus(PLAYER_STATUS_WINNER) > 0 )
 if( Player[i]->GetStatus(PLAYER_STATUS_ALL_IN) > 0 )
 if( Player[i]->GetStatus(PLAYER_STATUS_SIDEPOT_CHECKED) == 0)
 if( Player[i]->SidePot <= sidepot )
 {
  sidepot=Player[i]->SidePot; //Lowest sidepot of winner(s)
 }

 for(int i=0;i<MAXPLAYERS;i++)
 if( Player[i]!=NULL)
 if( Player[i]->GetStatus(PLAYER_STATUS_WINNER) > 0 )
 if( Player[i]->GetStatus(PLAYER_STATUS_ALL_IN) > 0 )
 if( Player[i]->GetStatus(PLAYER_STATUS_SIDEPOT_CHECKED) == 0)
 {
  if( Player[i]->SidePot==sidepot)
  {
   Player[i]->SetStatus(PLAYER_STATUS_SIDEPOT_CHECKED);
   Log("sidepot" + Player[i]->GetNickName() + " "+QString().setNum(sidepot));
   //In case the winner is not the only one All In
   for(int j=0;j<MAXPLAYERS;j++)
   if(Player[j]!=NULL)
   if(Player[j]->SidePot > 0)
   {
    //anderer Leute sidepot kann man bedingungslos reduzieren, denn sollten Sie noch drannkommen,
    //dann mit reduziertem Sidepot. Ist noch was übrig, macht sich das am Pot bemerkbae
    Log(" before reducing sidepot of " + Player[j]->GetNickName() +" "+QString().setNum(Player[j]->SidePot));
    if(sidepot<Player[j]->SidePot)
    Player[j]->SidePot-=sidepot;
    else
    Player[j]->SidePot=0;
    //if(Player[j]->SidePot==0)
    //{
    // Player[j]->SetStatus(PLAYER_STATUS_SIDEPOT_CHECKED);
     //Player[j]->SetStatus(PLAYER_STATUS_WINNER_CHECKED);
    //}
    Log(" after reducing sidepot of " + Player[j]->GetNickName()+" "+QString().setNum(Player[j]->SidePot));
   }
   break; //wir haben ein mal den niedrigsten sidepot von allen existenten sidpots abgezogen, also raus hier
  }
 }

 return(sidepot);
}

//---------------------------------------------------

int TGame::RoundFinish(void)
{
 QString msg;
 int bankwin=0;

 //Gewinner/Anzahl ermitteln
 unsigned long long Result=0;
 int winnercount=0;
 for(int i=0;i<MAXPLAYERS;i++)
 if(Player[i]!=NULL)
 if(Player[i]->GetStatus(PLAYER_STATUS_NO_CREDIT)==0)
 if(Player[i]->GetStatus(PLAYER_STATUS_FOLDED)==0)
 if(Player[i]->GetStatus(PLAYER_STATUS_WINNER_CHECKED)==0)
 {
  if( Player[i]->GetResult() >= Result )
  Result=Player[i]->GetResult();
 }

 for(int i=0;i<MAXPLAYERS;i++)
 if(Player[i]!=NULL)
 if(Player[i]->GetStatus(PLAYER_STATUS_NO_CREDIT)==0)
 if(Player[i]->GetStatus(PLAYER_STATUS_FOLDED)==0)
 if(Player[i]->GetStatus(PLAYER_STATUS_WINNER_CHECKED)==0)
 {
  if( Player[i]->GetResult() == Result )
  {
   winnercount++;
   Player[i]->SetStatus(PLAYER_STATUS_WINNER);

   msg="Winner: "+Player[i]->GetNickName();
   if(Player[i]->GetStatus(PLAYER_STATUS_SHOW_CARDS)>0)
   msg+=" "+Player[i]->Hand->ExplainResult(Result);

   AddMessage(msg);
  }
 }


 //Falls der/die Gewinner All In sind, den niedrigsten All In bestimmen und verteilen
 //Gibt es keinen SidePot, ist sidepot der Pot
 int sidepot=GetLowestSidePot();

 if(sidepot<Pot)
 {
  msg="we have a sidepot of "+QString().setNum(sidepot);
  AddMessage(msg);
 }

 //Pot vor dem teilen verkleinern, Modulo geht an die Bank
 int potpart=0;
 if(winnercount>0)
 {
  potpart=sidepot/winnercount;
  bankwin=sidepot % winnercount;
  BankWin+=bankwin;
 }
 else
 {
  Log("no winner found");
  BankWin+=sidepot;
 }

 if(winnercount>1)
 {
  msg="every winner gets "+QString().setNum(potpart);
  if(bankwin>0)
  msg+=", Bank earns "+QString().setNum(bankwin);
  AddMessage(msg);
 }

 Pot-=sidepot;

 //Pot auf den/die Gewinner verteilen
 for(int i=0;i<MAXPLAYERS;i++)
 if(Player[i]!=NULL)
 if( Player[i]->GetStatus(PLAYER_STATUS_WINNER) > 0)
 if( Player[i]->GetStatus(PLAYER_STATUS_WINNER_CHECKED) == 0)
 {
  msg=Player[i]->GetNickName()+" wins "+QString().setNum(potpart);
  AddMessage(msg);
  Player[i]->Credit+=potpart;

  Log(msg+", now "+QString().setNum(Player[i]->Credit));

  /*
  if(DbQuery!=NULL)
  DbQuery->AddToModelOnce(RoundCount,i,Player[i]->GetResult());
  */

  //WSSTODO::Inform the Player
  if( (Player[i]->GetStatus(PLAYER_STATUS_ALL_IN) >  0 && Player[i]->GetStatus(PLAYER_STATUS_SIDEPOT_CHECKED) >  0) )
  Player[i]->SetStatus(PLAYER_STATUS_WINNER_CHECKED);
 }

 //Nochmal kucken
 if(Pot>0)
 {
  msg="we still have a pot of "+QString().setNum(Pot);
  AddMessage(msg);
  RoundFinish();
  return(0);
 }

 return(0);
}

//---------------------------------------------------

void TGame::CheckPlayers(void)
{
    int totalcredit=0;

    for(int i=0;i<MAXPLAYERS;i++)
    if(Player[i]!=NULL)
    totalcredit+=Player[i]->Credit;
    Log("totalcredit " + QString().setNum(totalcredit));
    if(BankWin>0)
    Log("Bankwin " +QString().setNum(BankWin));

    AddMessage("Totalcredit "+QString().setNum(totalcredit)+", Bank has "+QString().setNum(BankWin));
}

//---------------------------------------------------

void TGame::SetKownCardsForDebugging(void)
{
    /*
    Player[0]->iCard[0]=Deck->GetCardIndex(diamonds,ace);
    Player[0]->iCard[1]=Deck->GetCardIndex(clubs,ace);

    Player[1]->iCard[0]=Deck->GetCardIndex(clubs,two);
    Player[1]->iCard[1]=Deck->GetCardIndex(clubs,three);

    Player[2]->iCard[0]=Deck->GetCardIndex(diamonds,queen);
    Player[2]->iCard[1]=Deck->GetCardIndex(spades,queen);

    Player[3]->iCard[0]=Deck->GetCardIndex(diamonds,king);
    Player[3]->iCard[1]=Deck->GetCardIndex(clubs,king);

    Player[4]->iCard[0]=Deck->GetCardIndex(diamonds,ten);
    Player[4]->iCard[1]=Deck->GetCardIndex(clubs,ten);

    Board->iCard[0]=Deck->GetCardIndex(clubs,four);
    Board->iCard[1]=Deck->GetCardIndex(clubs,five);
    Board->iCard[2]=Deck->GetCardIndex(clubs,six);
    Board->iCard[3]=Deck->GetCardIndex(spades,seven);
    Board->iCard[4]=Deck->GetCardIndex(spades,nine);
*/

    /*
    Player[0]->iCard[0]=Deck->GetCardIndex(spades,ten);
    Player[0]->iCard[1]=Deck->GetCardIndex(clubs,four);

    Player[1]->iCard[0]=Deck->GetCardIndex(diamonds,ace);
    Player[1]->iCard[1]=Deck->GetCardIndex(diamonds,nine);

    Board->iCard[0]=Deck->GetCardIndex(spades,jack);
    Board->iCard[1]=Deck->GetCardIndex(clubs,king);
    Board->iCard[2]=Deck->GetCardIndex(hearts,king);
    Board->iCard[3]=Deck->GetCardIndex(diamonds,jack);
    Board->iCard[4]=Deck->GetCardIndex(hearts,nine);
    */

    /*
    Player[0]->iCard[0]=Deck->GetCardIndex(clubs,ace);
    Player[0]->iCard[1]=Deck->GetCardIndex(clubs,ten);

    Player[1]->iCard[0]=Deck->GetCardIndex(hearts,ace);
    Player[1]->iCard[1]=Deck->GetCardIndex(hearts,ten);

    Player[2]->iCard[0]=Deck->GetCardIndex(diamonds,king);
    Player[2]->iCard[1]=Deck->GetCardIndex(hearts,king);

    Player[3]->iCard[0]=Deck->GetCardIndex(spades,queen);
    Player[3]->iCard[1]=Deck->GetCardIndex(diamonds,queen);

    Player[4]->iCard[0]=Deck->GetCardIndex(spades,jack);
    Player[4]->iCard[1]=Deck->GetCardIndex(diamonds,jack);

    Board->iCard[0]=Deck->GetCardIndex(diamonds,two);
    Board->iCard[1]=Deck->GetCardIndex(clubs,king);
    Board->iCard[2]=Deck->GetCardIndex(clubs,queen);
    Board->iCard[3]=Deck->GetCardIndex(clubs,jack);
    Board->iCard[4]=Deck->GetCardIndex(hearts,nine);
    */
}

//------------------------------------------------------------

int TGame::CheckActivePlayersJump(int stagefrom, int stageto)
{
 int CountActivePlayers=0;
 int CountAllInPlayers=0;
 for(int i=0;i<MAXPLAYERS;i++)
 if( Player[i]!=NULL &&
     Player[i]->GetStatus(PLAYER_STATUS_FOLDED)==0 )
 CountActivePlayers++;
 if(CountActivePlayers>1)
 return(stagefrom); //continue normally

 /*
 for(int i=0;i<MAXPLAYERS;i++)
 if( Player[i]!=NULL &&
     Player[i]->GetStatus(PLAYER_STATUS_ALL_IN)==0 )
 CountAllInPlayers++;
 if(CountAllInPlayers>1)
 return(stageto); //according to nextstep, continue dealing cards and ask for flip/fold
 */

 if(ContinueAllFolded==1)
 return(stageto);   //according to nextstep, continue dealing cards and ask for flip/fold
 else if(ContinueAllFolded==2)
 return(GS_BET_FOR_SHOWDOWN); //do not continue dealing cards and ask for flip/fold

 return(GS_ROUND_FINISH); //do not continue dealing cards and do not ask for flip/fold
}

//---------------------------------------------------

void TGame::NextStep()
{
 /*******************************************************************/
 StageOld=Stage;
 Round++;

 iSendDataTo=GAME_SENDDATA;

 switch(Stage)
 {
  case GS_WAITFORPLAYERS:
  {
   if(RunMode>0 && Continue==0)
   {
    iSendDataTo=GAME_DONTSENDDATA;
    break;
   }
   FillWithBots();
   Continue=0;
   Stage=GS_INIT;
  }break;
  case GS_PAUSE:
  {
   //we're pausing
   //the one who claims pausing has at least to remember Stage
   iSendDataTo=GAME_DONTSENDDATA;
  }break;
  case GS_INIT:
  {
   if(RunMode>0 && Continue==0)
   {
    iSendDataTo=GAME_DONTSENDDATA;
    break;
   }

   InitRound();
   AddMessage("------Bliinds bidde---------");
   Stage++;
  }break;
  case GS_BET_FOR_BLINDS:
  {
   if(Bet(BET_FOR_BLINDS)==0)
   {
    Log("Increasing Step");
    Stage++;
   }
   else
   Log("Holding Step");
  }break;
  case GS_START:
  {
   GiveCardsToPlayers();
   AddMessage("------"+StageName[BET_FOR_PREFLOP]+"---------");
   DisplayStageName=StageName[BET_FOR_PREFLOP];
   Stage++;
  }break;
  case GS_BET_FOR_PREFLOP:
  {
   if(Bet(BET_FOR_PREFLOP)==0)
   {
    Stage=CheckActivePlayersJump(++Stage, GS_FLOP);
   }
  }break;
  case GS_FLOP:
  {
   GiveCardsToFlop();
   AddMessage("------"+StageName[BET_FOR_FLOP]+"---------");
   DisplayStageName=StageName[BET_FOR_FLOP];
   Stage=CheckActivePlayersJump(++Stage, GS_TURN);
  }break;
  case GS_BET_FOR_FLOP:
  {
   if(Bet(BET_FOR_FLOP)==0)
   {
    Stage=CheckActivePlayersJump(++Stage, GS_TURN);
   }
  }break;
  case GS_TURN:
  {
   GiveCardsToTurn();
   AddMessage("------"+StageName[BET_FOR_TURN]+"---------");
   DisplayStageName=StageName[BET_FOR_TURN];
   Stage=CheckActivePlayersJump(++Stage, GS_RIVER);
  }break;
  case GS_BET_FOR_TURN:
  {
   if(Bet(BET_FOR_TURN)==0)
   {
    Stage=CheckActivePlayersJump(++Stage, GS_RIVER);
   }
  }break;
  case GS_RIVER:
  {
   GiveCardsToRiver();
   AddMessage("------"+StageName[BET_FOR_RIVER]+"---------");
   DisplayStageName=StageName[BET_FOR_RIVER];
   Stage=CheckActivePlayersJump(++Stage, GS_BET_FOR_SHOWDOWN);
   iShowDownInit=1;
  }break;
  case GS_BET_FOR_RIVER:
  {
   if(Bet(BET_FOR_RIVER)==0)
   {
    Stage=CheckActivePlayersJump(++Stage, GS_BET_FOR_SHOWDOWN);
    iShowDownInit=1;
   }
  }break;
  case GS_BET_FOR_SHOWDOWN:
  {
   if(iShowDownInit==1)
   {
    iShowDownInit=2;
    AddMessage("---"+StageName[BET_FOR_SHOWDOWN]+"---");
    DisplayStageName="";
   }
   if(Bet(BET_FOR_SHOWDOWN)==0)
   {
    Stage++;
   }
  }break;
  case GS_ROUND_FINISH:
  {
   SetKownCardsForDebugging();
   RoundFinish();
   Stage++;
  }break;
  case GS_CHECK_PLAYERS:
  {
   CheckPlayers();
   Continue=-PauseBetweenRounds;
   Continue_Old=0;
   PauseTimer.start();
   Stage++;
  }break;
  case GS_CLEAR:
  {
   if(RunMode>0)
   {
    if(Continue==0)
    {
     iSendDataTo=GAME_DONTSENDDATA;
     break;
    }
    else if(Continue<0)
    {
     Continue= (PauseTimer.elapsed()/1000) - PauseBetweenRounds;
     if(Continue_Old==Continue)
     iSendDataTo=GAME_DONTSENDDATA;
     else
     Continue_Old=Continue;

     if(Continue<0)
     break;
     else
     Continue=1;
    }
   }
   Clear();
   /**/
   if(CountPlayers>1)
   Stage=GS_INIT;
   else
   Stage=GS_WAITFORPLAYERS;
   /**/
   /*
   if(CountPlayers==10)
   Stage=GS_INIT;
   else
   Stage=GS_WAITFORPLAYERS;
   */
   //Stage=GS_FINISH;
  }break;
  case GS_FINISH:
  {
   for(int i=0;i<MAXPLAYERS;i++)
   if(Player[i]!=NULL)
   {
    Log("And The Winner is " + Player[i]->GetNickName());
    break;
   }
   iSendDataTo=GAME_DONTSENDDATA;
  }break;
 }

 //return(StageOld);
}

//--------------------------------------------------

void TGame::SlotDataReceived(QString data)
{
 Log("Game data" + data);
}



