#define HELPERSInitC

#include "Helpers.hpp"
#include <QDebug>
//-----------------------------------------------------------
void Log(QString LogText)
{
  QString msg=QDateTime::currentDateTime().toString() +" "+LogText;
  qDebug() << msg;
#ifdef POKER_LOGTOFILE
  const QString qPath("PokerTJ.log");
  QFile qFile(qPath);
  if (qFile.open    (QIODevice::Append | QIODevice::WriteOnly)) {
    QTextStream out(&qFile);
    out <<  msg << endl;
    qFile.close();
  }
#endif
}
//-----------------------------------------------------------
QString GetPokerTJVersionString(void)
{
 return(POKERTJ_VERSION);
}
//-----------------------------------------------------------
QString GetConfigLocation(QString Detail, QString FileName)
{
 QString ConfigLocation=QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
 if(Detail.length()>0)
     ConfigLocation+="/"+Detail;
 if(FileName.length()>0)
     ConfigLocation+="/"+FileName;
 Log("Requested ConfigLocation: Detail="+Detail+", FileName="+FileName+" -> "+ConfigLocation);
 return(ConfigLocation);
}
//-----------------------------------------------------------
bool fileExists(QString path) {
    QFileInfo check_file(path);
    // check if path exists and if yes: Is it really a file and no directory?
    return check_file.exists() && check_file.isFile();
}
//-----------------------------------------------------------
QString GetSystemUserName(void)
{
    QString UserName=QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    Log("UserName Raw: "+UserName);
    int idx=UserName.lastIndexOf("/");
    Log("UserName Index: "+QString().setNum(idx));
    UserName=UserName.right(idx-2);
    Log("UserName Indexed: "+UserName);
    return(UserName);
}

//---------------------------------------------

QString GetRandomString(int length)
{
   const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");

   QString randomString;
   for(int i=0; i<length; ++i)
   {
       int index = RG->bounded(0,possibleCharacters.length());
       QChar nextChar = possibleCharacters.at(index);
       randomString.append(nextChar);
   }
   return randomString;
}

//----------------------------------------------

QString GetMouseActorCaption(int actor, Qt::MouseButton mb, int wheeldir)
{
 if(actor==MOUSEACTOR_BUTTON)
 {
  if ( (mb & Qt::AllButtons) == Qt::AllButtons )
      return("All");
  else if ( (mb & Qt::LeftButton) == Qt::LeftButton )
      return("Left");
  else if ( (mb & Qt::RightButton) == Qt::RightButton )
      return("Right");
  else if ( (mb & Qt::MiddleButton) == Qt::MiddleButton )
      return("Middle");
  else if ( (mb & Qt::BackButton) == Qt::BackButton )
      return("Back");
  else if ((mb & Qt::XButton1) == Qt::XButton1 )
      return("X1");
  else if ( (mb & Qt::ExtraButton1) == Qt::ExtraButton1 )
      return("Extra");
  else if ( (mb & Qt::ForwardButton) == Qt::ForwardButton )
      return("Forward");
  else if ( (mb & Qt::XButton2) == Qt::XButton2 )
      return("X2");
  else if ( (mb & Qt::ExtraButton2) == Qt::ExtraButton2 )
      return("Extra2");
  else if ( (mb & Qt::TaskButton) == Qt::TaskButton )
      return("Task");
  else if ( (mb & Qt::ExtraButton3) == Qt::ExtraButton3 )
      return("TaskButton");
  else if ( (mb & Qt::ExtraButton4) == Qt::ExtraButton4 )
      return("Extra4");
  else if ( (mb & Qt::ExtraButton5) == Qt::ExtraButton5 )
      return("Extra5");
  else if ( (mb & Qt::ExtraButton6) == Qt::ExtraButton6 )
      return("Extr6");
  else if ( (mb & Qt::ExtraButton7) == Qt::ExtraButton7 )
      return("Extra7");
  else if ( (mb & Qt::ExtraButton8) == Qt::ExtraButton8 )
      return("Extra8");
  else if ( (mb & Qt::ExtraButton9)== Qt::ExtraButton9 )
      return("Extra9");
  else if ( (mb & Qt::ExtraButton10) == Qt::ExtraButton10 )
      return("Extra10");
  else if ( (mb & Qt::ExtraButton11) == Qt::ExtraButton11 )
      return("Extra11");
  else if ( (mb & Qt::ExtraButton12) == Qt::ExtraButton12 )
      return("Extra12");
  else if ( (mb & Qt::ExtraButton13) == Qt::ExtraButton13 )
      return("Extra13");
  else if ( (mb & Qt::ExtraButton14) == Qt::ExtraButton14 )
      return("Extra14");
  else if ( (mb & Qt::ExtraButton15) == Qt::ExtraButton15 )
      return("Extra15");
  else if ( (mb & Qt::ExtraButton16) == Qt::ExtraButton16 )
      return("Extra16");
  else if ( (mb & Qt::ExtraButton17) == Qt::ExtraButton17 )
      return("Extra16");
  else if ( (mb & Qt::ExtraButton18) == Qt::ExtraButton18 )
      return("Extra18");
  else if ( (mb & Qt::ExtraButton19) == Qt::ExtraButton19 )
      return("Extra19");
  else if ( (mb & Qt::ExtraButton20) == Qt::ExtraButton20 )
      return("Extra20");
  else if ( (mb & Qt::ExtraButton21) == Qt::ExtraButton21 )
      return("Extra21");
  else if ( (mb & Qt::ExtraButton22) == Qt::ExtraButton22 )
      return("Extra22");
  else if ( (mb & Qt::ExtraButton23) == Qt::ExtraButton23 )
      return("Extra23");
  else if ( (mb & Qt::ExtraButton24) == Qt::ExtraButton24 )
      return("Extra24");
  else if ( (mb & Qt::NoButton) )
      return("NoButton");

  return("-----");
 }
 else if ( actor == MOUSEACTOR_WHEEL )
 {
     if ( wheeldir == 1 )
         return("Wheel Up");
     else if ( wheeldir == 0 )
         return("Wheel Down");
     else
        return("-----");
 }


}


