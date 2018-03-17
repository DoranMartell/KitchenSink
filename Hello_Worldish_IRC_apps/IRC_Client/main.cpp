#include "client.h"
#include "servcredentials.h"
#include "loginwindow.h"
#include "regnewwindow.h"
#include "receivingthread.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Client w;
    ServCredentials scw;
    LogInWindow lgw;
    RegNewWindow rgw;

    scw.show();
    //w.show();

    QObject::connect(&scw, &ServCredentials::ShowLoginWindow, &lgw, &LogInWindow::ShowWindow);
    //QObject::connect(&scw, &ServCredentials::OpenSocketClient, &w, &Client::OpenSocket);
    QObject::connect(&lgw, &LogInWindow::OpenSocketClient, &w, &Client::OpenSocket/*, Qt::BlockingQueuedConnection*/);
                                                            /* this is the same thread - no need for blocking queued connection */

    QObject::connect(&lgw, &LogInWindow::RegWndShow, &rgw, &RegNewWindow::Appear/*, Qt::BlockingQueuedConnection*/);
    QObject::connect(&rgw, &RegNewWindow::OpenSocketRegWind, &w, &Client::OpenSocket/*, Qt::BlockingQueuedConnection*/);
                                                            /* this is the same thread - no need for blocking queued connection */

    QObject::connect(&lgw, &LogInWindow::ShowClientWindow, &w, &Client::ShowConversationWindow);

    QObject::connect(&w, &Client::ReturnToStart, &scw, &ServCredentials::ShowServCrWindow);
    QObject::connect(&w, &Client::LoginWindowClose, &lgw, &LogInWindow::hide);
    QObject::connect(&lgw, &LogInWindow::StepBack, &scw, &ServCredentials::ShowServCrWindow);

    QObject::connect(&lgw, &LogInWindow::SendMessageClient, &w, &Client::SendMessage);
    QObject::connect(&rgw, &RegNewWindow::SendRegMessage, &w, &Client::SendMessage);

    QObject::connect(&rgw, &RegNewWindow::EnableLogInWindow, &lgw, &LogInWindow::Enable);

    QObject::connect(&w.RecvThread,&ReceivingThread::SignOnConfirmedToLogInWindow , &lgw,
                                &LogInWindow::setSignOnConfirmed, Qt::BlockingQueuedConnection);
    QObject::connect(&w.RecvThread, &ReceivingThread::FailLogIn, &lgw, &LogInWindow::ErrorLogIn);
    QObject::connect(&scw, &ServCredentials::KillRecvThread, &w.RecvThread, &ReceivingThread::setKillThread);

    QObject::connect(&w.RecvThread, &ReceivingThread::LastSeenOnlineToGUI, &w, &Client::LastSeenMsgBox, Qt::BlockingQueuedConnection);

    QObject::connect(&w.RecvThread, SIGNAL(setRegResponse(short, QString)), &rgw,
                               SLOT(setRegResponse(short, QString)), Qt::BlockingQueuedConnection);

    return a.exec();
}
