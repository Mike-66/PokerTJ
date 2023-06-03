#include "launcher.hpp"

//---------------------------------------------

Launcher::Launcher()
{
 CheckUsersInterval=150;
 CheckUsersTOis=CheckUsersInterval;
 UserInactiveTimeout=180;
 UserInactiveTimeout=45;

 ServerMessageId=0;
 ServerMessageIdSent=0;

 /*
 DbQuery=new TDbQuery;

 if( ! LoadSettings("/etc/pokertjd.conf") )
 if( ! LoadSettings( QStandardPaths::writableLocation(QStandardPaths::ConfigLocation)+"/pokertjd.conf" ) )
 SaveSettings( QStandardPaths::writableLocation(QStandardPaths::ConfigLocation)+"/pokertjd.conf" );
 */

 //+++ProcessTiming
 WorkTimerInterIntervallDesired=100; //msecs
 WorkTimer = new QTimer;
 connect(WorkTimer, SIGNAL(timeout()),this, SLOT(onTimer()));
 WorkTimer->start(WorkTimerInterIntervallDesired);
 LoadTimer.start();
 //---ProcessTiming

 /*
 DbQuery->OpenDatabase();
 DbQuery->CreateModel("configuration_parameters","");
 */

 //Create Server
 WS_Server=new TWS_Server(this,9002,1);
}

//---------------------------------------------

bool Launcher::LoadSettings(QString FileName)
{
    QSettings settings(FileName,QSettings::IniFormat);

    if(!settings.contains("Database/User"))
    return(false);

    /*
    DbQuery->DB_User=settings.value("Database/User").toString();
    DbQuery->DB_Role=settings.value("Database/Role").toString();
    DbQuery->DB_Password=settings.value("Database/Password").toString();
    DbQuery->DB_Host=settings.value("Database/Host").toString();
    DbQuery->DB_Name=settings.value("Database/Name").toString();
    */

    Log("loaded settings from "+FileName);

    return(true);
}

//---------------------------------------------

void Launcher::SaveSettings(QString FileName)
{
    QSettings settings(FileName,QSettings::IniFormat);

    /*
    settings.setValue("Database/User",DbQuery->DB_User);
    settings.setValue("Database/Role",DbQuery->DB_Role);
    settings.setValue("Database/Password",DbQuery->DB_Password);
    settings.setValue("Database/Host",DbQuery->DB_Host);
    settings.setValue("Database/Name",DbQuery->DB_Name);
    */

    settings.sync();

    Log("saved User settings to "+FileName);
}
//---------------------------------------------

