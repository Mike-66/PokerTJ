#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../Base/Helpers.hpp"

MainWindow::MainWindow(TGame *g, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->Game=g;

    ActionByMouse=1;
    ActionByKey=1;

    ShowKeyBinds=1;
    Key_Esc=16777216;
    Key_Start=16777220;
    Key_Raise=Qt::Key_F1;
    Key_Check=Qt::Key_F2;
    Key_Call=Qt::Key_F3;
    Key_Fold=Qt::Key_F4;
    Key_IncBet=Qt::Key_W;
    Key_DecBet=Qt::Key_S;

    ShowMouseBinds=0;
    MouseButton_Start=Qt::NoButton;
    MouseButton_Raise=Qt::MiddleButton;
    MouseButton_Check=Qt::LeftButton;
    MouseButton_Call=Qt::LeftButton;
    MouseButton_Fold=Qt::RightButton;
    MouseButton_IncBet=Qt::NoButton;
    MouseButton_DecBet=Qt::NoButton;

    MouseActor_Start=MOUSEACTOR_BUTTON;
    MouseActor_Raise=MOUSEACTOR_BUTTON;;
    MouseActor_Check=MOUSEACTOR_BUTTON;
    MouseActor_Call=MOUSEACTOR_BUTTON;
    MouseActor_Fold=MOUSEACTOR_BUTTON;
    MouseActor_IncBet=MOUSEACTOR_WHEEL;
    MouseActor_DecBet=MOUSEACTOR_WHEEL;

    MouseWheel_IncBet=1;
    MouseWheel_DecBet=0;

    QVBoxLayout *mainLayout = new QVBoxLayout;

    QWidget *wid = new QWidget(this);
    this->setCentralWidget(wid);

    //Load the deck
    QString url;// = R"(:/img/Pics/0.png)";
    int  value=0;
    int  color=0;
    int  card_width=501;
    int  card_height=726;
#ifdef Q_OS_WIN
    int  scaledcardheight=190;
#else
    int  scaledcardheight=315;
#endif
    for(int i=0;i<NUM_CARDSX;i++)
    {
        url=":/pic/"+QString().setNum(color)+"_"+QString().setNum(value)+".png";
        //qDebug() << url;
        QPixmap img(url);
        PM_Card[i]=img.copy(0,0,card_width,card_height);
        PM_Card[i]=PM_Card[i].scaledToHeight(scaledcardheight);
        value++;
        if(value==NUM_VALUES)
        {
         value=0;
         color++;
        }
    }

    url = R"(:/pic/backside.jpg)";
    QPixmap Card_Back(url);
    PM_Back=Card_Back.scaledToHeight(scaledcardheight);

    //this and that
    QHBoxLayout *layout, *p_layout[2];
    QString H;
    QVBoxLayout *vlayout, vplayer_layout;

    playersGroupBox[0]=new QGroupBox(this);         //Groupbox representing players, containing
    playersGroupBox[0]->setObjectName("GBPlayers");  //NickName, Credit
    playersGroupBox[1]=new QGroupBox(this);         //Card1, Card2
    playersGroupBox[1]->setObjectName("GBPlayers");  //Bet, in a vertical layout .

    p_layout[0] = new QHBoxLayout;                 // the topmost layout for the upper and the lower playerrow
    p_layout[1] = new QHBoxLayout;                 //containing 5 Groupboxes each for a player

    playerGroupBox = new QGroupBox*[MAXPLAYERS];  //QGroupbox to hold horzontal layout of the player  representation
    
    PBPlayer = new QPushButton*[MAXPLAYERS];       //the Buttons containing Nickname if a singledplayer
    LCredit = new ClickableLabel*[MAXPLAYERS];             //the Label for the players credit

    LPlayerCard = new QLabel**[MAXPLAYERS];

    LBet = new QLabel*[MAXPLAYERS];

    //remembers the rows of the player
    // 1) Nickname, Credit -> playerdetailGroupBox
    // 2) Card1, Card2 -> playercardGroupBox
    // 3) Bet -> since its alone needs no GrupBox
    playerdetailGroupBox = new QGroupBox*[MAXPLAYERS]; //Groupbox containing name and credit
    playercardGroupBox = new QGroupBox*[MAXPLAYERS];   //containg the 2 players cards

    //Creation and Object Name
    for(int i=0;i<MAXPLAYERS;i++)
    {
     playerGroupBox[i] = new QGroupBox;
     playerGroupBox[i]->setObjectName("GBPlayer");

     //PBPlayer[i] = new QPushButton("free slot");
     PBPlayer[i] = new QPushButton("");
     PBPlayer[i]->setObjectName("PBPlayer");
     H.setNum(0);
     LCredit[i]=new ClickableLabel();
     LCredit[i]->setText(H);
     LCredit[i]->setToolTip("Click to Rebuy");
     LCredit[i]->setObjectName("LCredit");

     playercardGroupBox[i] = new QGroupBox;
     playercardGroupBox[i]->setObjectName("GBPlayerCard");
     //has fixed size
     playercardGroupBox[i]->setMinimumSize(QSize(20+NUM_PLAYER_CARDS*PM_Card[0].width(),20+PM_Card[0].height()));
     playercardGroupBox[i]->setMaximumSize(QSize(20+NUM_PLAYER_CARDS*PM_Card[0].width(),20+PM_Card[0].height()));

     playerdetailGroupBox[i] = new QGroupBox;
     playerdetailGroupBox[i]->setObjectName("GBPlayerDetail");
     playerdetailGroupBox[i]->setMinimumWidth(playercardGroupBox[i]->minimumWidth());

     LPlayerCard[i] = new QLabel*[NUM_PLAYER_CARDS];
     for(int j=0;j<NUM_PLAYER_CARDS;j++)
     {
      LPlayerCard[i][j] = new QLabel;
      LPlayerCard[i][j]->setObjectName("LCard");
      //has fixed size
      LPlayerCard[i][j]->setMinimumSize(QSize(PM_Card[0].width(),PM_Card[0].height()));
      LPlayerCard[i][j]->setMaximumSize(QSize(PM_Card[0].width(),PM_Card[0].height()));
     }

     LBet[i]=new QLabel(H);
     LBet[i]->setObjectName("LBet");
    }

#ifdef Q_OS_WIN
    //third Row: Players Cards
#endif

    int playerrow=0;
    int pos;
    for(int i=0;i<MAXPLAYERS;i++)
    {
     pos=i;
     playerrow = i>=MAXPLAYERS/2;
     if(playerrow==1)
     pos=MAXPLAYERS-i+4;

     //the players details nickname and credit
     layout = new QHBoxLayout;
     layout->addWidget(PBPlayer[pos],0,Qt::AlignHCenter);
     layout->addWidget(LCredit[pos],0,Qt::AlignHCenter);
     playerdetailGroupBox[pos]->setLayout(layout); //now Nickname and credit are horizontaily in the gb

     layout = new QHBoxLayout;
     for(int j=0;j<NUM_PLAYER_CARDS;j++)
     layout->addWidget(LPlayerCard[pos][j],0,Qt::AlignHCenter);
     playercardGroupBox[pos]->setLayout(layout); //now palyers cards are horizontaily in the gb

     vlayout=new QVBoxLayout;
     if(playerrow==0)
     {
      vlayout->addWidget(playerdetailGroupBox[pos],0,Qt::AlignHCenter);
      vlayout->addWidget(playercardGroupBox[pos],0,Qt::AlignHCenter);
      vlayout->addWidget(LBet[pos],0,Qt::AlignHCenter);              //bet is added solely to the vboxlayout
     }
     else
     {
      vlayout->addWidget(LBet[pos],0,Qt::AlignHCenter); //bet is added solely to the vboxlayout
      vlayout->addWidget(playercardGroupBox[pos],0,Qt::AlignHCenter);
      vlayout->addWidget(playerdetailGroupBox[pos],0,Qt::AlignHCenter);
     }
     playerGroupBox[pos]->setLayout(vlayout);

     p_layout[playerrow]->addWidget(playerGroupBox[pos],0,Qt::AlignHCenter);
    }

    for(int i=0;i<2;i++)
    playersGroupBox[i]->setLayout(p_layout[i]);


    if(MAXPLAYERS>0)
    QObject::connect( PBPlayer[0], SIGNAL(clicked()), this, SLOT(SlotPBPlayer0Clicked()) );
    if(MAXPLAYERS>1)
    QObject::connect( PBPlayer[1], SIGNAL(clicked()), this, SLOT(SlotPBPlayer1Clicked()) );
    if(MAXPLAYERS>2)
    QObject::connect( PBPlayer[2], SIGNAL(clicked()), this, SLOT(SlotPBPlayer2Clicked()) );
    if(MAXPLAYERS>3)
    QObject::connect( PBPlayer[3], SIGNAL(clicked()), this, SLOT(SlotPBPlayer3Clicked()) );
    if(MAXPLAYERS>4)
    QObject::connect( PBPlayer[4], SIGNAL(clicked()), this, SLOT(SlotPBPlayer4Clicked()) );
    if(MAXPLAYERS>5)
    QObject::connect( PBPlayer[5], SIGNAL(clicked()), this, SLOT(SlotPBPlayer5Clicked()) );
    if(MAXPLAYERS>6)
    QObject::connect( PBPlayer[6], SIGNAL(clicked()), this, SLOT(SlotPBPlayer6Clicked()) );
    if(MAXPLAYERS>7)
    QObject::connect( PBPlayer[7], SIGNAL(clicked()), this, SLOT(SlotPBPlayer7Clicked()) );
    if(MAXPLAYERS>8)
    QObject::connect( PBPlayer[8], SIGNAL(clicked()), this, SLOT(SlotPBPlayer8Clicked()) );
    if(MAXPLAYERS>9)
    QObject::connect( PBPlayer[9], SIGNAL(clicked()), this, SLOT(SlotPBPlayer9Clicked()) );

    if(MAXPLAYERS>0)
    QObject::connect( LCredit[0], SIGNAL(clicked()), this, SLOT(SlotLCredit0Clicked()) );
    if(MAXPLAYERS>1)
    QObject::connect( LCredit[1], SIGNAL(clicked()), this, SLOT(SlotLCredit1Clicked()) );
    if(MAXPLAYERS>2)
    QObject::connect( LCredit[2], SIGNAL(clicked()), this, SLOT(SlotLCredit2Clicked()) );
    if(MAXPLAYERS>3)
    QObject::connect( LCredit[3], SIGNAL(clicked()), this, SLOT(SlotLCredit3Clicked()) );
    if(MAXPLAYERS>4)
    QObject::connect( LCredit[4], SIGNAL(clicked()), this, SLOT(SlotLCredit4Clicked()) );
    if(MAXPLAYERS>5)
    QObject::connect( LCredit[5], SIGNAL(clicked()), this, SLOT(SlotLCredit5Clicked()) );
    if(MAXPLAYERS>6)
    QObject::connect( LCredit[6], SIGNAL(clicked()), this, SLOT(SlotLCredit6Clicked()) );
    if(MAXPLAYERS>7)
    QObject::connect( LCredit[7], SIGNAL(clicked()), this, SLOT(SlotLCredit7Clicked()) );
    if(MAXPLAYERS>8)
    QObject::connect( LCredit[8], SIGNAL(clicked()), this, SLOT(SlotLCredit8Clicked()) );
    if(MAXPLAYERS>9)
    QObject::connect( LCredit[9], SIGNAL(clicked()), this, SLOT(SlotLCredit9Clicked()) );


    //MAXPLAYERS

    //4th Row: Pot
    H.setNum(0);
    LPot = new QLabel(H);
    LPot->setObjectName("LPot");
    LLPot=new QLabel("Pot");
    LLPot->setObjectName("LLPot");
    potGroupBox=new QGroupBox(this);
    potGroupBox->setObjectName("GBPot");
    vlayout = new QVBoxLayout;
    vlayout->addWidget(LLPot,0,Qt::AlignHCenter);
    vlayout->addWidget(LPot,0,Qt::AlignHCenter);
    potGroupBox->setLayout(vlayout);
    //HighestBet
    H.setNum(0);
    LHighestBet = new QLabel(H);
    LHighestBet->setObjectName("LHighestBet");
    LLHighestBet=new QLabel("Preflop");
    LLHighestBet->setObjectName("LLHighestBet");
    highestbetGroupBox=new QGroupBox(this);
    highestbetGroupBox->setObjectName("GBHighestBet");
    vlayout = new QVBoxLayout;
    vlayout->addWidget(LLHighestBet,0,Qt::AlignHCenter);
    vlayout->addWidget(LHighestBet,0,Qt::AlignHCenter);
    highestbetGroupBox->setLayout(vlayout);



    //4th Row: boardcards and logview
    LogView=new QTextEdit(this);
    LogView->setReadOnly(true);
    LogView->setObjectName("TELogView");
    LogView->setMaximumWidth(300);
    LogView->setMinimumWidth(300);

    LBoardCard = new QLabel*[NUM_BOARD_CARDS];
    for(int i=0;i<NUM_BOARD_CARDS;i++)
    {
     LBoardCard[i] = new QLabel;
     LBoardCard[i]->setObjectName("LCard");
     LBoardCard[i]->setMinimumSize(QSize(PM_Card[0].width(),PM_Card[0].height()));
     LBoardCard[i]->setMaximumSize(QSize(PM_Card[0].width(),PM_Card[0].height()));
    }
    boardcardGroupBox=new QGroupBox(this);
    boardcardGroupBox->setObjectName("GBBoard");

    QGridLayout *actionlayout = new QGridLayout;
    QGridLayout *actionGrid1 = new QGridLayout;
    QHBoxLayout *actionLine2 = new QHBoxLayout;
    QHBoxLayout *actionLine3 = new QHBoxLayout;

    actionlayout->addLayout(actionGrid1,0,0);
    actionlayout->addLayout(actionLine2,1,0);
    actionlayout->addLayout(actionLine3,2,0);

    LEBet=new QLineEdit(this);
    LEBet->setObjectName("LEBet");
    connect(LEBet,SIGNAL(textChanged(const QString)),this,SLOT(on_LEBet_textChanged(const QString)));

    PBFold=new QPushButton(GetActionButtonText("Fold",Key_Fold,MouseActor_Fold,MouseButton_Fold,0));
    PBFold->setObjectName("PBFold");
    PBCheck=new QPushButton(GetActionButtonText("Check",Key_Check,MouseActor_Check,MouseButton_Check,0));
    PBCheck->setObjectName("PBCheck");
    PBCall=new QPushButton(GetActionButtonText("Call",Key_Call,MouseActor_Call,MouseButton_Call,0));
    PBCall->setObjectName("PBCall");
    PBRaise=new QPushButton(GetActionButtonText("Raise",Key_Raise,MouseActor_Raise,MouseButton_Raise,0));
    PBRaise->setObjectName("PBRaise");
    PBStart=new QPushButton(GetActionButtonText("Start",Key_Start,MouseActor_Start,MouseButton_Start,0));
    PBStart->setObjectName("PBStart");
    if(Game->RunMode==1 && Game->Continue==0)
    PBStart->setEnabled(true);
    else
    PBStart->setEnabled(false);

    int buttonheight=43;
    LEBet->setMaximumWidth(110);
    LEBet->setMinimumHeight(buttonheight);
    PBRaise->setMinimumHeight(buttonheight);
    PBCheck->setMinimumHeight(buttonheight);
    PBCall->setMinimumHeight(buttonheight);
    PBFold->setMinimumHeight(buttonheight);
    PBStart->setMinimumHeight(buttonheight);

    actionGrid1->addWidget(LEBet,0,0);
    actionGrid1->addWidget(PBRaise,0,1);
    actionGrid1->addWidget(PBCheck,1,0);
    actionGrid1->addWidget(PBCall,1,1);
    actionLine2->addWidget(PBFold);
    actionLine3->addWidget(PBStart);

    myActionGroupBox=new QGroupBox(this);
    myActionGroupBox->setObjectName("GBAction");
    myActionGroupBox->setLayout(actionlayout);
    myActionGroupBox->setMinimumWidth(280);
    myActionGroupBox->setMaximumWidth(280);
    DisableExternalBet();

    layout = new QHBoxLayout;
    //layout->addWidget(logviewGroupBox);
    layout->addWidget(LogView);
    layout->addWidget(potGroupBox);
    for(int i=0;i<NUM_BOARD_CARDS;i++)
    layout->addWidget(LBoardCard[i]);
    layout->addWidget(highestbetGroupBox);
    layout->addWidget(myActionGroupBox);
    boardcardGroupBox->setLayout(layout);

    QObject::connect( PBFold, SIGNAL(clicked()), this, SLOT(SlotPBFoldClicked()) );
    QObject::connect( PBCheck, SIGNAL(clicked()), this, SLOT(SlotPBCheckCallClicked()) );
    QObject::connect( PBCall, SIGNAL(clicked()), this, SLOT(SlotPBCheckCallClicked()) );
    QObject::connect( PBRaise, SIGNAL(clicked()), this, SLOT(SlotPBRaiseClicked()) );
    QObject::connect( PBStart, SIGNAL(clicked()), this, SLOT(SlotPBStartClicked()) );

    //The three remaining rows
    mainLayout->addWidget(playersGroupBox[0]);
    mainLayout->addWidget(boardcardGroupBox);
    mainLayout->addWidget(playersGroupBox[1]);

    wid->setLayout(mainLayout);

    SetColors();

    //For reading keyboard, mouse
    qApp->installEventFilter(this);//in constructor

    ToggleConfigVisible=0;

    SoundFileName[MYSOUND_PLAYERSTURN]   ="PlayersTurn.wav";
    SoundFileName[MYSOUND_ACTIONWARNING] = "ActionWarning.wav";

    EffectLoops[MYSOUND_PLAYERSTURN]=1;
    EffectLoops[MYSOUND_ACTIONWARNING]=5;

   for(int i=0;i<MAX_PARALLEL_SOUNDS;i++)
    {

     EffectVolume[i]=0.5f;

     if(fileExists(GetConfigLocation("Sound",SoundFileName[i])))
         effect[i].setSource(QUrl::fromLocalFile(GetConfigLocation("Sound",SoundFileName[i])));
     else
         effect[i].setSource(QUrl::fromLocalFile(":/snd/"+SoundFileName[i]));

     effect[i].setLoopCount(EffectLoops[i]);
     effect[i].setVolume(EffectVolume[i]);
    }

    ActionWarningPercent=50;

    GameiPlayerOld=-1;

    MyTimer = new QTimer(this);
    QObject::connect(MyTimer, SIGNAL(timeout()),this, SLOT(SlotMyTimer()));
    MyTimer->start(100);
}
//----------------------------------------------
MainWindow::~MainWindow()
{
    delete ui;
}
//----------------------------------------------

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
        //qDebug() << "eventFilter " << event->type();

        if (obj == this  && !LogView->hasFocus())
        {
         switch(event->type())
         {
          case QEvent::Wheel:
          {
           if(ActionByMouse==1)
           {
             QWheelEvent *wheelEvent = static_cast<QWheelEvent *>(event);

             qDebug() << "wheelEvent " <<  wheelEvent->angleDelta().ry();

             if(wheelEvent->angleDelta().ry() > 0)
             {
                 //LEBet->setText("up");
                 iBetFaktor++;
             }
             else
             {
                 //LEBet->setText("down");
                 if(iBetFaktor>0)
                 iBetFaktor--;
             }
             if(LEBet->isEnabled())
             LEBet->setText(QString().setNum(iBetFaktor*Game->Blind[1]));
           }
          }break;
          case QEvent::MouseButtonPress:
          {
           QMouseEvent *mouseEvent = static_cast<QMouseEvent*> (event);
           Qt::MouseButtons mouseButton=mouseEvent->button();
           qDebug() << "mousevent " <<  mouseButton;

           if(ActionByMouse==1)
           {
            if( mouseButton== MouseButton_Check || mouseButton== MouseButton_Call )
            {
                if(PBCheck->isEnabled() || PBCall->isEnabled())
                SlotPBCheckCallClicked();
            }else if( mouseButton==MouseButton_Raise )
            {
               if(PBRaise->isEnabled())
               SlotPBRaiseClicked();
            }else if(mouseButton==MouseButton_Fold )
            {
              //LEBet->setText("right");
              if(PBFold->isEnabled())
              SlotPBFoldClicked();
            }
           }
          }break;
          case QEvent::KeyPress:
          {
           QKeyEvent *key = static_cast<QKeyEvent*>(event);
           qDebug() << "keypressed " <<  key->key();
           qDebug() << QKeySequence(key->key()).toString();

           if ( key->key()== Key_Esc )
           ToggleConfigVisible=1;

           if(ActionByKey==1)
           {
             if(key->key()==Key_Start)
             {
                 if(PBStart->isEnabled())
                 SlotPBStartClicked();
             }
             else if (key->key()==Key_Raise)
             {
                 if(PBRaise->isEnabled())
                 SlotPBRaiseClicked();
             }
             else if (key->key()==Key_Check)
             {
                 if(PBCheck->isEnabled())
                 SlotPBCheckCallClicked();
             }
             else if (key->key()==Key_Call)
             {
                 if(PBCall->isEnabled())
                 SlotPBCheckCallClicked();
             }
             else if (key->key()==Key_Fold)
             {
                 if(PBFold->isEnabled())
                 SlotPBFoldClicked();
             }
             else if (key->key()==Key_IncBet)
             {
                 iBetFaktor++;
                 if(LEBet->isEnabled())
                 LEBet->setText(QString().setNum(iBetFaktor*Game->Blind[1]));
             }
             else if (key->key()==Key_DecBet)
             {
                 if(iBetFaktor>0)
                 iBetFaktor--;
                 if(LEBet->isEnabled())
                 LEBet->setText(QString().setNum(iBetFaktor*Game->Blind[1]));
             }
           }
          }break;

         }
        }

 return(false);
}

