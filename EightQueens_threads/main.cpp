#include "queens.h"
#include "queenthread.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Queens w;
    w.show();

    QObject::connect(&w.thread, &QueenThread::print, &w, &Queens::printBoard, Qt::BlockingQueuedConnection);
    QObject::connect(&w.thread, &QueenThread::refreshOnStopOff, &w, &Queens::RefreshOnStopOff, Qt::QueuedConnection);
    QObject::connect(&w, &Queens::SpeedChange, &w.thread, &QueenThread::setSleeptime, Qt::QueuedConnection);
//    QObject::connect(&w.thread, &QueenThread::printFinal, &w, &Queens::printBoard, Qt::QueuedConnection);

    return a.exec();
}
