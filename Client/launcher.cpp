#include "launcher.hpp"

//---------------------------------------------
Launcher::Launcher()
{
 stage=STAGE_LD_ENTERING;

 MeMySelf=CreateUserObject( "Thomas", "-Jason-", "No_Type", AI_HUMAN, "", "",NULL);

 w=NULL;
 g=NULL;
 WSS_Client=NULL;
 ld=new LauncherDialog;

 ServerPrefix="wss://";
 ServerRoute="/wss";

 IniFileName=GetConfigLocation("","pokertj.cfg");
 LoadSettings();

 /*The User*/
 //UUserKey="";
 //UUserType="";
 //UGameKey="";
 /*Until here*/

 WSS_CheckTimeout=100;
 WSS_CheckTimeoutIs=WSS_CheckTimeout;

 WSS_ServerTimeout=50;
 WSS_ServerTimeoutIs=0;

 EnterGameTimeout=50;
 EnterGameTimeoutIs=0;

 doExit=0;

 MessageId=0;

 MessageBoxVisible=0;
}

//---------------------------------------------

Launcher::~Launcher()
{
    if(ld!=NULL)
    delete(ld);
    if(WSS_Client!=NULL)
    delete(WSS_Client);
}

//---------------------------------------------

void Launcher::LoadSettings(void)
{
    QSettings settings(IniFileName,QSettings::IniFormat);

    if(ld!=NULL && settings.contains("ConnectOptions/ConnectType"))
    {
     ld->ConnectType=settings.value("ConnectOptions/ConnectType").toInt();
     ld->Server=settings.value("ConnectOptions/Server").toString();
     ld->UserName=settings.value("ConnectOptions/UserName").toString();
     ld->Password=settings.value("ConnectOptions/Password").toString();
     ld->NickName=settings.value("ConnectOptions/NickName").toString();

     ld->CreateGameName=settings.value("GameOptions/GameName").toString();
     ld->NumberofPlayers=settings.value("GameOptions/CountPlayers").toInt();
     ld->Credit=settings.value("GameOptions/Credit").toInt();
     ld->StrenghOfBots=settings.value("GameOptions/StrengthofBots").toInt();
     ld->BigBlind=settings.value("GameOptions/BigBlind").toInt();
     ld->RaiseBlindMinutes=settings.value("GameOptions/RaiseBlindMinutes").toInt();

     ld->Init();
    }

    if(w!=NULL && settings.contains("ActionKeys/ActionByMouse"))
    {
     w->ActionByMouse=settings.value("ActionKeys/ActionByMouse").toInt();
     w->ShowMouseBinds=settings.value("ActionKeys/ShowMouseBinds").toInt();
     w->ActionByKey=settings.value("ActionKeys/ActionByKey").toInt();
     w->ShowKeyBinds=settings.value("ActionKeys/ShowKeyBinds").toInt();
     w->Key_Start=settings.value("ActionKeys/Key_Start").toInt();
     w->Key_Raise=settings.value("ActionKeys/Key_Raise").toInt();
     w->Key_Check=settings.value("ActionKeys/Key_Check").toInt();
     w->Key_Call=settings.value("ActionKeys/Key_Call").toInt();
     w->Key_Fold=settings.value("ActionKeys/Key_Fold").toInt();
     w->Key_IncBet=settings.value("ActionKeys/Key_IncBet").toInt();
     w->Key_DecBet=settings.value("ActionKeys/Key_DecBet").toInt();
    }

    if(w!=NULL && settings.contains("Sound/Volume_PlayersTurn"))
    {
     w->EffectVolume[MYSOUND_PLAYERSTURN]=settings.value("Sound/Volume_PlayersTurn").toFloat();
     w->EffectLoops[MYSOUND_PLAYERSTURN]=settings.value("Sound/EffectLoops_PlayersTurn").toFloat();
     w->EffectVolume[MYSOUND_ACTIONWARNING]=settings.value("Sound/Volume_ActionWarning").toFloat();
     w->EffectLoops[MYSOUND_ACTIONWARNING]=settings.value("Sound/EffectLoops_ActionWarning").toFloat();
    }

    if(w!=NULL && settings.contains("Misc/ActionWarningPercent"))
    {
     w->ActionWarningPercent=settings.value("Misc/ActionWarningPercent").toInt();
     w->EffectLoops[MYSOUND_PLAYERSTURN]=settings.value("Sound/EffectLoops_PlayersTurn").toFloat();
     w->EffectVolume[MYSOUND_ACTIONWARNING]=settings.value("Sound/Volume_ActionWarning").toFloat();
     w->EffectLoops[MYSOUND_ACTIONWARNING]=settings.value("Sound/EffectLoops_ActionWarning").toFloat();
    }

}

