#include "launcherdialog.hpp"
#include "ui_launcherdialog.h"

#include <QDebug>

LauncherDialog::LauncherDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LauncherDialog)
{
    ui->setupUi(this);
    QPushButton *reloadButton= new QPushButton(tr("&Reload"));
    reloadButton->setDefault(true);
    ui->buttonBox->addButton(reloadButton,QDialogButtonBox::ActionRole);

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(onOKclicked()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(onCancelclicked()));
    connect(reloadButton, SIGNAL(clicked()), this, SLOT(onReloadclicked()));
    connect(ui->PBLogin, SIGNAL(clicked()), this, SLOT(OnPBLoginClicked()));
    connect(ui->PBCreateGame, SIGNAL(clicked()), this, SLOT(OnPBCreateGameClicked()));
    connect(ui->PBLeaveGame, SIGNAL(clicked()), this, SLOT(OnPBLeaveGameClicked()));
    connect(ui->GameTableView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(OnGameTableClicked(const QModelIndex &)));;
    connect(ui->RBLocal, SIGNAL(toggled(bool)),this,SLOT(OnRBToggled(bool)));
    connect(ui->CBShowKeyBinds, SIGNAL(clicked()),this,SLOT(OnCBShowKeyBindsChecked()));
    connect(ui->CBShowMouseBinds,SIGNAL(clicked()),this,SLOT(OnCBShowMouseBindsChecked()));
    connect(ui->CBActionByKey, SIGNAL(clicked()),this,SLOT(onCBActionByKeyChecked()));
    connect(ui->CBActionByMouse, SIGNAL(clicked()),this,SLOT(onCBActionByMouseChecked()));
    connect(ui->hSLPlayersTurnVolume, SIGNAL(sliderReleased()),this,SLOT(onhSLPlayersTurnVolumeReleased()));
    connect(ui->hSLActionWarningVolume, SIGNAL(sliderReleased()),this,SLOT(onhSLActionWarningVolumeReleased()));
    connect(ui->CBActionWarningPercent, SIGNAL(currentTextChanged(QString)),this,SLOT(onCBActionPercentWarningcurrentTextChanged(QString)));


    this->window()->setWindowTitle(GetPokerTJVersionString());

    ui->GameTableView->setModel(&emptyGameModel);
    ui->GameTableView->horizontalHeader()->setStretchLastSection(true);
    ui->GameTableView->setColumnWidth(GM_COL_GAMENAME,180);
    ui->GameTableView->setColumnWidth(GM_COL_PLAYERS,45);
    ui->GameTableView->setColumnWidth(GM_COL_ACTIVEPLAYERS,80);
    ui->GameTableView->setColumnWidth(GM_COL_SPOTTERS,55);
    ui->GameTableView->setColumnWidth(GM_COL_BOTS,34);
    ui->GameTableView->setColumnWidth(GM_COL_STRENGTHOFBOTS,60);
    ui->GameTableView->setColumnWidth(GM_COL_CREDIT,40);
    ui->GameTableView->setColumnWidth(GM_COL_BIGBLIND,55);
    ui->GameTableView->setColumnWidth(GM_COL_BLINDRAISE,35);

    //Set initial variables shared between laucherdialog, it's ui and launcher.
    //will later been overriden by init depending on init file entries exist
    ConnectType=LD_CONNECT_TYPE_LOCAL;
    ui->RBLocal->setChecked(!ConnectType);
    UserName=GetSystemUserName();
    ui->LEUserName->setText(UserName);
    Password="";
    NickName=GetSystemUserName();
    ui->LENickName->setText(NickName);
    Server=ui->CBServer->currentText();

    NumberofPlayers=ui->CBNumberofPlayers->currentText().toInt();
    Credit=ui->CBCredit->currentText().toInt();;
    StrenghOfBots=ui->CBStrengthofBots->currentText().toInt();
    GameName=ui->LEGameName->text();
    BigBlind=ui->CBBigBlind->currentText().toInt();
    RaiseBlindMinutes=ui->CBRaiseBlinds->currentText().toInt();

    //Flags to interact with launcher
    CreateGameName="";
    connecttoserver=0;
    //Set initial ---
}