void Launcher::onTimer()
{
 //+++ProcessTiming
 WorkTimer->stop();
 qint64 TimeOutSideTimer=LoadTimer.nsecsElapsed();
 LoadTimer.start();
 //---ProcessTiming

 //DbQuery->ShowModel();
 //DbQuery->UpdateModel("Launcher.BBoot",723);

 /*
 if(DbQuery->OpenSQL(0))
 while(DbQuery->FindNext())
 {
  QString Name;
  int     Value;
  DbQuery->GetField("name",&Name);
  DbQuery->GetField("1",&Value);
  Log(Name+"="+QString().setNum(Value));
 }
 */

 //Log("Launcher Timer Ticked");
 for( int i=0; i<Games.count(); ++i )
 {
  Games[i]->NextStep();

  if(Games[i]->iSendDataTo>GAME_DONTSENDDATA ||
     Games[i]->iSendDataTo_EXTERN>GAME_DONTSENDDATA ||
     Games[i]->CountMessages>0 )
  {
   Games[i]->iSendDataTo_EXTERN=GAME_DONTSENDDATA;

   for(int j=0;j<MAXPLAYERS;j++)
   if(Games[i]->Player[j]!=NULL &&
      Games[i]->Player[j]->User!=NULL &&
      Games[i]->Player[j]->User->pClient!=NULL)
   {
    QJsonObject O;
    CopyGameToJsonObject(Games[i],j,&O);
    Log("SendJsonDocument to Player "+ Games[i]->Player[j]->GetNickName());
    SendJsonDocument(Games[i]->Player[j]->User->pClient,"GameState",0,&O);
   }

   for(int j=0;j<Games[i]->Spotters.count();j++)
   if(Games[i]->Spotters[j]->pClient!=NULL)
   {
    QJsonObject O;
    CopyGameToJsonObject(Games[i],-1,&O);
    Log("SendJsonDocument to Spotter "+ Games[i]->Spotters[j]->NickName);
    SendJsonDocument(Games[i]->Spotters[j]->pClient,"GameState",0,&O);
   }

   Games[i]->CountMessages=0;

   if(Games[i]->iSendDataTo>=GAME_SENDDATAUSER)
   if(Games[i]->Player[Games[i]->iSendDataTo]->ExternalBet==EXTERNALBET_REQUESTED)
   {
    Games[i]->Player[Games[i]->iSendDataTo]->ExternalBet=EXTERNALBET_SENT;
    Games[i]->ExternalBet_UserKey=Games[i]->Player[Games[i]->iSendDataTo]->User->Key;
   }

  }
 }

 if(CheckUsersTOis>0 && --CheckUsersTOis==0)
 {
  CheckUsers();
  CheckUsersTOis=CheckUsersInterval;
 }

 //+++ProcessTiming
 QElapsedTimer t;
 t.start();
 qint64 TimeInsideTimer=LoadTimer.nsecsElapsed();
 //Log("TimeInsideTimer="+QString().setNum(TimeInsideTimer)+"nsecs");
 //Log("TimeOutSideTimer="+QString().setNum(TimeOutSideTimer)+"nsecs");
 qint64 nextintervall=WorkTimerInterIntervallDesired*1000000-TimeInsideTimer;
 float nextintervallmsecs=nextintervall/1000000.;
 //Log("nextintervall="+QString().setNum(nextintervallmsecs)+"msecs");
 float gameload=100.-nextintervallmsecs;
 float responseload=ResponseTimeTotal/1000000.;
 //Log("GameLoad="+QString().setNum(gameload)+"%");
 //Log("Response="+QString().setNum(responseload)+"%");
 //Log("LoadTotal="+QString().setNum(gameload+responseload));
 ResponseTimeTotal=0;
 //Log("thisrubbish="+QString().setNum(t.nsecsElapsed()));

 LoadTimer.start();
 WorkTimer->start((int)nextintervallmsecs);
 //---ProcessTiming
}

//---------------------------------------------

void Launcher::LogOff(TUser* User)
{
    RemoveUserFromGame(User);
    ActiveUsers.removeAll(User);
    delete(User);
}

//---------------------------------------------

int Launcher::RemoveUserFromGame(TUser* User)
{
    int res=0;
    Log("RemoveUserFromGame for "+User->Key);

    TPlayer *Player;
    TGame *Game=GetGameByUserKey(User->Key,&Player,NULL);
    if(Game!=NULL)
    Game->RemovePlayer(Player);
    else
    {
     Log("Found no game for User "+User->Key);
     res=-1;
    }

    int foundhuman=0;
    if(Game!=NULL)
    {
     for(int i=0;i<MAXPLAYERS;i++)
     if(Game->Player[i]!=NULL && Game->Player[i]->User!=NULL)
     if(Game->Player[i]->User->AI_Class==AI_HUMAN)
     {
      foundhuman=1;
      break;
     }
     if(foundhuman==0)
     {
      Log("last human quit, deleting game "+Game->Key);
      delete(Game);
      Games.removeAll(Game);
     }
    }

    Game=GetGameByUserKeySpotter(User->Key);
    if(Game!=NULL)
    {
     Log("Game "+Game->Key +" removing Spotter");
     Game->Spotters.removeAll(User);
    }

    return(res);
}


//---------------------------------------------

void Launcher::CheckUsers(void)
{
    Log("CheckUsers Active Users: "+QString().setNum(ActiveUsers.count()));
    for(int i=0;i<ActiveUsers.count();i++)
    {
     TUser *User=ActiveUsers[i];
     qint64 tnow=QDateTime::currentSecsSinceEpoch();
     qint64 notseenfor;

     if( User->LastSeen==0 )
     notseenfor=-1;
     else
     notseenfor=tnow-User->LastSeen;

     Log("CheckUsers "+User->UserName+" not seen since "+QString().setNum(notseenfor)+" secs "+QString().setNum(UserInactiveTimeout));

     if(notseenfor>UserInactiveTimeout)
     {
        QString UserName=User->UserName;
        Log("CheckUsers request Logoff for User "+UserName);
        WS_Server->RequestClose(UserName, User->pClient);
        LogOff(User);
        Log("CheckUsers Logoff User "+UserName+", Active Users: "+QString().setNum(ActiveUsers.count()));
     }
    }
}