//---------------------------------------------

void Launcher::SaveSettings(void)
{
    QSettings settings(IniFileName,QSettings::IniFormat);

    if(ld!=NULL)
    {
     settings.setValue("ConnectOptions/ConnectType",ld->ConnectType);
     settings.setValue("ConnectOptions/Server",ld->Server);
     settings.setValue("ConnectOptions/UserName",ld->UserName);
     settings.setValue("ConnectOptions/Password",ld->Password);
     settings.setValue("ConnectOptions/NickName",ld->NickName);
    }

    if(ld!=NULL)
    {
     settings.setValue("GameOptions/GameName",ld->GameName);
     settings.setValue("GameOptions/CountPlayers",ld->NumberofPlayers);
     settings.setValue("GameOptions/Credit",ld->Credit);
     settings.setValue("GameOptions/StrengthofBots",ld->StrenghOfBots);
     settings.setValue("GameOptions/BigBlind",ld->BigBlind);
     settings.setValue("GameOptions/RaiseBlindMinutes",ld->RaiseBlindMinutes);
    }

    if(w!=NULL)
    {
     settings.setValue("ActionKeys/ActionByMouse",w->ActionByMouse);
     settings.setValue("ActionKeys/ShowMouseBinds",w->ShowMouseBinds);
     settings.setValue("ActionKeys/ActionByKey",w->ActionByKey);
     settings.setValue("ActionKeys/ShowKeyBinds",w->ShowKeyBinds);
     settings.setValue("ActionKeys/Key_Start",w->Key_Start);
     settings.setValue("ActionKeys/Key_Raise",w->Key_Raise);
     settings.setValue("ActionKeys/Key_Check",w->Key_Check);
     settings.setValue("ActionKeys/Key_Call",w->Key_Call);
     settings.setValue("ActionKeys/Key_Fold",w->Key_Fold);
     settings.setValue("ActionKeys/Key_IncBet",w->Key_IncBet);
     settings.setValue("ActionKeys/Key_DecBet",w->Key_DecBet);

     settings.setValue("Sound/Volume_PlayersTurn",w->EffectVolume[MYSOUND_PLAYERSTURN]);
     settings.setValue("Sound/EffectLoops_PlayersTurn",w->EffectLoops[MYSOUND_PLAYERSTURN]);
     settings.setValue("Sound/Volume_ActionWarning",w->EffectVolume[MYSOUND_ACTIONWARNING]);
     settings.setValue("Sound/EffectLoops_ActionWarning",w->EffectLoops[MYSOUND_ACTIONWARNING]);

     settings.setValue("Misc/ActionWarningPercent",w->ActionWarningPercent);

    }

    settings.sync();

    qDebug() << "saved settings to " << IniFileName;
}

//---------------------------------------------

void Launcher::ShowMessageBox(QString Text)
{
 if(MessageBoxVisible>0)
 return;

 MessageBoxVisible=1;
 QMessageBox::StandardButton reply;
 reply = QMessageBox::information(NULL, "Info", Text,QMessageBox::Ok);
 MessageBoxVisible=0;
}

//---------------------------------------------

