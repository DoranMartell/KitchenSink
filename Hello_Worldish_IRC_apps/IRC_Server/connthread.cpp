#include "connthread.h"
#include "rwthread.h"

#include <QDebug>

ConnThread::ConnThread()
{
    //setConnectKilled(false);
    //number_of_connections = 0;
    //emit NumOfConnUpd(number_of_connections, NULL);
    //memset(buff, '\0', BUFFSIZE);
}

/*--------------------------------------------------------------------------------------------------------------*/
void ConnThread::run()
{
    /* only once the connection thread just started */
    setConnectKilled(false);
    number_of_connections = 0;
    emit NumOfConnUpd(number_of_connections, NULL);

    /* socket: create a socket */
    listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket < 0)
    {
        errMessConnThread("Error opening listening socket!");
    }

    BuildServerAddr();

    /* bind: associate the listening socket with a port */
    if ( bind(listenSocket, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0)
    {
        /* proper handling of returned value */
            QString err_string("Error " + QString::number(errno) + ": " + QString(strerror(errno)));
            emit ShowAskPortWindow(err_string);
            return;
       // errMessConnThread("Error binding listening socket!");

    }

    /* listen: make it a listening socket ready to accept connection requests */
    if (listen(listenSocket, MAX_CONNECTIONS) < 0)
    {
        errMessConnThread("Error listening!");
    }

    int n = 0; /* ever increasing unique thread/ClientsParam structure indentifier */

    while (1)
    {       
            ClientsParams* temp = new ClientsParams();

            socklen_t clientlen;
            clientlen = sizeof(/*temp->clientaddr*/ struct sockaddr_in);

            /* accept: wait for a connection request */
            if ((temp->connectSocket = accept(listenSocket, (struct sockaddr *) & temp->clientaddr, &clientlen)) < 0)
            {
                 qDebug() << "Listening socket " << listenSocket << " was closed - accept() returned -1!";
                 QString err_string("Error " + QString::number(errno) + ": " + QString(strerror(errno)));
                 qDebug() << err_string;
                 qDebug() << "If it was us who shut the socket - we're closing application - no error then";
                 return; /* anyway most probably we pressed 'Shut Down' so return is fine */
            }

            /* Structure node's ID */
            temp->num = n++;

            qDebug() << "Accept from connection socket" << temp->num << "with descriptor:" << temp->connectSocket;

            temp->clientsIP = inet_ntoa(temp->clientaddr.sin_addr);

            temp->clientsPort = ntohs(temp->clientaddr.sin_port);
            emit PrintMessageConnThread("Server established connection with " + QString(temp->clientsIP) +
                          ":" + QString::number(static_cast<int>(temp->clientsPort)));

            qDebug() << "About to push all new connection info to list of connections and start client's thread...";
            Clients.push_back(temp);

            ReadWriteStart(&temp);

            /* Had to use list of pointer to objects instead of objects*/
            //QList<ClientsParams*>::iterator i;
            //ClientsParams * current;

            /*for debugging only*/
            //qDebug() << "Socket descriptors currently in the list:";
            //for (i = Clients.begin(); i != Clients.end(); i++)
            //{
            //    current = *i;
            //    qDebug() << current->getSocket();
            //}
            //qDebug() << "That's all for sockets";

            qDebug() << "Number of connections: " << number_of_connections;
            /* send this number of connections to GUI thread for it to change it in statusBar */


        /* Get There if all sockets are going to be closed
            and all threads are going to be killed - if we pressed ShutDown */
        if (ConnectKilled())
        {
            QList<ClientsParams*>::iterator i;
            ClientsParams * current;

            /* first close socket, then kill thread
              otherwise read() will still be running */
            i = Clients.begin();
            while (!Clients.isEmpty())
            {
                current = *i;
                //RemoveFromListAndNumConDecr(current->ReadWriteThread.threadSubscript);
                RemoveFromList(current->ReadWriteThread.threadSubscript);
                emit NumOfConnUpd(number_of_connections, NULL);
                i++;
            }


//            for (i = Clients.begin(); i != Clients.end(); i++)
//            {
//                current = *i;
//                ::shutdown(current->connectSocket, SHUT_RDWR); /* second argument - no more receiving and sending both */
//                ::close(current->connectSocket);
//                qDebug() << "Sending signal to thread" << current->num << "to die....";
//                //emit KillRWThread(true);
//                current->ReadWriteThread.setRWKilled(true);
//                current->ReadWriteThread.quit();
//                number_of_connections--;
//                emit NumOfConnUpd(number_of_connections, NULL);
//                RemoveFromList(current->ReadWriteThread.threadSubscript);

//            }

            qDebug() << "Connection thread is going to die";
            return; /* Thread is going to die */
        }

//        /* for debuggin purposes only */
//        char str1[10];
//        unsigned short sh2;
//        for (i = Clients.begin(); i != Clients.end(); i++)
//        {
//            current = *i;
//            qDebug() << "Socket" << current->num << "is" << current->connectSocket;
//            /* converting address in_addr to string */

//            inet_ntop(AF_INET, &(current->clientaddr.sin_addr), str1, INET_ADDRSTRLEN);
//            sh2 = ntohs(current->clientaddr.sin_port);

//            qDebug() << "Client addr " << QString::fromStdString(str1)
//                        << ":" << QString::number(sh2);
//        }

    }
}

