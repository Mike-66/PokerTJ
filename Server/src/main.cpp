#include <QCoreApplication>
#include "launcher.hpp"


TWS_Server *WS_Server;

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    RG=new QRandomGenerator(QDateTime::currentSecsSinceEpoch());

    Launcher l;

    return a.exec();
}
