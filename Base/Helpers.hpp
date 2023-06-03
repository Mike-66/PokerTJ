#ifdef extint
#undef extint
#endif
#ifdef HELPERSInitC
#define extint
#else
#define extint extern
#endif

#ifndef HELPERS_HPP
#define HELPERS_HPP

//#define POKER_LOGTOFILE
#define  POKERTJ_VERSION "PokerTJ 0.1.1"

#define  MOUSEACTOR_BUTTON 0
#define  MOUSEACTOR_WHEEL  1

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QTime>
#include <QFileInfo>
#include <QStandardPaths>
#include <QRandomGenerator>

void Log(QString LogText);
bool fileExists(QString path);
QString GetPokerTJVersionString(void);
QString GetSystemUserName(void);
QString GetConfigLocation(QString Detail,QString FileName);
QString GetRandomString(int length);
QString GetMouseActorCaption(int actor, Qt::MouseButton mb, int wheeldir);

#endif // HELPERS_HPP

extint QRandomGenerator *RG;