//---------------------------------------------

TGame* Launcher::CreateGame(int CountPlayers, int StrengthOfBots, int Credit, int BigBlind, int RaiseBlindMinutes, QString Name)
{
    QString GameKey=Name+"_"+GetRandomString(24);
    TGame *Game=new TGame(CountPlayers,StrengthOfBots,Credit,BigBlind,RaiseBlindMinutes,Name,GameKey);
    Games << Game;
    return(Game);
}

//---------------------------------------------

void Launcher::on_WS_Socketdisonnected(QWebSocket* pClient)
{
    Log("Launcher::on_WS_Socketdisonnected");

    TUser *User=NULL;
    for(int i=0;i<ActiveUsers.count();i++)
    {
        if(ActiveUsers[i]->pClient==pClient)
        {
            User=ActiveUsers[i];
            break;
        }
    }
    if(User!=NULL)
    {
        //Log("found User with Key " << User->Key);
        //delete(User);
        //ActiveUsers.removeAll(User);
        //Log("deleted User, ActiveUsers.count() now " + ActiveUsers.count());
        User->pClient=NULL;
        Log("updated socket for User with Key " + User->Key);
    }

}

//---------------------------------------------

void Launcher::on_WS_MessageReceived(QString message)
{
    //Log("Launcher::on_WS_MessageReceived " + message);
}

//---------------------------------------------

void Launcher::on_WS_BinaryMessageReceived(QWebSocket* pClient,QByteArray message)
{
    Log("Launcher::on_WS_BinaryMessageReceived");
    ResponseTimer.start();

    QString MessageType;
    int     ClientMessageId;

    QJsonParseError parseError;
    QJsonDocument jsonDoc;
    jsonDoc = QJsonDocument::fromJson(message, &parseError);
    if(parseError.error != QJsonParseError::NoError){
            qWarning() << "Parse error at " << parseError.offset << ":" << parseError.errorString();
            return;
    }

    QJsonObject jsonObj_in,jsonObj_out;
    jsonObj_in = jsonDoc.object();
    jsonObj_out.empty();

    MessageType=jsonObj_in.value("MessageType").toString();
    ClientMessageId=jsonObj_in.value("ClientMsgId").toInt();

    if(MessageType=="Login")
    {
     ProcessLoginMessage(pClient,&jsonObj_in,&jsonObj_out);
    }
    else
    {
     QString UserKey;
     UserKey=jsonObj_in.value("UserKey").toString();

     int iUser=-1;
     if( UserKey!="")
     for( int i=0; i<ActiveUsers.count(); ++i )
     {
         if( ActiveUsers[i]->Key==UserKey )
         {
            if ( ActiveUsers[i]->pClient!=pClient )
            {
                ActiveUsers[i]->pClient=pClient;
                Log("updated socket for User with key " + ActiveUsers[i]->Key);
            }
            if(ActiveUsers[i]->BannedUntil<QDateTime::currentSecsSinceEpoch())
            ActiveUsers[i]->UpdateLastSeen();
            iUser=i;
            break;
         }
     }

     if(iUser<0)
     {
        Log("Authentication required for Key " + UserKey);
        MessageType="Login";
        jsonObj_out.insert("ServerInfo","authentication required");
     }
     else if(MessageType=="KeepAlive")
     {
        ProcessKeepAliveMessage(&jsonObj_in,&jsonObj_out);
     }
     else if(MessageType=="Continue")
     {
        ProcessContinueMessage(UserKey,&jsonObj_in,&jsonObj_out);
     }
     else if(MessageType=="ExternalBet")
     {
        ProcessExternalBetMessage(UserKey,&jsonObj_in,&jsonObj_out);
     }
     else if(MessageType=="PlayerState")
     {
        ProcessPlayerStateMessage(UserKey, &jsonObj_in,&jsonObj_out);
     }
     else if(MessageType=="GameState")
     {
        ProcessGameStateMessage(UserKey,&jsonObj_in,&jsonObj_out);
     }
     else if(MessageType=="GameStats")
     {
        ProcessGameStatsMessage(UserKey,&jsonObj_in,&jsonObj_out);
     }
     else if(MessageType=="RequestGame")
     {
        ProcessRequestGameMessage(UserKey,iUser, &jsonObj_in,&jsonObj_out);
     }
     else if(MessageType=="PlayerSeat")
     {
        ProcessRequestPlayerSeat(UserKey, iUser, &jsonObj_in,&jsonObj_out);
     }
     else if(MessageType=="AvailableGames")
     {
        ProcessAvailableGames(UserKey,&jsonObj_in,&jsonObj_out);
     }
     else if(MessageType=="Rebuy")
     {
        ProcessRequestRebuy(UserKey, iUser, &jsonObj_in,&jsonObj_out);
     }
     else if(MessageType=="CreateGame")
     {
        ProcessRequestCreateGame(UserKey, iUser, &jsonObj_in,&jsonObj_out);
     }
     else if(MessageType=="LeaveGame")
     {
        ProcessRequestLeaveGame(iUser,&jsonObj_in,&jsonObj_out);
     }
     else
     {
        Log("unknown MessageType " +MessageType);
     }
    }

    if(!jsonObj_out.isEmpty())
    {
     SendJsonDocument(pClient,MessageType,ClientMessageId,&jsonObj_out);
    }

    ResponseTimeTotal+=ResponseTimer.nsecsElapsed();
}

