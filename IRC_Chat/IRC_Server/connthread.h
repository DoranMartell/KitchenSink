#ifndef CONNTHREAD_H
#define CONNTHREAD_H

#include <QObject>
#include <QThread>
#include <QMessageBox>
#include <QList>
#include <iterator>
#include <QDebug>

#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <errno.h>

#include "rwthread.h"

#define MAX_CONNECTIONS 10

class ConnThread : public QThread
{
    Q_OBJECT
    Q_PROPERTY(bool ConnectKilled READ ConnectKilled WRITE setConnectKilled NOTIFY ConnectKilledChanged)

    bool m_ConnectKilled;

public:

    ConnThread();

    int portNo; /* port number received from AskPort window */

    int listenSocket;

    struct sockaddr_in serveraddr; /* server's address */

    typedef struct clientsparams
    {
        int num; /* number of thread */
        RWThread ReadWriteThread;
        int connectSocket;  /* one socket for each connection */
        struct sockaddr_in clientaddr; /* client's address */
        char * clientsIP; /* dotted decimal host addr string */
        int clientsPort;
        QString clientsNick;

        int getSocket() const
        {
            qDebug() << "getSocket() = " << connectSocket;
            return connectSocket;
        }

        char * getIp() const
        {
            qDebug() << "getIp() = " << clientsIP;
            return clientsIP;
        }

        int getPort() const
        {
            qDebug() << "getPort() = " << clientsPort;
            return clientsPort;
        }

//        clientsparams() //constructor
//        {
//            this->ReadWriteThread = new RWThread();
//        }


    } ClientsParams;


    QList<ClientsParams*> Clients;

    int number_of_connections;

    void run();

    void setPort(int );

    void BuildServerAddr();

    bool ConnectKilled() const;

    void ReadWriteStart(ClientsParams ** ); /* starts new Read-Write thread */

public slots:

    void setConnectKilled(bool ConnectKilled);

    void SendToAll(QString );

    void RemoveFromList(int ); /* response to RWThread signal RemoveClientNode() */

    //void RemoveFromListAndNumConDecr(int ); /* .. and decrease number of connections */

    void setClientNick(QString , int ); /* int is for ThreadSubscript */

    void CheckClientOnline(QString , int , bool * ); /* Checks if such user has an active session already */

signals:

    void NumOfConnUpd(int , ClientsParams ** ); /* connect signals of new thread to server
                                                 and print new number of connections */

    void errMessConnThread(QString);

    void PrintMessageConnThread(QString);

    void ConnectKilledChanged(bool ConnectKilled);

    void ReadWriteSetSocket(int , int); /* pass connectionSocket value and i
                                            new number_of_connections to ReadWriteThread */

    void KillRWThread(bool ); /* is sent with 'true' when we need to kill ReadWriteThread */

    void EchoingThread(); /* echoing thread in debugging purposes */

    void ShowAskPortWindow(QString );

};

#endif // CONNTHREAD_H
