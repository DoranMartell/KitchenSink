#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <QObject>
#include <QDateTime>
#include <QCloseEvent>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <errno.h>

#include "receivingthread.h"

#define BUFFSIZE 256

namespace Ui {
class Client;
}

class Client : public QMainWindow
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = 0);
    ~Client();

    QString serverIpAddr;
    QString serverPortNumber;
    QString userName;

    struct sockaddr_in dest; /* destination address structure */

    int connectionSocket; /* socket descriptor */

    char RawMsg[BUFFSIZE]; /* message in "msgtype + username + actual_message" format */

    ReceivingThread RecvThread; /* thread to receive responses from server */

signals:

    void SendSignOnRequest(char[] , QString); /* sends login/passwrd to server for verification */

    void ReturnToStart(); /* shows ServCredentials window again */

    void LoginWindowClose(); /* closes login window if socket couldn't open */

public slots:

    void OpenSocket(QString , QString , bool &); /* passing servIp, servPortNo, third arg is result (true - success, false - fail) */

    void ShowConversationWindow(QString ); /* username in window's title */

    /* had to add as slot as LoginWindow also has to indirectly use it */
    void SendMessage(QString, QString, QString ); /* First builds, then sends */

    /* following function is a slot as it prints incoming messages from server
        as well as outgoing from client */
    void PrintMessageOnScreen(QString );

    void Disconnected(QString ); /* closes thread and socket in case Discon button was clicked
                                                         and in case server was shut down */
      /* argument - who initiated disconnect */


    void LastSeenMsgBox(QString );

private slots:

    void on_SendButton_clicked();

    void on_DissconButton_clicked();

    void on_QuitButton_clicked();

    void closeEvent(QCloseEvent * );

private:
    Ui::Client *ui;

    void BuildServerAddr();

    void BuildRawMessage(QString, QString, QString); /* builds char[] message from msgtype, username and actual message */

    /* Build helpers: */
    void Pad(QString & , int ); /* add trailing spaces so messge would satisfy format and be correctly parsed on server */

    void Trim(QString & , int ); /* cut the message as it may be longer than message buffer */
};

#endif // CLIENT_H