//---------------------------------------------

TUser* Launcher::ProcessLoginMessage(QWebSocket* pClient,QJsonObject *jsonObj_in,QJsonObject *jsonObj_out)
{
    Log("ProcessLogin");

    TUser* User=NULL;

    QString UserName;
    QString Password;

    UserName=jsonObj_in->value("UserName").toString();
    Password=jsonObj_in->value("Password").toString();

    Log("UserName/Password" + UserName + "/" + Password);

    //From Database
    QString NickName="N_"+UserName;
    QString Type="Admin";

    //if( UserExists || PasswordOK)
    {
        for(int i=0;i<ActiveUsers.count();i++)
        {
            if(ActiveUsers[i]->UserName==UserName)
            {
                User=ActiveUsers[i];
                break;
            }
        }

        if(User==NULL)
        {
            QString UserKey="Key_"+UserName+GetRandomString(24);
            User=CreateUserObject(UserName,NickName,Type,AI_HUMAN,UserKey,"",pClient);
            ActiveUsers << User;
            Log("Login successfull for User " + UserName +", Active Users: " + ActiveUsers.count());
        }
    }

    if( User!=NULL ) //LOGINOK
    {
     jsonObj_out->insert("UserKey",User->Key);
     jsonObj_out->insert("UserType","Admin");
    }
    else
    {
     Log("Authentication failed for " + UserName);
     jsonObj_out->insert("ServerInfo","authentication failed");
    }

    return(User);
}

//---------------------------------------------

void Launcher::ProcessPlayerStateMessage(QString UserKey, QJsonObject *jsonObj_in,QJsonObject *jsonObj_out)
{
    Log("ProcessPlayerState for UserKey " + UserKey);
    PrepareAvailableGames(UserKey,jsonObj_out);
}

//---------------------------------------------

void Launcher::ProcessAvailableGames(QString UserKey,QJsonObject *jsonObj_in,QJsonObject *jsonObj_out)
{
    Log("ProcessAvailableGames for UserKey " + UserKey);
    PrepareAvailableGames(UserKey, jsonObj_out);
}

//---------------------------------------------

void Launcher::PrepareAvailableGames(QString UserKey, QJsonObject *jsonObj_out)
{
    Log("Launcher::PrepareAvailableGames");

    TGame *Game=GetGameByUserKey(UserKey,NULL,NULL);
    if( Game != NULL )
    jsonObj_out->insert("GameKey",Game->Key);

    QJsonArray jsonArray;
    for( int i=0; i<Games.count(); i++ )
    {
        QJsonObject o;
        o.insert("Name",Games[i]->Name);
        o.insert("Players",Games[i]->CountPlayers_Default);
        o.insert("Active Players",Games[i]->CountPlayers);
        o.insert("Spotters",Games[i]->Spotters.count());
        o.insert("Bots",Games[i]->GetCountBots());
        o.insert("Strength",Games[i]->StrengthOfBots);
        o.insert("Credit",Games[i]->Credit);
        o.insert("Big Blind",Games[i]->Blind[1]);
        o.insert("Raise",Games[i]->RaiseBlindMinutes);
        o.insert("Key",Games[i]->Key);
        jsonArray.append(o);
    }
    jsonObj_out->insert("AvailableGames",jsonArray);

}

