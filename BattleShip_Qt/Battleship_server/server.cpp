#include "server.h"
#include "ui_server.h"
/*----------------------------------------------------------------------------------*/
server::server(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::server)
{
    ui->setupUi(this);

    /* Had to put it here so each Client thread could send a signal to GUI thread */
    connect(this, &server::WriteToLogSig, this, &server::WriteToLogSlot); // need to connect only ONCE!
    connect(this, &server::SetPlayerNameOnWindowSig, this, &server::SetPlayerNameOnWindowSlot);
    connect(this, &server::CloseConnectionSig, this, &server::CloseConnectionSlot/*, Qt::BlockingQueuedConnection*/);
                                                          /* had to do this to make sure connection is closed from main thread */
    connect(this, &server::JoinThreadSig, this, &server::JoinThreadSlot);

    connect(this, &server::ChangeColorSig, this, &server::ChangeColorSlot, Qt::BlockingQueuedConnection);

    connect(this, &server::FillShipsSig, this, &server::FillShipsSlot);

    connect(this, &server::ClearGridsButNotConnectionsSig, this, &server::ClearGridsButNotConnectionsSlot);

    /* do we even need this signal-slot couple?? */
    connect(this, &server::SendToAllSig, this, &server::SendToAllSlot);

    mu.lock();
    ServerInitiatedShutdown = false;
    mu.unlock();

    SetClosedValue(false);

    NameFleetMap.clear();

    ClearShipKilled();

    LinkCoordinatesAndActualSquares();

    SetPendingColor("");

}
/*----------------------------------------------------------------------------------*/
void server::startServAndShowSlot()
{
    show();

    QString MainThreadId = GetQStrThreadId(std::this_thread::get_id());
    qDebug() << "Main Thread Id = " << MainThreadId;

    /* Pushing associated pair of socket and thread, accepting new connection requests */
    std::thread * th = new std::thread(&server::HandleConnectionRequest,this);
    ThreadSafeInsertToMap(listen_sock, th);

}
/*----------------------------------------------------------------------------------*/
void server::setPortNumSlot(QString PortString)
{
    BuildServerAddr(PortString.toInt());
}
/*----------------------------------------------------------------------------------*/
void server::BuildServerAddr(int Port)
{
    ServerDetails.sin_family = AF_INET;
    ServerDetails.sin_addr.s_addr = htonl(INADDR_ANY);
    ServerDetails.sin_port = htons( (unsigned short) Port);

    CreatSockAndBindToAddr();
}
/*----------------------------------------------------------------------------------*/
int server::CreatSockAndBindToAddr()
{
    shouldHideAskWindow = 1;
    SetClientsConnected(0);
    SetClientsFieldFilled(0);

    if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        ShowErrDescription(errno);
        shouldHideAskWindow = 0;
        return -1;
    }

    /* setting SO_REUSEADDR so we could stop listening and then start it again */
    int enable = 1;
    if (setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    {
        ShowErrDescription(errno);
        shouldHideAskWindow = 0;
        return -2;
    }

    if (bind(listen_sock, (struct sockaddr *) &ServerDetails, sizeof(ServerDetails)) < 0)
    {
        ShowErrDescription(errno);
        shouldHideAskWindow = 0;
        return -3;
    }

    if (listen(listen_sock, 2) < 0)
    {
        ShowErrDescription(errno);
        shouldHideAskWindow = 0;
        return -4;
    }

    qDebug() << "Created socket" << listen_sock << "to handle incoming connection requests";

    FILE *in;
    char host_addr_buff[50];
    in = popen("hostname -I", "r");
    fgets(host_addr_buff, sizeof(host_addr_buff), in);
    pclose(in);
    QString IpAddr = QString(host_addr_buff);

    QString title = "BattleShip Server: " + /*QString(inet_ntoa(ServerDetails.sin_addr))*/ IpAddr.trimmed() + ":"
                                           +  QString::number(ntohs(ServerDetails.sin_port));
    setWindowTitle(title);

    return 0;
}
/*----------------------------------------------------------------------------------*/
void server::HandleConnectionRequest()
{
    QString ListenThreadId = GetQStrThreadId(std::this_thread::get_id());
    qDebug() << "Listening Thread Id = " << ListenThreadId;

    ClientDetails NewClient;

    /* non-blocking accept() */
    //fcntl(listen_sock, F_SETFL, O_NONBLOCK);

    qDebug() << "Server is ready to accept incoming connection requests";

    /* this is not filled by accept()!! length value should be passed to accept()! */
    NewClient.ClientLen = sizeof(struct sockaddr_in);

    while(1)
    {
        if ( GetClientsConnected() < 2)
        {
            if ((NewClient.client_socket =
                   accept(listen_sock, (struct sockaddr *) &NewClient.ClientAddress,
                                    &NewClient.ClientLen)) < 0)
            {
                /* non-blocking accept() didn't receive connection request - sleep and try again */
                //usleep(5000);

                qDebug() << "accept() returned errno" << errno << ": " << QString(strerror(errno));
                return;

            }
            else
            {
                /* accept() received connection request from client:
                     fill ClientDetails class for this particular client,
                     create a separate thread where we will handle messages from and to this client */
                NewClient.ClientIP = QString(inet_ntoa(NewClient.ClientAddress.sin_addr));
                NewClient.ClientPort = ntohs(NewClient.ClientAddress.sin_port);
                NewClient.ClientName = QString();
                NewClient.MessagesReceived = 0;

                /* non-blocking recvfrom() */
                //fcntl(NewClient.client_socket, F_SETFL, O_NONBLOCK);

                /* Mind syntax of threads by member-functions with arguments!!!!*/
                std::thread * th = new std::thread(&server::HandleClient, this, NewClient);
                ThreadSafeInsertToMap(NewClient.client_socket, th);

                /* Increase ClientsConnected value */
                SetClientsConnected(GetClientsConnected() + 1);

                if (GetClientsConnected() == 2)
                {
                    qDebug() << "Both Players connected, server to cease waiting for new connections";
                    qDebug() << "Shutdown listening socket";
                    ::shutdown(listen_sock, SHUT_RDWR);
                }
            }
        }
        else
        {
            /* Two clients are enough to play BattleShip game */
            usleep(5000);

            /* what if socket is shut down but we want to remove the thread?
                    we should leave the function for listening thread to be joined.... */
            if (GetClosedValue() == true)
            {
                qDebug() << "Listening thread is going to be killed...";
                return;
            }
        }
    }
}
/*----------------------------------------------------------------------------------*/
void server::HandleClient(ClientDetails Client)
{    
    QString HandleClientThreadId = GetQStrThreadId(std::this_thread::get_id());
    qDebug() << "Client Thread Id = " << HandleClientThreadId;

    int n;
    char buff[BUFFSIZE];
    memset(buff, '\0', BUFFSIZE);

    while (1)
    {

        n = recvfrom(Client.client_socket, buff, BUFFSIZE, 0, (struct sockaddr *) &Client.ClientAddress, &Client.ClientLen);
        if (n </*=*/ 0)
        {
            /* We are here as recvfrom() was set to non-blocking mode, which is suppossed to happen once
                    the thread is going to be joined */

            /* no need to get here if server initiated shutdown, CloseConnection() is called at close_event handler */
            if (!ServerInitiatedShutdown)
            {
                WriteToLogWindow("Apparently, player \""+ Client.ClientName + "\" has unexpectedly fled!");
                emit CloseConnectionSig(Client.client_socket, Client.ClientName, true/*, GetQStrThreadId(std::this_thread::get_id())*/);
            }
            return;
        }
        else if (n == 0) /* need to think when else such case is possible */
        {
            /* no need to get here if server initiated shutdown, CloseConnection() is called at close_event handler */
            if (!ServerInitiatedShutdown)
            {
                WriteToLogWindow("Apparently, player \""+ Client.ClientName + "\" has unexpectedly fled!");

                /* first emit the signal, then return from function, as if it is not done, thread would not be joined */
                emit CloseConnectionSig(Client.client_socket, Client.ClientName, true/*, GetQStrThreadId(std::this_thread::get_id())*/);
            }

            return;
        }
        else
        {
            qDebug() << "Socket" << Client.client_socket << "received following message:" << buff;

            QString Message(buff);

            /* if the first message has a header other than "name", connection should be dropped immediately and thread joined */
            if (ParseMessage(Client, Message) == -999 ) /* ParseMessage() returns -999 when it received BBYE message
                                                                meaning Client, who sent it, is about to disconect*/
            {
                WriteToLogWindow("Apparently, player \""+ Client.ClientName + "\" has cowardly fled!");

                /* that should be done by some other thread I think, as CloseConnection is called from the same thread
                                which is going to be killed */
                emit CloseConnectionSig(Client.client_socket, Client.ClientName, true/*, GetQStrThreadId(std::this_thread::get_id())*/);

                /* Hardcoded */
                /* even if the victorious player decided to stay, anyway he/she will need to place ships again */
                //SetClientsFieldFilled(0);
                /* if the victorious player decided to leave, CloseConnection() will decrease ClientsConnected value to 0 */
                //SetClientsConnected(1);

                /* Grids need to be cleared in case new player arrive instead of fled one*/
                /**/ emit ClearGridsButNotConnectionsSig();

                //qDebug() << "Open listening socket for listening once again as one of clients has fled";
                //listen(listen_sock, 2);

                return;
            }

            memset(buff, '\0', BUFFSIZE);

        }

    }
}