void Launcher::Work()
{
    switch(stage)
    {
     case STAGE_LD_ENTERING:
     {
      qDebug() << "laucherd";
      ld->SetMainWindow(w);
      ld->show();
      stage=STAGE_LD_VISIBLE;
     }break;
     case STAGE_LD_VISIBLE:
     {
      //here, ld is visible and we're waiting for users input
      if( ld->Result==LD_OK || ld->Action==LD_ACTION_REQUEST_GAME )
      {
       EnterGameTimeoutIs=EnterGameTimeout;
       if(ld->Result==LD_OK && ld->GameKey=="")
       {
        if(ld->ConnectType==LD_CONNECT_TYPE_ONLINE)
        RequestPlayerStateUnsol();
        EnterGameTimeoutIs=1;
       }

       stage=STAGE_GAME_ENTERING;
       StartGame(ld->ConnectType);
       ld->hide();
      }
      else if (ld->Result==LD_CANCEL)
      {
       doExit=1;
      }

      switch(ld->Action)
      {
       case LD_ACTION_CREATE_GAME:
       {
        if(ld->CreateGameName!="")
        {
         if(ld->ConnectType==LD_CONNECT_TYPE_ONLINE)
         RequestCreateGame();
         ld->CreateGameName="";
        }
       }break;
       case LD_ACTION_LEAVE_GAME:
       {
        RequestLeaveGame();
       }break;
       case LD_ACTION_RELOAD:
       {
        RequestAvailableGames();
       }break;
      }    //switch ld->Action
      ld->Action=LD_ACTION_INIT;
     }break;
     case STAGE_GAME_ENTERING:
     {
      if(w!=NULL)
      stage=STAGE_GAME_RUNNING;
      else
      {
       if(EnterGameTimeoutIs>0)
       if(--EnterGameTimeoutIs == 0)
       {
        qDebug() <<  "timeout entering game";
        stage=STAGE_LD_ENTERING;
       }
      }
     }break;
     case STAGE_GAME_RUNNING:
     {
      if( w==NULL )
      {
       qDebug() << "w is null";
       break;
      }

      if( w->ToggleConfigVisible )
      {
       if( !ld->isVisible() )
       {
        ld->SetMainWindow(w);
        ld->show();
       }
       else
       ld->hide();

       w->ToggleConfigVisible=0;
      }

      if( w->isVisible() )
      {
       if(WSS_Client!=NULL)
       {
        switch(w->iSendToServer)
        {
         case REQUESTPLAYERSEAT:RequestPlayerSeat();break;
         case REQUESTCONTINUE:RequestContinue();break;
         case REQUESTEXTERNALBET:RequestExternalBet();break;
         case REQUESTREBUY:RequestRebuy();break;
         case REQUESTGAMESTATS:RequestGameStats(g);
        }
        w->iSendToServer=REQUESTNONE;
       }
       else if(g->isLocalGame)
       {
        switch(w->iSendToServer)
        {
         case REQUESTPLAYERSEAT:g->AddPlayer(g->iPlayerSeatRequested,MeMySelf,ld->Credit);break;
         case REQUESTREBUY:g->PerformRebuy(g->iWindowPlayer,g->iRebuyRequested);
        }
        w->iSendToServer=REQUESTNONE;
       }
      }
      else
      {
       SaveSettings();
       w->close();
       delete(w);
       w=NULL;
       qDebug() << "cleared window";
       ld->SetMainWindow(w);

       if(g!=NULL)
       delete(g);
       g=NULL;
       qDebug() << "cleared game";

       stage=STAGE_LD_ENTERING;
      }
     }break;
    }

 if( ld->ConnectType==LD_CONNECT_TYPE_ONLINE && ld->connecttoserver )
 {
    if(WSS_Client==NULL)
    {
     Log("creating WSS_Client to "+ld->Server );
     WSS_Client=new TWSS_Client(this,ServerPrefix+ld->Server+ServerRoute);
     WSS_CheckTimeoutIs=WSS_CheckTimeout;
    }

    if(WSS_CheckTimeoutIs>0)
    if(--WSS_CheckTimeoutIs==0)
    {
        RequestKeepAlive();
        WSS_CheckTimeoutIs=WSS_CheckTimeout;
    }

    if(WSS_ServerTimeoutIs>0)
    if(--WSS_ServerTimeoutIs==0)
    {
     qDebug() << "Connection to " << ld->Server << "timed out";
     ld->SetRoundTrip(200000);
     ShowMessageBox("Connection to " + ld->Server + " timed out");
     delete(WSS_Client);
     WSS_Client=NULL;
    }
 }
 else
 {
    LogOff();
 }
}

//---------------------------------------------

void Launcher::LogOff(void)
{
    MeMySelf->Key="";
    if(WSS_Client!=NULL)
    {
     delete(WSS_Client);
     WSS_Client=NULL;
     ld->SetRoundTrip(200000);
    }
}

//---------------------------------------------

void Launcher::on_WSS_MessageReceived(QString message)
{
    qDebug() << "Launcher::on_WS_MessageReceived " << message;
}

//---------------------------------------------