//---------------------------------------------

void Launcher::ProcessRequestGameMessage(QString UserKey, int iUser, QJsonObject *jsonObj_in,QJsonObject *jsonObj_out)
{
        Log("ProcessRequestGame");

        QString GameKey=jsonObj_in->value("GameKey").toString();

        TGame *Game=GetGameByKey(GameKey);

        if(Game!=NULL)
        {
         int player_idx=GetPlayerIndex(Game,UserKey);
         if( player_idx<0)
         Game->AddSpotter(ActiveUsers[iUser]);
         CopyGameToJsonObject(Game,player_idx,jsonObj_out);
        }
        else
        {
         jsonObj_out->insert("ServerInfo","There is no game with this key");
        }
}

//---------------------------------------------

void Launcher::ProcessRequestCreateGame(QString UserKey, int iUser,QJsonObject *jsonObj_in,QJsonObject *jsonObj_out)
{
    Log("Launcher::RequestCreateGame");

    QString GameName=jsonObj_in->value("GameName").toString();
    int CountPlayers=jsonObj_in->value("CountPlayers").toInt();
    int Credit=jsonObj_in->value("Credit").toInt();
    int StrengthofBots=jsonObj_in->value("StrengthofBots").toInt();
    int BigBlind=jsonObj_in->value("BigBlind").toInt();
    int RaiseBlindMinutes=jsonObj_in->value("RaiseBlindMinutes").toInt();

    TGame* Game=GetGameByName(GameName);

    if(Game!=NULL)
    {
     jsonObj_out->insert("ServerInfo","a game with that name already exists");
     goto error;
    }

    CreateGame(CountPlayers,StrengthofBots,Credit,BigBlind,RaiseBlindMinutes,GameName);

    PrepareAvailableGames(UserKey, jsonObj_out);

error:
    return;

}

//---------------------------------------------

void Launcher::SendJsonDocument(QWebSocket *pClient, QString MessageType, int ClientMessageId, QJsonObject* jsonObj_out)
{
    jsonObj_out->insert("MessageType",MessageType);
    jsonObj_out->insert("ServerMsgId",ServerMessageId);
    if(ClientMessageId>0)
    jsonObj_out->insert("ClientMsgId",ClientMessageId);
    QJsonDocument jsondoc=QJsonDocument(*jsonObj_out);
    WS_Server->SendJsonDocument(pClient,&jsondoc);
    ServerMessageIdSent=ServerMessageId;
}

//---------------------------------------------

void Launcher::CopyGameToJsonObject(TGame *Game,int iPlayer,QJsonObject *O)
{
    O->insert("Msg","GetAll");
    QJsonArray  a;
    O->insert("Game",Game->AsJsonObject());
    O->insert("Board",Game->Board->AsJsonObject());
    for(int i=0;i<MAXPLAYERS;i++)
    if(Game->Player[i]!=NULL)
    a.append(Game->Player[i]->AsJsonObject(i,iPlayer==i));
    O->insert("Players",a);
}

//---------------------------------------------

void Launcher::CopyGameStatsToJsonObject(TGame *Game,QJsonObject *O)
{
    QJsonArray  a;
    for(int i=0;i<MAXPLAYERS;i++)
    if(Game->Player[i]!=NULL)
    a.append(Game->Player[i]->StatAsJsonObject(i));
    O->insert("Stats",a);
}


//---------------------------------------------

void Launcher::ProcessRequestLeaveGame(int iUser,QJsonObject *jsonObj_in,QJsonObject *jsonObj_out)
{
    Log("Launcher::RequestLeaveGame");
    if(RemoveUserFromGame(ActiveUsers[iUser])==0)
    jsonObj_out->insert("ServerInfo","successfully left game");
    else
    jsonObj_out->insert("ServerInfo","you are actually not registered in any game");


    //ProcessPlayerStateMessage(ActiveUsers[iUser]->Key, jsonObj_in, jsonObj_out);
}

