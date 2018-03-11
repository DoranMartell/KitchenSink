#ifndef RWTHREAD_H
#define RWTHREAD_H

#include <QObject>
#include <QThread>
#include <QDateTime>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDriver>

#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <fcntl.h>

#define BUFFSIZE 256

class RWThread : public QThread
{
    Q_OBJECT

    Q_PROPERTY(bool RWKilled READ RWKilled WRITE setRWKilled )
    bool m_RWKilled;

public:

    RWThread();


    int connectSocket; /* the same as in connection thread */

    int threadSubscript;

    void run();

    char RawMsg[BUFFSIZE]; /* message in "msgtype + username + actual_message" format */

    char buff[BUFFSIZE]; /* Raw data came from client */

    bool RWKilled() const;

    bool isOnlineAlready; /* is needed to check is such user already has active session.
                              It is changed from connection thread, slot CheckOnline() */

    QSqlDatabase db;

    void UpdateLastOnline(QString ); /* updates value of LastSeenOnline in database
                                        for user in function's argument */

private:

    /* splits message into msgtype, datetime, user who sent it and actual message/password */
    void ParseMessage(QString & , QString & , QString & , QString & );

    /* compose from msgtype, user and actual message a whole string to be sent via write() */
    void BuildRawMessage(QString , QString, QString );

    /* Build helpers: */
    void Pad(QString & , int ); /* add trailing spaces so messge would satisfy format and be correctly parsed on server */

    void Trim(QString & , int ); /* cut the message as it may be longer than message buffer */

    /* checks username and password of client who is trying to connect:
            results:
                    1 - success
                    -1 - failed: wrong password, user exists in Database,
                    -2 - failed: no such user in Database,
                    -3 - failed: database error                         */
    int CheckInDB(QString InputUname, QString InputPass);

    int InsertToDB(QString SuggestedUname, QString SuggestedPass); /* 1 - success, -3 - database error */

    QString LastSeenOnline;

    QString connection_name; /* Name of connection to database, random each time */

public slots:

    void setConnSocket(int, int ); /* receives value of ConnectSocket from connection thread
                                        and values of current thread subscript in ReadWriteThread[] array*/

    void setRWKilled(bool RWKilled);

    void EchoReflected(); /* Thread responses on ConnThread echo */

signals:

    void errMessRWThread(QString ); /* signal to Server class */

    void PrintMessageRWThread(QString); /* signal to Server to print a message */

    void SendToAllClients(QString ); /* signal to ConnThread */

    void RemoveClientNode(int ); /* signal to ConnThread: */
                                 /* remove node with such thread number from list of opened connections */
                                 /* should change number of connections also */

    void DecreaseNumberOfConnections();

    void AssignNickToClient(QString , int ); /* once received sign-on - pass username to ConnThread (to verify if it is connected already) */

    void CheckOnline(QString , int, bool * ); /* sends username to connection thread to check if such user already connected */

};

#endif // RWTHREAD_H