LauncherDialog::~LauncherDialog()
{
    delete ui;
}

void LauncherDialog::SetMainWindow(MainWindow *w)
{
    this->w=w;
    if(w==NULL)
    {
     ui->TNConnectOptions->setEnabled(true);
     ui->TNGameOptions->setEnabled(true);
     ui->TNKeyBinds->setEnabled(false);
     ui->TNSound->setEnabled(false);
     return;
    }
    ui->TNConnectOptions->setEnabled(false);
    ui->TNGameOptions->setEnabled(false);
    ui->TNKeyBinds->setEnabled(true);
    ui->TNSound->setEnabled(true);

    ui->CBShowKeyBinds->setChecked(w->ShowKeyBinds);
    ui->CBActionByKey->setChecked(w->ActionByKey);
    ui->LKeyCall->setText(QKeySequence(w->Key_Call).toString());
    ui->LKeyCheck->setText(QKeySequence(w->Key_Check).toString());
    ui->LKeyFold->setText(QKeySequence(w->Key_Fold).toString());
    ui->LKeyStart->setText(QKeySequence(w->Key_Start).toString());
    ui->LKeyRaise->setText(QKeySequence(w->Key_Raise).toString());
    ui->LKeyIncBet->setText(QKeySequence(w->Key_IncBet).toString());
    ui->LKeyDecBet->setText(QKeySequence(w->Key_DecBet).toString());

    ui->CBShowMouseBinds->setChecked(w->ShowMouseBinds);
    ui->CBActionByMouse->setChecked(w->ActionByMouse);
    //ui->LMouseCall->setText(QMouse
    ui->LMouseCall->setText(GetMouseActorCaption(w->MouseActor_Call,w->MouseButton_Call,0));
    ui->LMouseCheck->setText(GetMouseActorCaption(w->MouseActor_Check,w->MouseButton_Check,0));
    ui->LMouseFold->setText(GetMouseActorCaption(w->MouseActor_Fold,w->MouseButton_Fold,0));
    ui->LMouseStart->setText(GetMouseActorCaption(w->MouseActor_Start,w->MouseButton_Start,0));
    ui->LMouseRaise->setText(GetMouseActorCaption(w->MouseActor_Raise,w->MouseButton_Raise,0));
    ui->LMouseIncBet->setText(GetMouseActorCaption(w->MouseActor_IncBet,w->MouseButton_IncBet,w->MouseWheel_IncBet));
    ui->LMouseDecBet->setText(GetMouseActorCaption(w->MouseActor_DecBet,w->MouseButton_DecBet,w->MouseWheel_DecBet));

    ui->hSLPlayersTurnVolume->setValue(w->EffectVolume[MYSOUND_PLAYERSTURN]*VOLUME_SLIDER_RANGE);
    ui->hSLActionWarningVolume->setValue(w->EffectVolume[MYSOUND_ACTIONWARNING]*VOLUME_SLIDER_RANGE);
    ui->CBActionWarningPercent->setCurrentText(QString().setNum(w->ActionWarningPercent));

}

void LauncherDialog::onhSLPlayersTurnVolumeReleased()
{
    Log("LauncherDialog::onhSLPlayersTurnVolumeReleased: vol="+QString().setNum(ui->hSLPlayersTurnVolume->value()));
    w->EffectVolume[MYSOUND_PLAYERSTURN]=ui->hSLPlayersTurnVolume->value()/VOLUME_SLIDER_RANGE;
    w->SetEffectParams(MYSOUND_PLAYERSTURN,w->EffectLoops[MYSOUND_PLAYERSTURN],w->EffectVolume[MYSOUND_PLAYERSTURN],1);
}