//---------------------------------------------

void Launcher::ProcessRequestPlayerSeat(QString UserKey, int iUser,QJsonObject *jsonObj_in,QJsonObject *jsonObj_out)
{
    Log("Launcher::RequestPlayerSeat");
    int iPlayerSeatRequested=jsonObj_in->value("iPlayerSeatRequested").toInt();
    QString NickName=jsonObj_in->value("NickName").toString();
    QString GameKey=jsonObj_in->value("GameKey").toString();

    TGame *Game;

    if( iPlayerSeatRequested<0 || iPlayerSeatRequested >= MAXPLAYERS )
    {
     jsonObj_out->insert("ServerInfo","don't even try");
     goto error;
    }
    else
    {
     Game=GetGameByUserKey(UserKey,NULL,NULL);
    }

    if(Game!=NULL) //User must not allready be in any Game, leave it first
    {
     jsonObj_out->insert("ServerInfo","You have already taken a seat in Game "+Game->Name+ " please leave it first");
     goto error;
    }
    else
    {
     Game=GetGameByKey(GameKey);
    }

    if(Game==NULL) //No Game with this key
    {
     jsonObj_out->insert("ServerInfo","There is no Game with this Key");
     goto error;
    }
    else if(Game->Player[iPlayerSeatRequested]!=NULL)
    {
     jsonObj_out->insert("ServerInfo","Please take another seat");
     goto error;
    }
    else if(Game->Stage>GS_INIT && Game->Stage<GS_CLEAR /*!!PAUSE!!*/ )
    {
     jsonObj_out->insert("ServerInfo","Try again before any card has been dealt");
     goto error;
    }

    ActiveUsers[iUser]->NickName=NickName; //will be removed if db on server is available
    Game->AddPlayer(iPlayerSeatRequested,ActiveUsers[iUser],Game->Credit);
    Game->iSendDataTo_EXTERN=GAME_SENDDATA;

error:
    return;
}
//---------------------------------------------

void Launcher::ProcessRequestRebuy(QString UserKey, int iUser,QJsonObject *jsonObj_in,QJsonObject *jsonObj_out)
{
    Log("Launcher::RequestRebuy");
    int iRebuyRequested=jsonObj_in->value("iRebuyRequested").toInt();
    QString GameKey=jsonObj_in->value("GameKey").toString();

    TPlayer *Player;
    int     iPlayer;
    TGame *Game=GetGameByUserKey(UserKey,&Player,&iPlayer);

    if(Game==NULL || Player==NULL)
    {
     jsonObj_out->insert("ServerInfo","No Game found to process rebuy for "+ ActiveUsers[iUser]->NickName);
     goto error;
    }

    if(Game->PerformRebuy(iPlayer,iRebuyRequested)!=0)
    {
     jsonObj_out->insert("ServerInfo","sorry "+ActiveUsers[iUser]->NickName+", you have no more credit in this house");
     goto error;
    }

    Game->iSendDataTo_EXTERN=GAME_SENDDATA;

error:
    return;
}

//---------------------------------------------

int Launcher::GetPlayerIndex(TGame* Game, QString UserKey)
{
    int iPlayer=-1;
    for(int i=0;i<MAXPLAYERS;i++)
    if(Game->Player[i]!=NULL && Game->Player[i]->User !=NULL)
    if(Game->Player[i]->User->Key==UserKey)
    {
     iPlayer=i;
     break;
    }
    return(iPlayer);
}

//---------------------------------------------

void Launcher::ProcessGameStateMessage(QString UserKey, QJsonObject *jsonObj_in,QJsonObject *jsonObj_out)
{
        Log("ProcessGameState");

        QString Msg=jsonObj_in->value("Msg").toString();
        QString GameKey=jsonObj_in->value("GameKey").toString();

        Log("Msg/GameKey " + Msg+ "/"+GameKey);

        TPlayer *Player;
        int     iPlayer;
        TGame *Game=GetGameByUserKey(UserKey,&Player,&iPlayer);

        if(Game==NULL)
        {
            jsonObj_out->insert("ServerInfo","A Game with this key doesn't exist");
            jsonObj_out->insert("GameKey",GameKey);
        }
        else
        {
            CopyGameToJsonObject(Game,iPlayer,jsonObj_out);
        }
}

