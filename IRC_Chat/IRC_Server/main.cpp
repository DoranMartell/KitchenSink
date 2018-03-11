#include "server.h"
#include "askport.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Server w;
    AskPort ask;

    QObject::connect(&ask, &AskPort::ShowMainWindow, &w, &Server::showWindow);
    QObject::connect(&ask, &AskPort::SetServerPort, &w.ConnectionThread, &ConnThread::setPort/*, Qt::BlockingQueuedConnection*/);
    QObject::connect(&w.ConnectionThread, &ConnThread::ShowAskPortWindow, &ask, &AskPort::ShowAskPortAgain);
    QObject::connect(&w.ConnectionThread, &ConnThread::ShowAskPortWindow, &w, &Server::CloseServerWindow);

    ask.show();
    //w.show();

    return a.exec();
}
