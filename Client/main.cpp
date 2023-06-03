#include "launcher.hpp"

#include <QApplication>
#include <QStyleFactory>

Launcher *l;

void timerfunc()
{
 l->Work();
 if(l->doExit==1)
 {
  l->SaveSettings();
  delete(l);
  qApp->quit();
 }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    a.setQuitOnLastWindowClosed(false);
    //QApplication::setStyle(QStyleFactory::create("WindowsVista"));
    RG=new QRandomGenerator(QDateTime::currentSecsSinceEpoch());
    l=new Launcher;
    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, timerfunc);
    timer.start(100);

    return a.exec();
}