void Launcher::on_WSS_BinaryMessageReceived(QByteArray message)
{
    qint64 trip=ResponseTimer.nsecsElapsed();
    //qDebug() << "Launcher::on_WSS_BinaryMessageReceived ";
    //WSS_CheckTimeoutIs=WSS_CheckTimeout;
    WSS_ServerTimeoutIs=0;

    QString MessageType;

    DebugTimer.start();

    QJsonParseError parseError;
    QJsonDocument jsonDoc;
    jsonDoc = QJsonDocument::fromJson(message, &parseError);
    if(parseError.error != QJsonParseError::NoError){
            qWarning() << "Parse error at " << parseError.offset << ":" << parseError.errorString();
            return;
    }

    QJsonObject jsonObj_in,jsonObj_out;
    jsonObj_in = jsonDoc.object();

    MessageType=jsonObj_in.value("MessageType").toString();

    if(jsonObj_in.value("ClientMsgId").toInt()==MessageIdSent)
    {
     qDebug() << "ResponseTimer" << trip;
     ld->SetRoundTrip(trip/1000000.);
    }

    QString Error=jsonObj_in.value("ServerInfo").toString();
    if( Error != "")
    {
        qDebug() << "Server claimed" << Error;
        ShowMessageBox(Error);
    }
    else if (MessageType=="Internal")
    {
        ProcessInternalMessage(&jsonObj_in, &jsonObj_out);
    }
    else if(MessageType=="Login")
    {
        ProcessLoginMessage(&jsonObj_in, &jsonObj_out);
    }
    else if(MessageType=="PlayerState")
    {
        ProcessPlayerStateMessage(&jsonObj_in, &jsonObj_out);
    }
    else if(MessageType=="KeepAlive")
    {
        ProcessKeepAliveMessage(&jsonObj_in, &jsonObj_out);
    }
    else if(MessageType=="GameState")
    {
        ProcessGameStateMessage(&jsonObj_in, &jsonObj_out);
    }
    else if(MessageType=="RequestGame")
    {
        ProcessRequestGame(&jsonObj_in, &jsonObj_out);
    }
    else if(MessageType=="PlayerSeat")
    {
        ProcessRequestPlayerSeat(&jsonObj_in, &jsonObj_out);
    }
    else if(MessageType=="CreateGame")
    {
        ProcessRequestCreateGame(&jsonObj_in, &jsonObj_out);
    }
    else if(MessageType=="AvailableGames")
    {
        ProcessAvailableGames(&jsonObj_in, &jsonObj_out);
    }
    else if(MessageType=="GameStats")
    {
        ProcessGameStats(&jsonObj_in, &jsonObj_out);
    }
    else
    {
        qDebug() << "unknown MessageType";
    }

    Log("on_WSS_BinaryMessageReceived MessageType "+MessageType+ "ProcessTime " +QString().setNum(DebugTimer.nsecsElapsed() / 1000000.)+" msecs");

    if(!jsonObj_out.isEmpty() && WSS_Client!=NULL)
    {
        SendRequest(&jsonObj_out);
    }
}

//---------------------------------------------

void Launcher::RequestKeepAlive()
{
    qDebug() << "Launcher::RequestKeepAlive";
    QJsonObject jsonObj_out;
    jsonObj_out.insert("MessageType","KeepAlive");
    SendRequest(&jsonObj_out);
}

//---------------------------------------------

void Launcher::RequestLeaveGame(void)
{
    qDebug() << "Launcher::RequestLeaveGame";
    QJsonObject jsonObj_out;
    jsonObj_out.insert("MessageType","LeaveGame");
    SendRequest(&jsonObj_out);
}

//----------------------------------------------

void Launcher::RequestRebuy(void)
{
    qDebug() << "Launcher::RequestRebuy";
    QJsonObject jsonObj_out;
    jsonObj_out.insert("MessageType","Rebuy");
    jsonObj_out.insert("GameKey",g->Key);
    jsonObj_out.insert("iRebuyRequested",g->iRebuyRequested);
    SendRequest(&jsonObj_out);
}

//----------------------------------------------

void Launcher::RequestContinue(void)
{
    qDebug() << "Launcher::RequestContinue";
    QJsonObject jsonObj_out;
    jsonObj_out.insert("MessageType","Continue");
    jsonObj_out.insert("Continue",g->Continue);
    SendRequest(&jsonObj_out);
}