void LauncherDialog::onhSLActionWarningVolumeReleased()
{
    Log("LauncherDialog::onhSLActionWarningVolumeReleased: vol="+QString().setNum(ui->hSLActionWarningVolume->value()));
    w->EffectVolume[MYSOUND_ACTIONWARNING]=ui->hSLActionWarningVolume->value()/VOLUME_SLIDER_RANGE;
    w->SetEffectParams(MYSOUND_ACTIONWARNING,w->EffectLoops[MYSOUND_ACTIONWARNING],w->EffectVolume[MYSOUND_ACTIONWARNING],1);
}

void LauncherDialog::OnCBShowKeyBindsChecked()
{
    w->ShowKeyBinds=ui->CBShowKeyBinds->isChecked();
    w->RefreshTable();
}

void LauncherDialog::OnCBShowMouseBindsChecked()
{
    w->ShowMouseBinds=ui->CBShowMouseBinds->isChecked();
    w->RefreshTable();
}

void LauncherDialog::onCBActionPercentWarningcurrentTextChanged(const QString &text)
{
    w->ActionWarningPercent=text.toInt();
    Log("LauncherDialog::onCBActionPercentWarningcurrentTextChanged ActionWarningPercent now "+QString().setNum(w->ActionWarningPercent));
}

void LauncherDialog::onCBActionByKeyChecked()
{
    w->ActionByKey=ui->CBActionByKey->isChecked();
}

void LauncherDialog::onCBActionByMouseChecked()
{
    w->ActionByMouse=ui->CBActionByMouse->isChecked();
}

void LauncherDialog::Init()
{
    //ConnectOptions;
    ui->RBLocal->setChecked(!ConnectType);
    ui->RBInternetGame->setChecked(ConnectType);
    ui->LEUserName->setText(UserName);
    ui->LEPassword->setText(Password);
    ui->LENickName->setText(NickName);
    ui->CBServer->setCurrentText(Server);

    //GameOptions
    ui->CBNumberofPlayers->setCurrentText(QString().setNum(NumberofPlayers));
    ui->CBCredit->setCurrentText(QString().setNum(Credit));
    ui->CBStrengthofBots->setCurrentText(QString().setNum(StrenghOfBots));
    //ui->CBGame->setCurrentText(GameName);
    ui->CBBigBlind->setCurrentText(QString().setNum(BigBlind));
    ui->CBRaiseBlinds->setCurrentText(QString().setNum(RaiseBlindMinutes));
}

void LauncherDialog::closeEvent (QCloseEvent *event)
{
    qDebug() <<  "closeEvent(QShowEvent *event)";

    Result=LD_CANCEL;;
}

void LauncherDialog::OnRBToggled(bool checked)
{
    bool online=true;
    if(ui->RBLocal->isChecked() )
    online=false;

    ConnectType=LD_CONNECT_TYPE_LOCAL;
    if(online)
    ConnectType=LD_CONNECT_TYPE_ONLINE;

    ui->LEUserName->setEnabled(online);
    ui->LEPassword->setEnabled(online);
    ui->CBServer->setEnabled(online);
    ui->PBLogin->setEnabled(online);
    ui->LRoundTrip->setVisible(online);
    ui->PBCreateGame->setEnabled(online);
    ui->PBLeaveGame->setEnabled(online);
    ui->GameTableView->setEnabled(online);
}

void LauncherDialog::showEvent(QShowEvent *event)
{
    qDebug() <<  "showEvent(QShowEvent *event)";

    Result=LD_INIT;
    Action=LD_ACTION_INIT;
}

void LauncherDialog::onOKclicked()
{
    qDebug() <<  "LauncherDialog::onOKclicked()";

    CreateGameName=ui->LEGameName->text();
    GameName=CreateGameName;
    NumberofPlayers=ui->CBNumberofPlayers->currentText().toInt();
    Credit=ui->CBCredit->currentText().toInt();;
    StrenghOfBots=ui->CBStrengthofBots->currentText().toInt();
    NickName=ui->LENickName->text();
    BigBlind=ui->CBBigBlind->currentText().toInt();
    RaiseBlindMinutes=ui->CBRaiseBlinds->currentText().toInt();
    Result=LD_OK;
}