/*--------------------------------------------------------------------------------------------------------------*/
void ConnThread::CheckClientOnline(QString uname, int thrNum, bool * isOnlineAlready) /* if you check the nickname, then this check should be in ReadWriteThread */
{
    QList<ClientsParams *>::iterator i;
    ClientsParams * current;

    qDebug() << "Checking if such user has an active session already:";
    qDebug() << "Is" << uname << "online now? -"<< *isOnlineAlready;

    for (i = Clients.begin(); i != Clients.end(); i++)
    {
        current = *i;

        qDebug() << "User name trying to connect:" << uname;
        qDebug() << "Another user name fetched from list of connections" << current->clientsNick;

        if (current->clientsNick == uname && current->num != thrNum)
        {
            QString msg = "User " + uname + " is online already!";
            *isOnlineAlready = true;

            qDebug() << "Matched! Change value to" << *isOnlineAlready;

            //emit PrintMessageConnThread(msg); /* commented out as caused DeadLock */

            return;
        }

    }

    qDebug() << "Did not match any username in active list. Value = " << *isOnlineAlready;

    *isOnlineAlready = false;

}

/*--------------------------------------------------------------------------------------------------------------*/
void ConnThread::setPort(int value)
{
    portNo = value;
}
/*--------------------------------------------------------------------------------------------------------------*/
void ConnThread::BuildServerAddr()
{
    /* build the server's internet address */
    memset((char *) &serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET; /* we are using the Internet */
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); /* accept reqs to any IP addr */
    serveraddr.sin_port = htons((unsigned short)portNo); /* port to listen on */

}
/*--------------------------------------------------------------------------------------------------------------*/
void ConnThread::SendToAll(QString Msg)
{
    int n;
    //char * buff = Msg.toLatin1().data();
                        /* it worked this way previously. where does this pointer points
                                                            and why did it change? */
    //qDebug() << "What's in buff? " << QString(buff);

    ClientsParams *current;
    QList<ClientsParams*>::iterator i;
    for (i = Clients.begin() ; i != Clients.end(); i++)
    {
        current = *i;
        if ( (n = write(current->getSocket(), /* buff */ Msg.toLatin1().data(), BUFFSIZE)) < 0)
        {
            qDebug() << "Error writing to socket " << current->getSocket();
        }
        qDebug() << "SendToAll(): sending to thread " << current->ReadWriteThread.threadSubscript
                        << " message " << /*QString(buff)*/ QString(Msg.toLatin1().data());
    }
}

/*--------------------------------------------------------------------------------------------------------------*/
bool ConnThread::ConnectKilled() const
{
    return m_ConnectKilled;
}

/*--------------------------------------------------------------------------------------------------------------*/
void ConnThread::setConnectKilled(bool ConnectKilled)
{
    if (m_ConnectKilled == ConnectKilled)
        return;

    m_ConnectKilled = ConnectKilled;
    emit ConnectKilledChanged(ConnectKilled);
}

