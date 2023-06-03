#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTextEdit>
#include <QLineEdit>
#include <QString>
#include <QGridLayout>
#include <QtGui>
#include <QPushButton>
#include <QGroupBox>
#include <QLabel>
#include <QSoundEffect>

#include "../Base/Game.h"
#include "ClickableLabel.h"

#define  REQUESTNONE          0
#define  REQUESTPLAYERSEAT    1
#define  REQUESTCONTINUE      2
#define  REQUESTEXTERNALBET   3
#define  REQUESTREBUY         4
#define  REQUESTGAMESTATS     5

#define  MAX_PARALLEL_SOUNDS  2

#define  MYSOUND_PLAYERSTURN   0
#define  MYSOUND_ACTIONWARNING 1

//#define  _DEBUG_PLAYER_SEQUENZ_

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(TGame *g, QWidget *parent = nullptr);
    ~MainWindow();

    void SetGBColor(QGroupBox *q, QColor c);
    void SetColors(void);

private:
    Ui::MainWindow *ui;

protected:
    //Top Row
    QGroupBox *playersGroupBox[2];
    QGroupBox **playerGroupBox;
    QPushButton **PBPlayer;
    ClickableLabel **LCredit;
    //2nd and 3rd Row
    QGroupBox **playerdetailGroupBox;
    QGroupBox **playercardGroupBox;
    QLabel    ***LPlayerCard;
    QLabel **LBet;

    //4th Row
    QGroupBox *potGroupBox;
    QLabel *LLPot;
    QLabel *LPot;

    QGroupBox *highestbetGroupBox;
    QLabel *LLHighestBet;
    QLabel *LHighestBet;
    //5th Row
    QGroupBox *boardcardGroupBox;
    QLabel **LBoardCard;

    QLineEdit   *LEBet;
    QPushButton *PBFold;
    QPushButton *PBCheck;
    QPushButton *PBCall;
    QPushButton *PBRaise;
    QPushButton *PBStart;
    QGroupBox   *myActionGroupBox;

    //7th Row
    QGroupBox *logviewGroupBox;
    QTextEdit *LogView;

    QTimer *MyTimer;
    TGame  *Game;

    QPixmap  PM_Card[NUM_CARDSX];
    QPixmap  PM_Back;

    void PBPlayerClicked(int player);
    void LCreditClicked(int player);

    void DisableExternalBet(void);
    QString GetActionButtonText(QString base, int key, int mouseactor, Qt::MouseButton mb, int wheeldir );

    bool eventFilter(QObject *obj, QEvent *event);//in header
    int  iBetFaktor;

    QElapsedTimer  DebugTimer;

    QSoundEffect effect[MAX_PARALLEL_SOUNDS];
    QString SoundFileName[MAX_PARALLEL_SOUNDS];
    QString BaseSoundFileName[MAX_PARALLEL_SOUNDS];
    int     GameiPlayerOld;

public:
    void RefreshTable(void);

    int  ToggleConfigVisible;

    int  iSendToServer;

    int  ActionByMouse;
    int  ActionByKey;

    int  ShowKeyBinds;
    int  Key_Esc;
    int  Key_Start;
    int  Key_Raise;
    int  Key_Check;
    int  Key_Call;
    int  Key_Fold;
    int  Key_IncBet;
    int  Key_DecBet;

    Qt::MouseButton MouseButton_Start;
    Qt::MouseButton MouseButton_Raise;
    Qt::MouseButton MouseButton_Check;
    Qt::MouseButton MouseButton_Call;
    Qt::MouseButton MouseButton_Fold;
    Qt::MouseButton MouseButton_IncBet;
    Qt::MouseButton MouseButton_DecBet;

    int ShowMouseBinds;
    int MouseActor_Start=MOUSEACTOR_BUTTON;
    int MouseActor_Raise=MOUSEACTOR_BUTTON;;
    int MouseActor_Check=MOUSEACTOR_BUTTON;
    int MouseActor_Call=MOUSEACTOR_BUTTON;
    int MouseActor_Fold=MOUSEACTOR_BUTTON;
    int MouseActor_IncBet=MOUSEACTOR_WHEEL;
    int MouseActor_DecBet=MOUSEACTOR_WHEEL;

    int MouseWheel_IncBet=1;
    int MouseWheel_DecBet=0;

    float    EffectVolume[MAX_PARALLEL_SOUNDS];
    int      EffectLoops[MAX_PARALLEL_SOUNDS];
    void     PlaySound(int sndidx);
    void     SetEffectParams(int sndix, int loops, float vol, int play);

    int      ActionWarningPercent;

public slots:
    void          SlotMyTimer();
    void          SlotPBPlayer0Clicked();
    void          SlotPBPlayer1Clicked();
    void          SlotPBPlayer2Clicked();
    void          SlotPBPlayer3Clicked();
    void          SlotPBPlayer4Clicked();
    void          SlotPBPlayer5Clicked();
    void          SlotPBPlayer6Clicked();
    void          SlotPBPlayer7Clicked();
    void          SlotPBPlayer8Clicked();
    void          SlotPBPlayer9Clicked();

    void          SlotLCredit0Clicked();
    void          SlotLCredit1Clicked();
    void          SlotLCredit2Clicked();
    void          SlotLCredit3Clicked();
    void          SlotLCredit4Clicked();
    void          SlotLCredit5Clicked();
    void          SlotLCredit6Clicked();
    void          SlotLCredit7Clicked();
    void          SlotLCredit8Clicked();
    void          SlotLCredit9Clicked();

    void          SlotPBFoldClicked();
    void          SlotPBCheckCallClicked();
    void          SlotPBRaiseClicked();
    void          SlotPBStartClicked();

    void          SlotDataReceived(QString data);

    void          on_LEBet_textChanged(const QString &text);
};
#endif // MAINWINDOW_H