void LauncherDialog::OnPBCreateGameClicked()
{
    NumberofPlayers=ui->CBNumberofPlayers->currentText().toInt();
    Credit=ui->CBCredit->currentText().toInt();;
    StrenghOfBots=ui->CBStrengthofBots->currentText().toInt();
    CreateGameName=ui->LEGameName->text();
    NickName=ui->LENickName->text();
    BigBlind=ui->CBBigBlind->currentText().toInt();
    RaiseBlindMinutes=ui->CBRaiseBlinds->currentText().toInt();

    Action=LD_ACTION_CREATE_GAME;
}

void LauncherDialog::OnPBLeaveGameClicked()
{
    NumberofPlayers=ui->CBNumberofPlayers->currentText().toInt();
    Credit=ui->CBCredit->currentText().toInt();;
    StrenghOfBots=ui->CBStrengthofBots->currentText().toInt();
    CreateGameName=ui->LEGameName->text();
    NickName=ui->LENickName->text();

    Action=LD_ACTION_LEAVE_GAME;
}

void LauncherDialog::OnGameTableClicked(const QModelIndex &index)
{
    if (index.isValid())
    {
        //GameKey=index.sibling(index.row(),index.column()).data();
        GameKey=GameModel.GameKeystringList.at(index.row());
        Action=LD_ACTION_REQUEST_GAME;
    }
}

void LauncherDialog::onCancelclicked()
{
    qDebug() <<  "LauncherDialog::onCancelclicked()";

    Result=LD_CANCEL;
}

void LauncherDialog::onReloadclicked()
{
    qDebug() <<  "LauncherDialog::onReloadclicked()";

    Action=LD_ACTION_RELOAD;
}

void LauncherDialog::OnPBLoginClicked()
{
    qDebug() <<  "LauncherDialog::OnPBLoginClicked()";

    if(connecttoserver==0)
    {
        Server=ui->CBServer->currentText();
        UserName=ui->LEUserName->text();
        Password=ui->LEPassword->text();

        ui->PBLogin->setText("Logoff");
        connecttoserver=1;
    }
    else
    {
        ui->PBLogin->setText("Login");
        connecttoserver=0;
    }
}

//---------------------------------------------------

void LauncherDialog::ClearAvailableGames()
{
    /**/
    ui->GameTableView->setModel(&emptyGameModel);
    GameModel.clearLists();
}

//---------------------------------------------------

void LauncherDialog::AddAvailableGame(QString name,
                                      int Players,
                                      int ActivePlayers,
                                      int Spotters,
                                      int Bots,
                                      int Strength,
                                      int Credit,
                                      int BigBlind,
                                      int Raise,
                                      QString Key)
{

    GameModel.stringList[GM_COL_GAMENAME].append(name);
    GameModel.stringList[GM_COL_PLAYERS].append(QString().setNum(Players));
    GameModel.stringList[GM_COL_ACTIVEPLAYERS].append(QString().setNum(ActivePlayers));
    GameModel.stringList[GM_COL_SPOTTERS].append(QString().setNum(Spotters));
    GameModel.stringList[GM_COL_BOTS].append(QString().setNum(Bots));
    GameModel.stringList[GM_COL_STRENGTHOFBOTS].append(QString().setNum(Strength));
    GameModel.stringList[GM_COL_CREDIT].append(QString().setNum(Credit));
    GameModel.stringList[GM_COL_BIGBLIND].append(QString().setNum(BigBlind));
    GameModel.stringList[GM_COL_BLINDRAISE].append(QString().setNum(Raise));;
    GameModel.GameKeystringList.append(Key);
}

//---------------------------------------------------

void LauncherDialog::ShowAvailableGames(QString GameKey)
{
    GameModel.SetActiveRow(GameKey);
    ui->GameTableView->setModel(&GameModel);
}

//---------------------------------------------------

void LauncherDialog::SetRoundTrip(double roundtrip)
{
    if(roundtrip<150000)
    ui->LRoundTrip->setText(QString::number(roundtrip,'f', 2)+ " ms");
    else
    ui->LRoundTrip->setText("not connected");
}

