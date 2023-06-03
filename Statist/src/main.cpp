#include <QCoreApplication>

#include "../../Base/Game.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    /**/
    DbQuery=new TDbQuery;
    DbQuery->OpenDatabase();
    DbQuery->CreateModel("configuration_parameters","");
    DbQuery->ShowModel();
    DbQuery->AddToModelOnce("tukukt",666);
    DbQuery->ShowModel();
    /**/

    int CountPlayers=10;
    int StrengthOfBots=2;
    int Credit=50;
    int BigBlind=2;
    int RaiseBlindMinutes=0;
    QString Name="StrategySim";
    QString GameKey="StrategySim.0001";

    RG=new QRandomGenerator(QDateTime::currentSecsSinceEpoch());

    TGame *Game=new TGame(CountPlayers,StrengthOfBots,Credit,BigBlind,RaiseBlindMinutes,Name,GameKey);

    TUser* User[CountPlayers];

    for(int i=0;i<CountPlayers;i++)
    {
     User[i]=CreateUserObject( "Statist"+QString().setNum(i),
                              "StatistNick"+QString().setNum(i),
                              "Admin",
                              AI_COMPUTER_2,
                              GetRandomString(16),
                              "",
                              NULL);

     Game->AddPlayer(i,User[i],Game->Credit);
    }

    Game->Continue=0;
    Game->RunMode=0; // Continuous;
    Game->Stage=GS_INIT;

    /*
    do
    {
        Game->NextStep();
    }while(1);	    
    */

    return a.exec();
}