/*----------------------------------------------------------------------------------*/
void server::WriteToLogWindow(QString Text)
{
    /* Had to split the function to signal-slot as apparently you
            cannot modify GUI objects from any thead but the main one */
    emit WriteToLogSig(Text);

}
/*----------------------------------------------------------------------------------*/
void server::WriteToLogSlot(QString Text)
{
    QDateTime dt = QDateTime::currentDateTime();
    QString date = dt.toString("dd-MMM-yyyy hh:mm:ss");

    //mu.lock();

    ui->LogWindow->setText(ui->LogWindow->toPlainText() + date + " " + Text + "\n");

    //mu.unlock();

    qDebug() << "Message Printed: " << date + " " + Text;
    return;
}
/*----------------------------------------------------------------------------------*/
void server::SetPlayerNameOnWindowSlot(QString Name, int Sock)
{
    mu.lock();

    if (!nameSet[0])
    {
        ui->Player1Box->setTitle(Name);  

        nameSet[0] = Sock;

        RedPlayerFleet.ClearAll();
        RedPlayerName = Name;
        NameFleetMap.insert(std::pair<QString, Fleet>(Name, RedPlayerFleet));
        RedPlayerGrid.WhitenAllSquares();
        NameGridMap.insert(std::pair<QString, Grid>(Name, RedPlayerGrid));
    }
    else if (!nameSet[1])
    {
        ui->Player2Box->setTitle(Name);

        nameSet[1] = Sock;

        BluePlayerFleet.ClearAll();
        BluePlayerName = Name;
        NameFleetMap.insert(std::pair<QString, Fleet>(Name, BluePlayerFleet));
        BluePlayerGrid.WhitenAllSquares();
        NameGridMap.insert(std::pair<QString, Grid>(Name, BluePlayerGrid));
    }

    mu.unlock();
}
/*----------------------------------------------------------------------------------*/
void server::CloseConnectionSlot(int Socket, QString ClientName, bool EraseFromMap/*, QString ThreadId*/)
{
    qDebug() << "CloseConnection() is called from thread" << GetQStrThreadId(std::this_thread::get_id());

    qDebug() << "CloseConnection for socket" << Socket /*<< "thread" << ThreadId*/;

    /* close connection to client associated with Socket and also kill associated thread */

    if (!EraseFromMap)
    {
        mu.unlock();
    }

    if (GetClientsConnected() > 0)
        SetClientsConnected(GetClientsConnected() - 1);

    if (GetClientsFieldFilled() > 0)
        SetClientsFieldFilled(GetClientsFieldFilled() - 1);

    if (!EraseFromMap)
    {
        mu.lock();
    }

    /* turning all recvfrom() and accept() to non-blocking mode so threads could join */
    //qDebug() << "Setting socket" << Socket << "to a Non-Blocking mode";
    //fcntl(Socket, F_SETFL, O_NONBLOCK);

    /* Seems that INSTEAD of setting accept() and recvfrom() to a NON-BLOCKING mode
         (which might be impossible to do from another thread while accept() is waiting in another),
         we should shutdown() assossiated file-descriptor */
    if (GetClientsConnected() != 1 || Socket != listen_sock)
    {
        qDebug() << "Shutting down socket" << Socket << "...";
        if (::shutdown(Socket, SHUT_RDWR) != 0)
        {
            qDebug() << "ERROR while shutting down socket" << Socket << QString(strerror(errno));
        }
    }
    else
    {
        /* make listening thread (which is currnetly in sleep as doesn't expect any new connections)
            to know that server is shutting down */
        SetClosedValue(true);
    }

    /* let the current thread release mutex so slot from other thread could access same shared data */
    if (!EraseFromMap) /* mutex is locked if we are not erasing records from Socket-Thread map, the case when server app is closed */
    {
        mu.unlock();
    }

    //emit JoinThreadSig(Socket, EraseFromMap /*ThreadId*/);
    JoinThreadSlot(Socket, EraseFromMap);

    if (!EraseFromMap) /* mutex is locked if we are not erasing records from Socket-Thread map, the case when server app is closed */
    {
        mu.lock();
    }

    ::close(Socket);

    if (nameSet[0] == Socket)
    {
        nameSet[0] = 0;
        ui->Player1Box->setTitle("<defunct>");
    }
    else if (nameSet[1] == Socket)
    {
        nameSet[1] = 0;
        ui->Player2Box->setTitle("<defunct>");
    }

    qDebug() << "Socket" << Socket << "was closed";

    /* if we close server window, we shouldn't proceed listening as we do once the client is away */
    if (GetClientsConnected() < 2 && EraseFromMap)
    {
        qDebug() << "Open listening socket for listening once again as one of clients has fled";
        listen(listen_sock, 2);
    }

    //SetClosedValue(true);

    /* Isn't the same job done by ClearGridsButNotConnections() ? */
    /*if (!ClientName.isEmpty())
    {
        NameGridMap.find(ClientName)->second.WhitenAllSquares();
        qDebug() << "Clearing Player" << ClientName << "grid...";
        NameGridMap.erase(NameGridMap.find(ClientName));
        NameFleetMap.find(ClientName)->second.ClearAll();
        NameFleetMap.find(ClientName)->second.AdversaryName.clear();
        qDebug() << "Clearing Player" << ClientName << "ships coordinates vectors...";
        NameFleetMap.erase(NameFleetMap.find(ClientName));

        if (ClientName == RedPlayerName)
            RedPlayerName.clear();
        else if (ClientName == BluePlayerName)
            BluePlayerName.clear();
    }*/

    ClearShipKilled();
}
/*----------------------------------------------------------------------------------*/
void server::JoinThreadSlot(int Sock, bool EraseFromMap /*QString ThreadID*/)
{
    mu.lock();

    qDebug() << "JoinThread() is called from thread" << GetQStrThreadId(std::this_thread::get_id());

    std::map<int, std::thread *>::iterator it;
    it = ClientSockAndThread.find(Sock);

    QString QStrCurThrId = GetQStrThreadId(it->second->get_id());

    qDebug() << "Is thread" << QStrCurThrId <<"joinable?" << it->second->joinable();
    while (!it->second->joinable())
    {
        usleep(5000);
        qDebug() << "Is thread" << QStrCurThrId <<"joinable?" << it->second->joinable();
    }

    it->second->join();
    qDebug() << "Thread" << QStrCurThrId << "was joined to the main thread";
    qDebug() << "Is thread still joinable?" << it->second->joinable();

    if (EraseFromMap)
    {
        ClientSockAndThread.erase(it);
        qDebug() << "Thread" << QStrCurThrId << "and socket" << Sock << "were removed from the map";
    }

    mu.unlock();

    //SetFinishedJoining(true);
}
/*----------------------------------------------------------------------------------*/
QString server::GetQStrThreadId(std::thread::id ThrId)
{
    std::stringstream ThreadID;
    ThreadID << ThrId;
    std::string StrThrId = ThreadID.str();
    QString QStrThrId = QString::fromStdString(StrThrId);

    return QStrThrId;
}
/*----------------------------------------------------------------------------------*/
void server::ShowErrDescription(int err_no)
{
    QString ErrorDescription("Error " + QString::number(err_no) + ": " + QString(strerror(err_no)));
    QMessageBox::critical(this, "Error", ErrorDescription);
}
/*----------------------------------------------------------------------------------*/
void server::on_ShutDown_clicked()
{
    WriteToLogWindow("Server is going to shut down!");
    this->close();
}
/*----------------------------------------------------------------------------------*/
void server::closeEvent(QCloseEvent * event)
{
    // it is possible we got to that window while some sockets are opened and connections are still up
    // need to free all resourses

    qDebug() << "Closing all client sockets and joining client threads one by one";

    //mu.lock();

    KillAllConnections(true);

    //mu.unlock();

    event->QCloseEvent::accept();
}
/*----------------------------------------------------------------------------------*/
server::~server()
{
    delete ui;
}
/*----------------------------------------------------------------------------------*/
void server::SetClosedValue(bool val)
{
    closing_mu.lock();
    Closed = val;
    closing_mu.unlock();
}
/*----------------------------------------------------------------------------------*/
bool server::GetClosedValue()
{
    bool val;

    closing_mu.lock();
    val = Closed;
    closing_mu.unlock();

    return val;
}
/*----------------------------------------------------------------------------------*/
void server::ChangeColorSlot(QString WhoShot, QString SquareCoordinate)
{
    QString WhoSuffered;

    if (WhoShot == RedPlayerName)
        WhoSuffered = BluePlayerName;
    else if (WhoShot == BluePlayerName)
        WhoSuffered = RedPlayerName;

    Fleet * CurrentPlayerFleet = &(NameFleetMap.find(WhoSuffered)->second);
    Grid * CurrentPlayerGrid = &(NameGridMap.find(WhoSuffered)->second);

    /*QString*/ color_t CurrentColor = CurrentPlayerGrid->find(SquareCoordinate)->second->getColor();

    qDebug() << "Current color of square with coordinates" << SquareCoordinate << "on player" << WhoSuffered
                    << "grid is" << CurrentColor;

    ClearShipKilled();

    if (CurrentColor == white)
    {
        SetPendingColor("G");
        CurrentPlayerGrid->find(SquareCoordinate)->second->changeColor(grey);
    }
    else if (CurrentColor == green)
    {
        CurrentPlayerFleet->DecreaseHitPointsBasedOnCoord(SquareCoordinate);

        qDebug() << "Ship with ID" << CurrentPlayerFleet->GetShipIDByCoord(SquareCoordinate)
                 << "Health left" << CurrentPlayerFleet->GetHitPointsBasedOnCoord(SquareCoordinate);

        if (CurrentPlayerFleet->GetHitPointsBasedOnCoord(SquareCoordinate) == 0)
        {
            /* change color of square and neighbouring yellow squares to red - ships was destroyed */
            qDebug() << "Ship With ID" << CurrentPlayerFleet->GetShipIDByCoord(SquareCoordinate)
                     << "was destroyed!!";

            SetPendingColor("R");

            short KilledID = CurrentPlayerFleet->GetShipIDByCoord(SquareCoordinate);

            std::vector<QString>::iterator it;
            /* Only those squares which belong to the same ship */
            for (it = CurrentPlayerFleet->Coordinates.begin();
                 it != CurrentPlayerFleet->Coordinates.end(); it++)
            {
                if (CurrentPlayerFleet->GetShipIDByCoord(*it) == KilledID)
                {
                    CurrentPlayerGrid->find(*it)->second->changeColor(red);
                    AddCoordShipKilled(*it);
                }
            }

        }
        else if (CurrentPlayerFleet->GetHitPointsBasedOnCoord(SquareCoordinate) != -1)
        {
            SetPendingColor("Y");
            CurrentPlayerGrid->find(SquareCoordinate)->second->changeColor(yellow);
        }
    }

}
/*----------------------------------------------------------------------------------*/
void server::KillAllConnections(bool listening_too)
{

    mu.lock();

    WriteToLogWindow("Notify all connected players that Server purged their grids and coordinates of their ships!");

    ServerInitiatedShutdown = true;

    if (listening_too)
    {
        ::shutdown(listen_sock, SHUT_RDWR);
    }

    std::map<int, std::thread *>::iterator it;
    it = ClientSockAndThread.begin();

    while (it != ClientSockAndThread.end())
    {
        if (!listening_too && it->first == listen_sock)
        {
            it++;

            qDebug() << "Apparantly, we are not shutting server down, as listening thread keeps living";
            qDebug() << "Make sure that it is able to receive new connection requests";
            listen(listen_sock, 2);
        }
        else
        {
            QString QStrCurThrId = GetQStrThreadId(it->second->get_id());
            qDebug() << "Socket" << it->first << "and thread" << QStrCurThrId
                                << "are about to be closed and joined";

            /* Sending BBYE message to all connected clients */
            /*emit*/ SendToAllSlot/*Sig*/("bbyeserver");

            emit CloseConnectionSig(it->first, "", 0); /* as we're in the loop, we will erase from a map from here, not from CloseConnection() */

            qDebug() << "Thread" << QStrCurThrId << "and socket" << it->first << "are about to be removed from the map";

            ClientSockAndThread.erase(it++);
         }
    }

    SetClientsConnected(0);
    //SetClientsFieldFilled(0); /* That's done in ClearGridsNotConnections() */

    mu.unlock();

}
/*----------------------------------------------------------------------------------*/
void server::FillShipsSlot(QString PlayerName, QString ShipsString)
{
    NameFleetMap.find(PlayerName)->second.ClearAll();

    qDebug() << "Going to extract coordinates from the string" << ShipsString;

    ShipsString.remove("Battleship").remove("Cruisers").remove("Destroyers").remove("Torpedo").remove("Boats");
    ShipsString.remove(" ").remove(",");

    qDebug() << "Trimmed string:" << ShipsString;

    QString PotentialCoord;
    short RemoveAmnt;

    while (!ShipsString.isEmpty())
    {
        RemoveAmnt = 3;
        if (ShipsString.size() >= 3)
        {
            PotentialCoord = ShipsString.left(3);
            //qDebug() << "character " << PotentialCoord.at(2).toLatin1();
            if (PotentialCoord.at(2).toLatin1() >= 'A' && PotentialCoord.at(2).toLatin1() <= 'J')
            {
                RemoveAmnt = 2;
                PotentialCoord.remove(2, 1);
            }
        }
        else if (ShipsString.size() == 2)
        {
            RemoveAmnt = 2;
            PotentialCoord = ShipsString.left(2);
        }
        else
        {
            qDebug() << "Smth unexpected and wrong happened here";
            break;
        }

        if (PotentialCoord.at(0).toLatin1() >= 'A' && PotentialCoord.at(0).toLatin1() <= 'J' &&
                PotentialCoord.at(PotentialCoord.size() - 1).toLatin1() >= '0' &&
                PotentialCoord.at(PotentialCoord.size() - 1).toLatin1() <= '9')
        {
            qDebug() << "Adding coordinate" << ShipsString.left(RemoveAmnt) << "to" << PlayerName << "fleet";
            NameFleetMap.find(PlayerName)->second.Coordinates.push_back(PotentialCoord);

            NameGridMap.find(PlayerName)->second.find(PotentialCoord)->second->changeColor(green);

            ShipsString.remove(0, RemoveAmnt);
        }
        else
        {
            ShipsString.remove(0, 1);
        }
    }
}
/*----------------------------------------------------------------------------------*/
void server::on_ClearAll_clicked()
{
    qDebug() << "Going to drop all connections, purge Ships arrays and Usernames, color all squares White";

    KillAllConnections(false);

    /*if(!NameFleetMap.empty())
    {
        if (!RedPlayerName.isEmpty())
            NameFleetMap.find(RedPlayerName)->second.Coordinates.clear();

        if (!BluePlayerName.isEmpty())
            NameFleetMap.find(BluePlayerName)->second.Coordinates.clear();

        NameFleetMap.clear();
    }
    qDebug() << "Coordinates arrays were purged";

    */

    /* CloseConnectionSlot() already contains everything from here, doesn't it? */
    ClearGridsButNotConnectionsSlot();

    /*QString CurrCoord;

    if (!NameGridMap.empty())
    {
        for (int i = 1; i <= 10; i++)
        {
            for (char j = 'A'; j <= 'J'; j++)
            {
                CurrCoord = QString(j) + QString::number(i);

                if (!RedPlayerName.isEmpty())
                    NameGridMap.find(RedPlayerName)->second.find(CurrCoord)->second->changeColor(white);

                if (!BluePlayerName.isEmpty())
                    NameGridMap.find(BluePlayerName)->second.find(CurrCoord)->second->changeColor(white);
            }
        }
    }
    qDebug() << "Grids squares were colored to white";

    */

}
/*----------------------------------------------------------------------------------*/
void server::ClearGridsButNotConnectionsSlot()
{
    qDebug() << "Just purging Ships arrays and Usernames, and coloring all squares White";

    if(!NameFleetMap.empty())
    {
        if (!RedPlayerName.isEmpty())
            NameFleetMap.find(RedPlayerName)->second.Coordinates.clear();

        if (!BluePlayerName.isEmpty())
            NameFleetMap.find(BluePlayerName)->second.Coordinates.clear();

        NameFleetMap.clear();
    }
    qDebug() << "Coordinates arrays were purged";

    QString CurrCoord;

    if (!NameGridMap.empty())
    {
        for (int i = 1; i <= 10; i++)
        {
            for (char j = 'A'; j <= 'J'; j++)
            {
                CurrCoord = QString(j) + QString::number(i);

                if (!RedPlayerName.isEmpty())
                    NameGridMap.find(RedPlayerName)->second.find(CurrCoord)->second->changeColor(white);

                if (!BluePlayerName.isEmpty())
                    NameGridMap.find(BluePlayerName)->second.find(CurrCoord)->second->changeColor(white);
            }
        }
    }


    qDebug() << "Grids squares were colored to white";

    SetClientsFieldFilled(0);
    GetClientsFieldFilled();

}/*----------------------------------------------------------------------------------*/
void server::on_SaveLog_clicked()
{
    QDateTime dt = QDateTime::currentDateTime();
    QString date = dt.toString("dd-MMM-yyyy_hh:mm:ss");

    QString filename = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                    "Log_BattleShipServer_" + date +".log",
                                                    tr("Text files (*.txt *log)"));
    QFile f(filename);
    f.open(QIODevice::WriteOnly);
    QTextStream stream( &f );
    stream << ui->LogWindow->toPlainText() << endl;
    f.close();

    WriteToLogWindow("Log is saved into file Log_BattleShipServer_" + date +".log");

}