//---------------------------------------------

void Launcher::RequestExternalBet(void)
{
    qDebug() << "Launcher::RequestExternalBet";
    QJsonObject jsonObj_out;
    jsonObj_out.insert("MessageType","ExternalBet");
    jsonObj_out.insert("ExternalBet",g->Player[g->iPlayer]->ExternalBet);
    SendRequest(&jsonObj_out);
}

//---------------------------------------------

void Launcher::SendRequest(QJsonObject *jsonObj)
{
    if(WSS_Client==NULL)
    {
     qDebug() << "WSS_Client not connected"    ;
     ShowMessageBox("Please login first");
     return;
    }
    jsonObj->insert("UserKey",MeMySelf->Key);
    jsonObj->insert("ClientMsgId",MessageId);
    QJsonDocument jsondoc=QJsonDocument(*jsonObj);
    WSS_Client->SendJsonDocument(&jsondoc);
    ResponseTimer.start();
    MessageIdSent=MessageId;
    MessageId++;
    WSS_ServerTimeoutIs=WSS_ServerTimeout;
}

//---------------------------------------------

void Launcher::ProcessInternalMessage(QJsonObject *jsonObj_in,QJsonObject *jsonObj_out)
{
    qDebug() << "Launcher::ProcessInternalMessage";

    QJsonValue jsonVal;
    QString Msg = jsonObj_in->value("Msg").toString();
    if(Msg=="connected")
    {
        if(MeMySelf->Key=="")
        {
            RequestLogin(jsonObj_out);
        }
        else
        {
            RequestPlayerState(jsonObj_out);
        }
    }
    else
    {
        qDebug() << "Launcher::ProcessInternalMessage unknown Msg " << Msg;
    }
}

//---------------------------------------------

void Launcher::RequestLogin(QJsonObject *jsonObj_out)
{
    jsonObj_out->empty();
    jsonObj_out->insert("MessageType","Login");
    jsonObj_out->insert("UserName",ld->UserName);
    jsonObj_out->insert("Password",ld->Password);
}

//---------------------------------------------

void Launcher::ProcessLoginMessage(QJsonObject *jsonObj_in,QJsonObject *jsonObj_out)
{
    qDebug() << "Launcher::ProcessLoginMessage";

    MeMySelf->Key=jsonObj_in->value("UserKey").toString();
    MeMySelf->Type=jsonObj_in->value("UserType").toString();

    qDebug() << "UserKey/UserType" << MeMySelf->Key << "/" << MeMySelf->Type;

    if(MeMySelf->Key!="")
    RequestPlayerState(jsonObj_out);

    /*
    QJsonArray gameArray;
    gameArray = jsonObj[0]["AvailableGames"].toArray();
    ld->ClearAvailableGames();
    for(int i = 0; i < gameArray.size(); i++)
    ld->AddAvailableGame(gameArray[i].toString());
    */
}

//---------------------------------------------

void Launcher::RequestPlayerState(QJsonObject *jsonObj_out)
{
    jsonObj_out->empty();
    jsonObj_out->insert("MessageType","PlayerState");
}

//---------------------------------------------

void Launcher::RequestPlayerStateUnsol(void)
{
    QJsonObject jsonObj_out;
    RequestPlayerState(&jsonObj_out);
    SendRequest(&jsonObj_out);
}

//---------------------------------------------

void Launcher::RequestAvailableGames(void)
{
    QJsonObject jsonObj_out;
    jsonObj_out.empty();
    jsonObj_out.insert("MessageType","AvailableGames");
    SendRequest(&jsonObj_out);
}

//---------------------------------------------

void Launcher::LoadAvailableGames(QJsonObject *jsonObj_in)
{
 QJsonArray gameArray;
 QJsonObject o;
 gameArray = jsonObj_in[0]["AvailableGames"].toArray();
 ld->ClearAvailableGames();
 for(int i = 0; i < gameArray.size(); i++)
 {
  o=gameArray[i].toObject();
  ld->AddAvailableGame(o.value("Name").toString(),
                       o.value("Players").toInt(),
                       o.value("Active Players").toInt(),
                       o.value("Spotters").toInt(),
                       o.value("Bots").toInt(),
                       o.value("Strength").toInt(),
                       o.value("Credit").toInt(),
                       o.value("Big Blind").toInt(),
                       o.value("Raise").toInt(),
                       o.value("Key").toString());
 }
 ld->ShowAvailableGames(jsonObj_in->value("GameKey").toString());
}