//----------------------------------------------

void MainWindow::SetColors(void)
{
    QString FileName="stylesheet.qss"; //first check the usr file
    if(!fileExists(FileName))
    FileName=":/qss/stylesheet.qss";
    else
    qDebug() << " using styleshet from " << FileName;

    QFile file(FileName);
    file.open(QFile::ReadOnly);
    QString SS_Buttons = QLatin1String(file.readAll());
    file.close();
    qDebug() << SS_Buttons;
    this->setStyleSheet(SS_Buttons);
}
//----------------------------------------------
void MainWindow::SetGBColor(QGroupBox *q, QColor c)
{
    q->setAutoFillBackground(true);
    q->setPalette(c);
    q->update();
}
//----------------------------------------------
QString MainWindow::GetActionButtonText(QString base, int key, int mouseactor, Qt::MouseButton mb, int wheeldir )
{
    QString Caption;

    if (ShowKeyBinds)
        Caption=QKeySequence(key).toString()+" ";
    else if (ShowMouseBinds)
        Caption=GetMouseActorCaption(mouseactor,mb,wheeldir)+" ";

    Caption+=base;

    return(Caption);
}
//----------------------------------------------
void MainWindow::PlaySound(int sndidx)
{
 if(sndidx<0 || sndidx>=MAX_PARALLEL_SOUNDS)
 {
     Log("MainWindow::PlaySound: ERROR sndix="+QString().setNum(sndidx)+" out of range");
     return;
 }
 Log("MainWindow::PlaySound: vol="+QString().setNum(EffectVolume[sndidx]));
 effect[sndidx].stop();
 effect[sndidx].setLoopCount(EffectLoops[sndidx]);
 effect[sndidx].setVolume(EffectVolume[sndidx]);
 Log("MainWindow::PlaySound: volpre="+QString().setNum(effect[sndidx].volume()));
 effect[sndidx].play();
 Log("MainWindow::PlaySound: volpost="+QString().setNum(effect[sndidx].volume()));
}
//----------------------------------------------
void MainWindow::SetEffectParams(int sndidx, int loops, float vol, int play)
{
 if(sndidx<0 || sndidx>=MAX_PARALLEL_SOUNDS)
 {
  Log("MainWindow::SetEffectParams: ERROR sndix="+QString().setNum(sndidx)+" out of range");
  return;
 }

 EffectLoops[sndidx]=loops;
 EffectVolume[sndidx]=vol;

 //effect[MYSOUND_PLAYERSTURN].setLoopCount(loops);
 //effect[MYSOUND_PLAYERSTURN].setVolume(vol);

 if(play)
 PlaySound(sndidx);
}
//----------------------------------------------
void MainWindow::RefreshTable(void)
{
 DebugTimer.start();

 QString H,I;

 if(Game->iPlayer<0 || Game->iPlayer>=MAXPLAYERS)
 {
     qDebug() << "MainWindow::RefreshTable fatal error Game->iPlayer=" << Game->iPlayer;
     return;
 }

 int isfirsmessagetoplayer=0;
 if(Game->iPlayer!=GameiPlayerOld)
 {
  isfirsmessagetoplayer=1;
  GameiPlayerOld=Game->iPlayer;
 }

 int showplayercontrolls=0;
 if( Game->iWindowPlayer>=0 && Game->iWindowPlayer == Game->iPlayer )
 {
  showplayercontrolls=1;
 }

 QString UserName="local";
 if( Game->iWindowPlayer>=0 && Game->iWindowPlayer<MAXPLAYERS &&
     Game->Player[Game->iWindowPlayer]!=NULL )
 UserName=Game->Player[Game->iWindowPlayer]->GetNickName();
 this->window()->setWindowTitle(UserName+"@"+Game->Name);

 //qDebug() << "entering refresh";

 //Board
 for(int i=0;i<NUM_BOARD_CARDS;i++)
 LBoardCard[i]->clear();
 for(int i=0;i<Game->Board->validcards;i++)
 LBoardCard[i]->setPixmap(PM_Card[Game->Board->iCard[i]]);

 //qDebug() << "board cleared";

 //Players, Bettings and Credit
 for(int i=0;i<MAXPLAYERS;i++)
 {
  PBPlayer[i]->setText("");
  PBPlayer[i]->setEnabled(true);
  LBet[i]->setText("");
  LCredit[i]->setText("");
#ifdef Q_OS_WIN
  for(int j=0;j<NUM_PLAYER_CARDS;j++)
  LPlayerCard[i][j]->clear();
#endif

  LPot->setText("");
  LHighestBet->setText("");
 }

 //qDebug() << "buttons cleared";

 for(int i=0;i<MAXPLAYERS;i++)
 if(Game->Player[i]!=NULL)
 {
  PBPlayer[i]->setText(Game->Player[i]->GetNickName());

  for(int j=0;j<Game->Player[i]->validcards;j++)
  {
   QPixmap *p_PixMap=NULL;
   QPixmap p_tmp;
   if(Game->Player[i]->GetStatus(PLAYER_STATUS_FOLDED) == 0)
   {
    if(Game->Player[i]->iCard[j]==-1 ||
       ( i != Game->iWindowPlayer && Game->Player[i]->GetStatus(PLAYER_STATUS_SHOW_CARDS) == 0) )
    {
     p_PixMap=&PM_Back;
    }
    else
    {
     p_PixMap=&PM_Card[Game->Player[i]->iCard[j]];
    }
   }

   if(p_PixMap!=NULL)
   {
    if ( Game->Player[i]->GetStatus( PLAYER_STATUS_SHOW_CARDS ) == 0 &&
         Game->Player[i]->GetStatus(PLAYER_STATUS_ALL_IN) != 0 )
    {
       QImage image(p_PixMap->size(), QImage::Format_ARGB32_Premultiplied);
       image.fill(Qt::transparent);
       QPainter painter(&image);
       painter.setOpacity(.4);
       painter.drawPixmap(0, 0, *p_PixMap);
       p_tmp = QPixmap::fromImage(image);
       p_PixMap=&p_tmp;
    }

    LPlayerCard[i][j]->setPixmap(*p_PixMap);
   }
  }

  if(Game->Player[i]->ActualBet>0)
  {
   H.setNum(Game->Player[i]->ActualBet);
   LBet[i]->setText(H);
  }

  H=(Game->Player[i]->GetNickName());
  if(Game->iDealer==i)
  {
   H.append(" D");
  }
#ifdef _DEBUG_PLAYER_SEQUENZ_
  else
  {
   I.setNum(Game->GetPlayerPosition(i));
   H.append("#");
   H.append(I);
  }
  if(Game->iBetRoundFinishPlayer==i)
  H.append("f");
#endif

  PBPlayer[i]->setText(H);
  if(i==Game->iPlayer)
  PBPlayer[i]->setEnabled(false);

  H.setNum(Game->Player[i]->Credit);
  LCredit[i]->setText(H);
 }

 if(Game->Pot>0)
 {
  H.setNum(Game->Pot);
  LPot->setText(H);
 }

 if(Game->HighestBet>0)
 {
  H.setNum(Game->HighestBet);
  LHighestBet->setText(H);
 }
 LLHighestBet->setText(Game->DisplayStageName);

 if( showplayercontrolls==1 )
 if(Game->Player[Game->iPlayer]!=NULL)
 {
  if(Game->Player[Game->iPlayer]->GetAI_Class()==AI_HUMAN)
  {
   if(Game->Player[Game->iPlayer]->ExternalBet==EXTERNALBET_REQUESTED ||
      Game->Player[Game->iPlayer]->ExternalBet==EXTERNALBET_SENT )
   {
    PBRaise->setText(GetActionButtonText("Raise",Key_Raise,MouseActor_Raise,MouseButton_Raise,0));
    PBCheck->setText(GetActionButtonText("Check",Key_Check,MouseActor_Check,MouseButton_Check,0));
    PBCall->setText(GetActionButtonText("Call",Key_Call,MouseActor_Call,MouseButton_Call,0));
    PBFold->setText(GetActionButtonText("Fold",Key_Fold,MouseActor_Fold,MouseButton_Fold,0));

    LEBet->setEnabled(true);
    iBetFaktor=1;
    PBFold->setEnabled(true);
    PBCheck->setEnabled(false);
    PBCall->setText(GetActionButtonText("Call",Key_Call,MouseActor_Call,MouseButton_Call,0));
    PBCall->setEnabled(false);
    PBRaise->setEnabled(true);

    H.setNum(Game->RaiseBet);
    LEBet->setText(H);
    PBRaise->setText(GetActionButtonText("Raise",Key_Raise,MouseActor_Raise,MouseButton_Raise,0));

    switch(Game->CallBet)
    {
     case EXTERNALBET_CALLBET_FLIP_CARDS:
     {
      PBCall->setText(GetActionButtonText("flip cards",Key_Call,MouseActor_Call,MouseButton_Call,0));
      PBCall->setEnabled(true);
      LEBet->setText("");
      LEBet->setEnabled(false);
      PBCheck->setEnabled(false);
      PBRaise->setEnabled(false);
     }break;
     case  0:PBCheck->setEnabled(true);break;
     default:PBCall->setEnabled(true);
             PBCall->setText(GetActionButtonText("Call "+QString().setNum(Game->CallBet),Key_Call,MouseActor_Call,MouseButton_Call,0));
             break;
    }

    if(Game->Player[Game->iPlayer]->ExternalBet==EXTERNALBET_REQUESTED /*isfirsmessagetoplayer*/ )
    {
     if( ActionWarningPercent > 0 && Game->CallBet >= ActionWarningPercent/100. * Game->Player[Game->iPlayer]->Credit )
         PlaySound(MYSOUND_ACTIONWARNING);
     else
         PlaySound(MYSOUND_PLAYERSTURN);
    }

    Game->Player[Game->iPlayer]->ExternalBet=EXTERNALBET_SENT;
   }
  }
 }

 for(int i=0;i<Game->CountMessages;i++)
 LogView->append(Game->Message[i]);
 Game->CountMessages=0;

 if(Game->RunMode==1 && Game->iWindowPlayer>=0)
 {
  if( Game->Continue==0 )
  {
      PBStart->setText(GetActionButtonText("Start",Key_Start,MouseActor_Start,MouseButton_Start,0));
      PBStart->setEnabled(true);
  }
  else if ( Game->Continue < 0 )
  {
      PBStart->setText(GetActionButtonText("Pause " +QString().setNum(-Game->Continue),Key_Start,MouseActor_Start,MouseButton_Start,0));
      PBStart->setEnabled(true);
  }
  else
  {
      PBStart->setText(GetActionButtonText("Start",Key_Start,MouseActor_Start,MouseButton_Start,0));
      PBStart->setEnabled(false);
  }
 }

 Log("Mainwindow:RefreshTable ProcessTime "+QString().setNum(DebugTimer.nsecsElapsed() / 1000000.) +" msecs");
}
//-------------------------------------
void MainWindow::SlotMyTimer()
{
  QString H;
  QString LogString;

  //qDebug() << "SMT 1";

  if(Game->isLocalGame)
  {
   Game->NextStep();

  //qDebug() << "SMT 2";

   if(Game->iSendDataTo>GAME_DONTSENDDATA ||
      Game->iSendDataTo_EXTERN>GAME_DONTSENDDATA ||
      Game->CountMessages>0 )
   {
    RefreshTable();
    Game->iSendDataTo_EXTERN=GAME_DONTSENDDATA;
   }
  }

  //qDebug() << "SMT 3";
}
//--------------------------------------------------
void MainWindow::SlotPBPlayer0Clicked()
{
    PBPlayerClicked(0);
}
//--------------------------------------------------
void MainWindow::SlotPBPlayer1Clicked()
{
    PBPlayerClicked(1);
}
//--------------------------------------------------
void MainWindow::SlotPBPlayer2Clicked()
{
    PBPlayerClicked(2);
}
//--------------------------------------------------
void MainWindow::SlotPBPlayer3Clicked()
{
    PBPlayerClicked(3);
}
//--------------------------------------------------
void MainWindow::SlotPBPlayer4Clicked()
{
    PBPlayerClicked(4);
}
//--------------------------------------------------
void MainWindow::SlotPBPlayer5Clicked()
{
    PBPlayerClicked(5);
}
//--------------------------------------------------
void MainWindow::SlotPBPlayer6Clicked()
{
    PBPlayerClicked(6);
}
//--------------------------------------------------
void MainWindow::SlotPBPlayer7Clicked()
{
    PBPlayerClicked(7);
}
//--------------------------------------------------
void MainWindow::SlotPBPlayer8Clicked()
{
    PBPlayerClicked(8);
}
//--------------------------------------------------
void MainWindow::SlotPBPlayer9Clicked()
{
    PBPlayerClicked(9);
}
//--------------------------------------------------
void MainWindow::LCreditClicked(int player)
{
 if(Game->iWindowPlayer==player)
 {
  iSendToServer=REQUESTREBUY;
  Game->iRebuyRequested=Game->Credit;
  Log("Player "+Game->Player[player]->GetNickName()+" requested rebuy " +QString().setNum(Game->iRebuyRequested));
 }
 else
 {
  Log("Player "+Game->Player[player]->GetNickName()+" did request wrong rebuy");
 }
}
//--------------------------------------------------
void MainWindow::SlotLCredit0Clicked()
{
 LCreditClicked(0);
}
//--------------------------------------------------
void MainWindow::SlotLCredit1Clicked()
{
 LCreditClicked(1);
}
//--------------------------------------------------
void MainWindow::SlotLCredit2Clicked()
{
 LCreditClicked(2);
}
//--------------------------------------------------
void MainWindow::SlotLCredit3Clicked()
{
 LCreditClicked(3);
}
//--------------------------------------------------
void MainWindow::SlotLCredit4Clicked()
{
 LCreditClicked(4);
}
//--------------------------------------------------
void MainWindow::SlotLCredit5Clicked()
{
 LCreditClicked(5);
}
//--------------------------------------------------
void MainWindow::SlotLCredit6Clicked()
{
 LCreditClicked(6);
}
//--------------------------------------------------
void MainWindow::SlotLCredit7Clicked()
{
 LCreditClicked(7);
}
//--------------------------------------------------
void MainWindow::SlotLCredit8Clicked()
{
 LCreditClicked(8);
}
//--------------------------------------------------
void MainWindow::SlotLCredit9Clicked()
{
 LCreditClicked(9);
}
//--------------------------------------------------
void MainWindow::PBPlayerClicked(int player)
{
 if(Game->iWindowPlayer==-1)
 {
  iSendToServer=REQUESTPLAYERSEAT;
  Game->iPlayerSeatRequested=player;
 }
 else
 {
  iSendToServer=REQUESTGAMESTATS;
 }

}
//--------------------------------------------------
void MainWindow::on_LEBet_textChanged(const QString &text)
{
 if(Game!=NULL)
 {
  if(Game->iPlayer>=0 && Game->iPlayer<MAXPLAYERS && Game->Player[Game->iPlayer]!=NULL)
  {
    if( text.toInt() >= Game->Player[Game->iPlayer]->Credit )
        PBRaise->setText(GetActionButtonText("ALL IN",Key_Raise,MouseActor_Raise,MouseButton_Raise,0));
    else
        PBRaise->setText(GetActionButtonText("Raise",Key_Raise,MouseActor_Raise,MouseButton_Raise,0));
    if( text.toInt() < Game->RaiseBet )
        PBRaise->setEnabled(false);
    else
        PBRaise->setEnabled(true);
    //if( text.toInt() < Game->CallBet )
    //    PBCheckCall->setEnabled(false);
    //else
    //    PBCheckCall->setEnabled(true);
  }
  else
  {
      qDebug() << "MainWindow::on_LEBet_textChanged fatal error Game->iPlayer=" << Game->iPlayer;
      return;
  }
 }
}
//--------------------------------------------------
void MainWindow::SlotPBFoldClicked()
{
 if(Game->iPlayer>=0 && Game->iPlayer<MAXPLAYERS)
 {
  Game->Player[Game->iPlayer]->ExternalBet=EXTERNALBET_FOLD;
  iSendToServer=REQUESTEXTERNALBET;
  DisableExternalBet();
 }
 else
 {
     qDebug() << "MainWindow::SlotPBFoldClicked fatal error Game->iPlayer=" << Game->iPlayer;
     return;
 }

}
//--------------------------------------------------
void MainWindow::SlotPBCheckCallClicked()
{
 if(Game->iPlayer>=0 && Game->iPlayer<MAXPLAYERS)
 {
  if(Game->CallBet==EXTERNALBET_CALLBET_FLIP_CARDS)
  Game->Player[Game->iPlayer]->ExternalBet=0;
  else
  Game->Player[Game->iPlayer]->ExternalBet=Game->CallBet;
  iSendToServer=REQUESTEXTERNALBET;
  DisableExternalBet();
 }
 else
 {
     qDebug() << "MainWindow::SlotPBCheckCallClicked fatal error Game->iPlayer=" << Game->iPlayer;
     return;
 }

}
//--------------------------------------------------
void MainWindow::SlotPBRaiseClicked()
{
 if(Game->iPlayer>=0 && Game->iPlayer<MAXPLAYERS)
 {
  Game->Player[Game->iPlayer]->ExternalBet=LEBet->text().toInt();
  iSendToServer=REQUESTEXTERNALBET;
  DisableExternalBet();
 }
 else
 {
     qDebug() << "MainWindow::SlotPBRaiseClicked fatal error Game->iPlayer=" << Game->iPlayer;
     return;
 }

}
//--------------------------------------------------
void MainWindow::SlotPBStartClicked()
{
 if(Game->Continue<0)
 Game->Continue=0;
 else
 Game->Continue=1;

 if(Game->isLocalGame)
 Game->iSendDataTo_EXTERN=GAME_SENDDATA;
 else
 iSendToServer=REQUESTCONTINUE;

 PBStart->setEnabled(false);
}
//--------------------------------------------------
void MainWindow::DisableExternalBet(void)
{
 LEBet->setEnabled(false);
 PBFold->setEnabled(false);
 PBCheck->setEnabled(false);
 PBCall->setEnabled(false);
 PBRaise->setEnabled(false);
}
//--------------------------------------------------
void MainWindow::SlotDataReceived(QString data)
{
 //qDebug() << "Mainwindow data" << data;
}
//--------------------------------------------------