/*--------------------------------------------------------------------------------------------------------------*/
void ConnThread::ReadWriteStart(ClientsParams ** ThreadToStart)
{
    qDebug() << "Received incoming conection!";

    if (number_of_connections < MAX_CONNECTIONS)
    {
        /* if we pass a pointer to Thread'N'Socket structure,
                          no need of this nonsense below: */
//        QList<ClientsParams*>::iterator i = Clients.end();
//        ClientsParams *ThreadToStart = new ClientsParams();
//        ThreadToStart = *i;

        QObject::connect(&(*ThreadToStart)->ReadWriteThread, &RWThread::SendToAllClients, this, &ConnThread::SendToAll/*, Qt::BlockingQueuedConnection*/);

        QObject::connect(&(*ThreadToStart)->ReadWriteThread, &RWThread::RemoveClientNode, this, &ConnThread::RemoveFromList, Qt::/*Blocking*/QueuedConnection);
                                                                                                     /* not blocking as otherwise it will never go to check if KillThread is true */
        //QObject::connect(&(*ThreadToStart)->ReadWriteThread, &RWThread::RemoveClientNode, this, &ConnThread::RemoveFromListAndNumConDecr, Qt::/*Blocking*/QueuedConnection);
                                                                                                     /* not blocking as otherwise it will never go to check if KillThread is true */
        QObject::connect(&(*ThreadToStart)->ReadWriteThread, &RWThread::CheckOnline, this, &ConnThread::CheckClientOnline, Qt::BlockingQueuedConnection/* */);

        QObject::connect(&(*ThreadToStart)->ReadWriteThread, &RWThread::AssignNickToClient, this, &ConnThread::setClientNick, Qt::BlockingQueuedConnection/* */);

        emit PrintMessageConnThread("Connection established with socket [" +
                         QString::number((*ThreadToStart)->getSocket()) + "]!");

        qDebug() << "Setting for " << (*ThreadToStart)->getSocket() << " socket, node's ID: " << (*ThreadToStart)->num;
        qDebug() << "Number of connection (doesn't necessarily match with highest Node's ID) " << number_of_connections;

        (*ThreadToStart)->ReadWriteThread.setConnSocket((*ThreadToStart)->getSocket(), (*ThreadToStart)->num);

        (*ThreadToStart)->ReadWriteThread.setRWKilled(false);

        emit NumOfConnUpd(++number_of_connections, ThreadToStart);
        /* first signals connecting in NumOfConnUpd(), then thread starts, or we skip some message prinitng as signals and slots not connected yet */

        qDebug() << "Starting " << (*ThreadToStart)->num << " RWThread!" /*<< (*ThreadToStart)->ReadWriteThread.threadSubscript*/;
        (*ThreadToStart)->ReadWriteThread.start();

        /* echoing threads in debugging purposes */
//        QObject::connect(this, &ConnThread::EchoingThread, &ReadWriteThread[number_of_connections-1],
//                                                  &RWThread::EchoReflected, Qt::BlockingQueuedConnection);
        //for (int i = 0; i < number_of_connections; i++)
       // {
       //     qDebug() << "Echoing thread" << i;
//            emit EchoingThread();
       // }
        /* one signal to multiple slots seems to fail */

    }
    else
    {
        errMessConnThread("Can't receive any more connections!");
        emit PrintMessageConnThread("Somebody tried to establish connection, but server reached its maximum of active connections!");
    }
}
/*--------------------------------------------------------------------------------------------------------------*/
void ConnThread::RemoveFromList(int threadNum)
{
    QList<ClientsParams *>::iterator i;
    ClientsParams * current;

    for (i = Clients.begin(); i != Clients.end(); i++)
    {
         current = *i;

        if ( current->num == threadNum)
        {
            if (!current->clientsNick.isEmpty())
            {
                qDebug() << "Update last-seen-online value before shutting down a thread for" << current->clientsNick;
                /* Update LastSeenOnline value in Database for this user */
                current->ReadWriteThread.UpdateLastOnline(current->clientsNick);
            }

            qDebug() << "Shutdown and close thread " << current->ReadWriteThread.threadSubscript;
            ::shutdown(current->connectSocket, SHUT_RDWR); /* second argument - no more receiving and sending both */
            ::close(current->connectSocket);
            qDebug() << "Killing thread" << current->ReadWriteThread.threadSubscript << "....";
            current->ReadWriteThread.setRWKilled(true);
            while (current->ReadWriteThread.isRunning())
            {
                usleep(500); //wait till it's killed
                qDebug() << "Waiting for" << current->ReadWriteThread.threadSubscript << "thread to die";
            }
            current->ReadWriteThread.quit();


            /* Client is disconnecting - decrease number of connections and notify GUI thread ... */

            number_of_connections--;
            qDebug() << "Server to change number_of_connections" << number_of_connections;

            /* commented as caused deadlock */
            //emit NumOfConnUpd(number_of_connections, NULL);
                /* removed ouside of here: sequence of signal-slot-slot is too long
                                and number of involved threads is too big */

            /* ... and delete client's structure from the list */
            Clients.erase(i);


            /* Need to check all list in case this structure was not erased */
            qDebug() << "Making sure that we removed correct client. Let's go through clients remained in the list";
            for (i = Clients.begin() ; i != Clients.end(); i++)
            {
                current = *i;
                qDebug() << "Checking thread " << current->num << "...";

                qDebug() << "Client's name is " << current->clientsNick ;
            }


            qDebug() << "ClientParams node related to " << threadNum << " thread was erased from the list of connections";
            break; /* presume there couldn't possibly be two same thread numbers */
        }
    }
}

/*--------------------------------------------------------------------------------------------------------------*/
//void ConnThread::RemoveFromListAndNumConDecr(int threadNum)
//{
//    RemoveFromList(threadNum);
//    emit NumOfConnUpd(--number_of_connections, NULL);
//}
/*--------------------------------------------------------------------------------------------------------------*/
void ConnThread::setClientNick(QString uname, int thrNumber)
{
    ClientsParams *current;
    QList<ClientsParams*>::iterator i;

    qDebug() << "Going to set " << uname << " as client's nickname for thread " << thrNumber ;

    for (i = Clients.begin() ; i != Clients.end(); i++)
    {
        current = *i;
        qDebug() << "Checking thread " << current->num << "...";

        if ( current->num == thrNumber  )
        {
            current->clientsNick = uname;
            qDebug() << "Client nickname for thread " << current->num << "is set to " << current->clientsNick ;
        }
    }
}
/*--------------------------------------------------------------------------------------------------------------*/