//---------------------------------------------

void Launcher::ProcessAvailableGames(QJsonObject *jsonObj_in,QJsonObject *jsonObj_out)
{
    qDebug() << "Launcher::ProcessAvailableGamesMessage";

    LoadAvailableGames(jsonObj_in);
}

//---------------------------------------------

void Launcher::ProcessPlayerStateMessage(QJsonObject *jsonObj_in,QJsonObject *jsonObj_out)
{
    qDebug() << "Launcher::ProcessPlayerStateMessage";

    LoadAvailableGames(jsonObj_in);

    MeMySelf->GameKey=jsonObj_in->value("GameKey").toString();
    qDebug() << "GameKey" << MeMySelf->GameKey;

    if(MeMySelf->GameKey!="")
    {
     ld->Action=LD_ACTION_REQUEST_GAME;
     ld->GameKey=MeMySelf->GameKey;
     stage=STAGE_LD_VISIBLE;
    }
}

//---------------------------------------------

int Launcher::StartGame(int online)
{
    qDebug() << "Launcher::StartGame()";
    if(online==LD_CONNECT_TYPE_LOCAL)
    {
     SaveSettings();
     MeMySelf->NickName=ld->NickName;
     g = new TGame(ld->NumberofPlayers,ld->StrenghOfBots,ld->Credit,ld->BigBlind,ld->RaiseBlindMinutes,ld->CreateGameName,"");
    }
    else if ( ld->GameKey!="" )
    {
     SaveSettings();
     RequestGame();
     ld->GameKey="";
    }

    if(g!=NULL && w==NULL && stage==STAGE_GAME_ENTERING)
    {
     w=new MainWindow(g);
     LoadSettings();
     w->show();
     w->RefreshTable();
     return(0);
    }

    return(1);
}

//---------------------------------------------

void Launcher::RequestGame()
{
    qDebug() << "Launcher::RequestGame";
    QJsonObject jsonObj_out;
    jsonObj_out.insert("MessageType","RequestGame");
    jsonObj_out.insert("GameKey",ld->GameKey);
    SendRequest(&jsonObj_out);
}

//---------------------------------------------

void Launcher::RequestGameState(QJsonObject *jsonObj_out)
{
    jsonObj_out->empty();
    jsonObj_out->insert("MessageType","GameState");
    jsonObj_out->insert("Msg","GetAll");
    jsonObj_out->insert("GameKey",MeMySelf->GameKey);
}

//---------------------------------------------

void Launcher::ProcessRequestGame(QJsonObject *jsonObj_in, QJsonObject *jsonObj_out)
{
    qDebug() << "Launcher::ProcessRequestGame";

    ProcessGameStateMessage(jsonObj_in,jsonObj_out);
}

//---------------------------------------------

void Launcher::ProcessGameStateMessage(QJsonObject *jsonObj_in,QJsonObject *jsonObj_out)
{
    QString Msg=jsonObj_in->value("Msg").toString();
    qDebug() << "Launcher::ProcessGameStateMessage Msg "<< Msg;
    QJsonObject o=jsonObj_in[0]["Game"].toObject();
    if(!o.isEmpty())
    {
     int gamecreated=0;
     if(g==NULL)
     {
      if(stage==STAGE_GAME_ENTERING)
      {
       g=new TGame;
       gamecreated=1;
      }
     }

     if(g!=NULL)
     {
      g->FromJsonObject(&o);
      if(gamecreated==1)
      qDebug() << "created game " << g->Key;

      QJsonArray Players;
      Players = jsonObj_in[0]["Players"].toArray();
      g->PrepareRefresh();
      for(int i = 0; i < Players.size(); i++)
      {
       o=Players[i].toObject();
       int idx=g->PlayerFromJsonObject(&o);
       qDebug() << "g->Player[i]->User#" << i << "idx#" << idx;
       if(idx>=0 && idx<MAXPLAYERS)
       if(g->Player[idx] != NULL &&
          g->Player[idx]->User !=NULL )
       {
        qDebug() << "g->Player[idx]->User->Key#" << idx << g->Player[idx]->User->Key;
        if( g->Player[idx]->User->Key == MeMySelf->Key)
           g->iWindowPlayer=idx;
       }
      }
      g->CheckRefresh();

      o=jsonObj_in[0]["Board"].toObject();
      g->BoardFromJsonObject(&o);
      //o=jsonObj_in[0]["Deck"].toObject();
      //g->DeckFromJsonObject(&o);

      qDebug() << "Launcher::ProcessGameStateMessage Loaded Game " << g->Key;

      if(w!=NULL)
      w->RefreshTable();
      else
      StartGame(LD_CONNECT_TYPE_ONLINE);

     } //g!=NULL

    }
}