//---------------------------------------------

void Launcher::ProcessGameStatsMessage(QString UserKey, QJsonObject *jsonObj_in,QJsonObject *jsonObj_out)
{
        QString GameKey=jsonObj_in->value("GameKey").toString();
        Log("ProcessGameStats "+GameKey);

        TGame *Game=GetGameByKey(GameKey);

        if(Game==NULL)
        {
            jsonObj_out->insert("ServerInfo","A Game with this key doesn't exist");
            jsonObj_out->insert("GameKey",GameKey);
        }
        else
        {
            CopyGameStatsToJsonObject(Game,jsonObj_out);
        }
}


//---------------------------------------------

void Launcher::ProcessKeepAliveMessage(QJsonObject *jsonObj_in,QJsonObject *jsonObj_out)
{
        jsonObj_out->insert("ServerTime",QDateTime::currentDateTime().toString());
}

//---------------------------------------------

void Launcher::ProcessContinueMessage(QString UserKey,QJsonObject *jsonObj_in,QJsonObject *jsonObj_out)
{
    TGame *Game=GetGameByUserKey(UserKey,NULL,NULL);
    if(Game!=NULL)
    {
     int conti=jsonObj_in->value("Continue").toInt();
     if(conti>0)
     Game->Continue=conti;
     else if ( Game->Continue < 0)
     {
      Game->Continue=conti;
      Game->iSendDataTo_EXTERN=GAME_SENDDATA;
     }
    }
}

//---------------------------------------------

void Launcher::ProcessExternalBetMessage(QString UserKey, QJsonObject *jsonObj_in,QJsonObject *jsonObj_out)
{
    Log("ProcessExternalBet");

    TPlayer *Player;
    TGame *Game=GetGameByUserKey(UserKey,&Player,NULL);
    if(Game!=NULL)
        Log("ProcessExternalBet found Game " + Game->Key);
    if(Player!=NULL)
        Log("ProcessExternalBet found Player " + Player->User->Key);
    if(Game!=NULL && Player!=NULL)
    {
     if(Game->ExternalBet_UserKey==UserKey)
     {
      Player->ExternalBet=jsonObj_in->value("ExternalBet").toInt();
      Game->ExternalBet_UserKey="";
     }
     else
     Log("ProcessExternalBet UserKey Error: Key " + UserKey + " wanted to bet for " + Game->ExternalBet_UserKey);
    }

}


//---------------------------------------------

TGame* Launcher::GetGameByName(QString GameName)
{
    for( int i=0; i<Games.count(); ++i )
    if(Games[i]->Name==GameName)
    return(Games[i]);

    return(NULL);
}

//---------------------------------------------

TGame* Launcher::GetGameByKey(QString GameKey)
{
    for( int i=0; i<Games.count(); ++i )
    if(Games[i]->Key==GameKey)
    return(Games[i]);

    return(NULL);
}

//-----------------------------------------------

TGame* Launcher::GetGameByUserKey(QString UserKey, TPlayer **Player, int *iPlayer)
{
    Log("GetGameByUserKey for "+UserKey);
    if(Player!=NULL)
    Player[0]=NULL;
    if(iPlayer!=NULL)
    iPlayer[0]=-1;

    for( int i=0; i<Games.count(); i++ )
    {
        for(int j=0;j<MAXPLAYERS;j++)
        {
         if(Games[i]->Player[j] !=NULL &&
            Games[i]->Player[j]->User != NULL &&
            Games[i]->Player[j]->User->Key==UserKey)
            {
             Log("GetGameByUserKey found Games[i]->Player[j]->User->Key"+Games[i]->Player[j]->User->Key);
             if(Player!=NULL)
             Player[0]=Games[i]->Player[j];
             if(iPlayer!=NULL)
             iPlayer[0]=j;
             return(Games[i]);
            }
        }
    }
 return(NULL);
}

//-----------------------------------------------

TGame* Launcher::GetGameByUserKeySpotter(QString UserKey)
{
    for( int i=0; i<Games.count(); i++ )
    for(int j=0;j<Games[i]->Spotters.count();j++)
    if(Games[i]->Spotters[j]->Key==UserKey)
    return(Games[i]);
    return(NULL)     ;
}


