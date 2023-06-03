#include "user.hpp"

TUser* CreateUserObject( QString UserName, QString NickName, QString Type, int AI_Class, QString Key, QString GameKey, QWebSocket *pClient)
{
 TUser *u=new TUser(UserName,NickName,Type,AI_Class,Key,GameKey,pClient);
 Log("Created User "+UserName+"/"+NickName+"/"+Type+"/"+QString().setNum(AI_Class)+"/"+Key+"/"+GameKey);
 return(u);
}

//-----------------------------------------------------
TUser::TUser( QObject *parent) : QObject(parent)
{
 UserName="UN_Const";
 NickName="NN_Const";
 Type="Type_Const";
 Key="";
 GameKey="";
 pClient=NULL;
 AI_Class=1;
 LastSeen=0;
}
//-----------------------------------------------------
TUser::TUser( QString UserName, QString NickName, QString Type,int AI_Class, QString Key, QString GameKey, QWebSocket *pClient, QObject *parent) : QObject(parent)
{
 this->UserName=UserName;
 this->NickName=NickName;
 this->Type=Type;
 this->Key=Key;
 this->GameKey=GameKey;
 this->pClient=pClient;
 this->AI_Class=AI_Class;

 LastSeen=0;
 BannedUntil=0;
}

//-----------------------------------------------------

QJsonObject TUser::AsJsonObject(int detail)
{
    QJsonObject O;

    O.insert("UserName",UserName);
    O.insert("NickName",NickName);
    O.insert("Type",Type);
    O.insert("detail",detail);
    if(detail>0)
    O.insert("Key",Key);
    O.insert("GameKey",GameKey);
    O.insert("pClient","NULL");
    O.insert("LastSeen",LastSeen);
    O.insert("AI_Class",AI_Class);
    O.insert("BannedUntil",BannedUntil);

    return(O);
}

//-----------------------------------------------------

void TUser::FromJsonObject(QJsonObject *O)
{
    UserName=O->value("UserName").toString();
    NickName=O->value("NickName").toString();
    Type=O->value("Type").toInt();
    int detail=O->value("detail").toInt();
    if(detail>0)
    Key=O->value("Key").toString();
    GameKey=O->value("GameKey").toString();
    //pClient=NULL; //leave pcClient untouched
    LastSeen=O->value("LastSeen").toInt();
    AI_Class=O->value("AI_Class").toInt();
    BannedUntil=O->value("BannedUntil").toInt();
}

//--------------------------------------------

void TUser::UpdateLastSeen()
{
    LastSeen=QDateTime::currentSecsSinceEpoch();
}

//--------------------------------------------