//---------------------------------------------

void Launcher::RequestCreateGame()
{
    qDebug() << "Launcher::RequestCreateGame";
    QJsonObject jsonObj_out;
    jsonObj_out.insert("MessageType","CreateGame");
    jsonObj_out.insert("GameName",ld->CreateGameName);
    jsonObj_out.insert("CountPlayers",ld->NumberofPlayers);
    jsonObj_out.insert("Credit",ld->Credit);
    jsonObj_out.insert("StrengthofBots",ld->StrenghOfBots);
    jsonObj_out.insert("BigBlind",ld->BigBlind);
    jsonObj_out.insert("RaiseBlindMinutes",ld->RaiseBlindMinutes);
    SendRequest(&jsonObj_out);
}

//---------------------------------------------

void Launcher::ProcessRequestCreateGame(QJsonObject *jsonObj_in,QJsonObject *jsonObj_out)
{
    qDebug() << "Launcher::ProcessRequestCreateGame";
    LoadAvailableGames(jsonObj_in);
    //ProcessPlayerStateMessage(jsonObj_in,jsonObj_out);
}

//---------------------------------------------

void Launcher::RequestPlayerSeat(void)
{
    qDebug() << "Launcher::RequestPlayerSeat";
    QJsonObject jsonObj_out;
    jsonObj_out.insert("MessageType","PlayerSeat");
    jsonObj_out.insert("GameKey",g->Key);
    jsonObj_out.insert("iPlayerSeatRequested",g->iPlayerSeatRequested);
    //NickName is part of User and should be transported only in save to db to server.
    //it then will be loaded on server at login time
    jsonObj_out.insert("NickName",ld->NickName);
    SendRequest(&jsonObj_out);
}

//---------------------------------------------

void Launcher::ProcessRequestPlayerSeat(QJsonObject *jsonObj_in,QJsonObject *jsonObj_out)
{
    qDebug() << "Launcher::ProcessPlayerSeat";

    ProcessGameStateMessage(jsonObj_in,jsonObj_out);
}

//---------------------------------------------

void Launcher::ProcessRequestRebuy(QJsonObject *jsonObj_in,QJsonObject *jsonObj_out)
{
    qDebug() << "Launcher::ProcessRebuy";

    ProcessGameStateMessage(jsonObj_in,jsonObj_out);
}

//---------------------------------------------

void Launcher::RequestGameStats(TGame *Game)
{
     QJsonObject jsonObj_out;
     jsonObj_out.insert("MessageType","GameStats");
     jsonObj_out.insert("GameKey",Game->Key);
     SendRequest(&jsonObj_out);
}

//---------------------------------------------

void Launcher::ProcessGameStats(QJsonObject *jsonObj_in,QJsonObject *jsonObj_out)
{
    QJsonArray Stats;
    QJsonObject o;
    QJsonArray a;
    Stats = jsonObj_in[0]["Stats"].toArray();
    for(int i = 0; i < Stats.size(); i++)
    {
     o=Stats[i].toObject();
     int index=o.value("index").toInt();
     Log("index="+QString().setNum(index));
     Log("NickName="+o.value("NickName").toString());
     a = o["PlayerStats"].toArray();
     for(int i = 0; i < a.size(); i++)
     {
      QJsonObject c=a[i].toObject();
      index=c.value("c").toInt();
      Log("c="+QString().setNum(index));
     }
    }
}

//---------------------------------------------

void Launcher::ProcessKeepAliveMessage(QJsonObject *jsonObj_in,QJsonObject *jsonObj_out)
{
    QString ServerTime=jsonObj_in->value("ServerTime").toString();

    qDebug() << "Launcher::ProcessKeepAliveMessage ServerTime "<<ServerTime;

}
