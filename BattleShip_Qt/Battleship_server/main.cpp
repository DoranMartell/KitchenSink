#include "server.h"
#include "askport.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    server serv;
    AskPort ask;

    QObject::connect(&ask, &AskPort::setPortNumSig, &serv, &server::setPortNumSlot/*, Qt::BlockingQueuedConnection*/);
    QObject::connect(&ask, &AskPort::startServAndShowSig, &serv, &server::startServAndShowSlot );

    ask.show();
    /* w.show(); */

    return a.exec();
}
