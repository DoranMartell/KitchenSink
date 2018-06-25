#include "client.h"
#include "ui_client.h"
/*----------------------------------------------------------------------------------*/
client::client(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::client)
{
    ui->setupUi(this);
    qDebug() << "Main Client thread started with ID" << GetQStrThreadId(std::this_thread::get_id());

    LinkYourSquareMap();
    LinkEnemySquareMap();

    mess_mu.lock();
    EnemySquare.GreenSquaresSelected = false;
    mess_mu.unlock();

    connect(this, &client::Flee, this, &client::on_FleeButton_clicked);
    connect(this, SIGNAL(writeToStatusBarSig(QString )), this, SLOT(writeToStatusBarSlot(QString )));
    connect(this, SIGNAL(writeToStatusBarSig(QString , QString , QString )), this, SLOT(writeToStatusBarSlot(QString , QString , QString )));

    SetOneGreenSquareChosen("");

//    setNameOk(true);
}
/*----------------------------------------------------------------------------------*/
void client::AllWhite()
{
    qDebug() << "Paint all squares on all Main Window grids to white color";
    for (char i = 'A'; i <= 'J'; i++)
    {
        for (short j = 1; j <= 10; j++)
        {
            YourSquare.find(QString(QString(i) + QString::number(j)))->second->changeColor(white);
            YourSquare.find(QString(QString(i) + QString::number(j)))->second->setClickability(false);
            EnemySquare.find(QString(QString(i) + QString::number(j)))->second->changeColor(white);
        }
    }
}

/*----------------------------------------------------------------------------------*/
void client::ShowMainWindowSlot()
{
    /* Take coordinates of Your ships and color respective squares in Green on Client Window */
    InitialShipsPlacement();

    /* Change title of the Box over Enemy Ships Grid with the name received from server */
    mess_mu.lock();
    ui->EnemygroupBox->setTitle(AdversaryName);
    mess_mu.unlock();

    setWindowTitle("Battleship Game Window of " + YourName);
    SetServerUp(true);

    QWidget::setEnabled(true);
    GameIsNotOver = true;
    ui->statusBar->clearMessage();

    show();

}
/*----------------------------------------------------------------------------------*/
void client::ServCredentialsCollectedSlot(QString Ip, int Port)
{
    BuildServerAddr(Ip, Port);
}
/*----------------------------------------------------------------------------------*/
void client::BuildServerAddr(QString Ip, int Port)
{
    ServerDetails.sin_family = AF_INET;
    inet_aton(Ip.toLatin1(), &ServerDetails.sin_addr);
    ServerDetails.sin_port = htons( (unsigned short) Port);

    CreatSockAndConnectToAddr();
}
/*----------------------------------------------------------------------------------*/
short client::CreatSockAndConnectToAddr()
{
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0)
    {
        ShowErrDescription(errno);
        return -1;
    }

    if (::connect(server_sock, (struct sockaddr *) &ServerDetails, sizeof(struct sockaddr)) < 0)
    {
        ShowErrDescription(errno);
        return -2;
    }

    /* let's start a thread here which would receive messages from server */
    ReceivingThread = std::thread(&client::ReceiveMess,this);

    shouldHideAskServWindow = 1;
    return 0;
}
/*----------------------------------------------------------------------------------*/
void client::ShowErrDescription(int err_no)
{
    QString ErrorDescription("Error " + QString::number(err_no) + ": " + QString(strerror(err_no)));
    QMessageBox::critical(this, "Error", ErrorDescription);
}
/*----------------------------------------------------------------------------------*/
void client::CloseConnAndJoinThreadSlot()
{
    qDebug() << "Current thread ID is" << GetQStrThreadId(std::this_thread::get_id());

    qDebug() << "ShutDown socket, associated with server, so receiving thread will stop awaiting for incoming messages";
    ::shutdown(server_sock, SHUT_RDWR);

    qDebug() << "Closing socket, associated with server...";
    ::close(server_sock);

    QString RecvThreadId = GetQStrThreadId(ReceivingThread.get_id());

    if (RecvThreadId.toDouble() != 0.0)
    {
        qDebug() << "Is thread" << RecvThreadId << "joinable?" << ReceivingThread.joinable();
        while (!ReceivingThread.joinable())
        {
            usleep(5000);
            qDebug() << "Is thread" << RecvThreadId << "joinable?" << ReceivingThread.joinable();
        }

        qDebug() << "Trying to join" << RecvThreadId << "thread...";
        ReceivingThread.join();
        qDebug() << "Thread with ID" << RecvThreadId << "was joined to the main thread";
        qDebug() << "Is thread" << RecvThreadId << "still joinable?" << ReceivingThread.joinable();
    }
    else
    {
        qDebug() << "Seems that thread" << RecvThreadId << "was already joined";
    }

}
/*----------------------------------------------------------------------------------*/
void client::InitialShipsPlacement()
{
    /* at first clearing grids of previous game leftovers */
    AllWhite();

    qDebug() << "Judging by your ships coordinates, sent to the server"
                " color squares with Green in Client's window";

    std::vector<QString>::iterator it;

    /* ships coords are shared between threads, though it is unlikely
        that they are used by smth else in the moment this function is called */
    mess_mu.lock();

    for (it = BattleShipCoords.begin(); it != BattleShipCoords.end(); it++)
    {
        YourSquare.find((*it))->second->changeColor(green);
    }

    for (it = CruisersCoords.begin(); it != CruisersCoords.end(); it++)
    {
        YourSquare.find((*it))->second->changeColor(green);
    }

    for (it = DestroyersCoords.begin(); it != DestroyersCoords.end(); it++)
    {
        YourSquare.find((*it))->second->changeColor(green);
    }

    for (it = TorpedoBoatsCoords.begin(); it != TorpedoBoatsCoords.end(); it++)
    {
        YourSquare.find((*it))->second->changeColor(green);
    }

    mess_mu.unlock();
}
/*----------------------------------------------------------------------------------*/
void client::ChangeOthersClickabilitySlot(QString coord)
{
    mess_mu.lock();

    if (EnemySquare.GreenSquaresSelected == true)
    {
        EnemySquare.GreenSquaresSelected = false;
        qDebug() << "You are able to press on Enemy Grid square to mark it as target";
    }
    else
    {
        EnemySquare.GreenSquaresSelected = true;
        qDebug() << "Target set on" << coord << "! Unable to mark other squares until this one is released";
    }

    EnemySquare.ChangeOthersClickability(coord);

    mess_mu.unlock();
}
/*----------------------------------------------------------------------------------*/
void client::on_FireButton_clicked()
{
    qDebug() << "Fire Button Was Clicked!";

    if (GetOneGreenSquareChosen().isEmpty())
    {
        QMessageBox::critical(this, "Error", "You didn't choose a square which you are going to hit!"
                                             " Please, click on any white square and press \"Fire\" button again.");
        return;
    }
    else
    {
        SendFireSlot(GetOneGreenSquareChosen());

        SetOneGreenSquareChosen("");
        return;
    }
}
/*----------------------------------------------------------------------------------*/
void client::ColorTheSquare(QString WhoShot, QString Coordinate, QString Color, short MessageNumber)
{
    color_t Col_Val;

    if (Color == "Y") Col_Val = yellow;
    else if (Color == "R") Col_Val = red;
    else if (Color == "G") Col_Val = grey;

    if (WhoShot == YourName)
    {
        qDebug() << "Going to change color of one square on ENEMY grid...";
        EnemySquare.find(Coordinate)->second->changeColor(Col_Val);

        /* making you able to select a square on your next turn */
        if (MessageNumber == AmountOfSubsequentMessages)
        {
            emit EnemySquare.find(Coordinate)->second->ChangeOthersClickabilitySig(Coordinate);
        }
    }
    else if (WhoShot == AdversaryName)
    {
        qDebug() << "Going to change color of one square on YOUR grid...";
        YourSquare.find(Coordinate)->second->changeColor(Col_Val);
    }

    emit writeToStatusBarSig(WhoShot, Coordinate, Color);

}
/*----------------------------------------------------------------------------------*/
void client::writeToStatusBarSlot(QString message)
{
   if (!ui->statusBar->currentMessage().endsWith("Congratulations!") ||
         !ui->statusBar->currentMessage().endsWith("All is lost!"))
    ui->statusBar->showMessage(message);
}
/*----------------------------------------------------------------------------------*/
void client::writeToStatusBarSlot(QString Who, QString Coord, QString Col)
{
    QString WhoInStatus;
    QString WhoseInStatus;
    QString ResultInStatus;

    if (Who == YourName)
    {
        WhoInStatus = "You";
        WhoseInStatus = "enemy";
    }
    else
    {
        WhoInStatus = Who;
        WhoseInStatus = "your";
    }

    if (Col == "R")
        ResultInStatus = "Destroyed " + WhoseInStatus + " ship and can shoot again!";
    else if (Col == "Y")
        ResultInStatus = "Hit " + WhoseInStatus + " ship and can shoot again!";
    else if (Col == "G")
        ResultInStatus = "Missed!";

    QString Message = (Col != "R") ? (WhoInStatus + " shot square " + Coord + ". " + WhoInStatus + " " + ResultInStatus)
                                   : (WhoInStatus + " " + ResultInStatus);
    writeToStatusBarSlot(Message);
}

/*----------------------------------------------------------------------------------*/
void client::on_FleeButton_clicked()
{
    qDebug() << "Flee called from thread" << GetQStrThreadId(std::this_thread::get_id());

    close();
}
/*----------------------------------------------------------------------------------*/
void client::closeEvent(QCloseEvent *event)
{
    if (GetServerUp())
    {
        qDebug() << "sending BBYE message to the server";
        SendBbyeSlot();
    }
    else if (GetStartGame() == true) /* show discon window from here only if Client Window is actually opened */
    {
        QMessageBox::critical(this, "Disconnected from server!", "Disconnected from Server!");
    }

    qDebug() << "We're currently in the thread with ID" << GetQStrThreadId(std::this_thread::get_id());
    qDebug() << "Close connection and join the receiving thread...";
    CloseConnAndJoinThreadSlot();

    qDebug() << "Closing Client Main window";
    event->QCloseEvent::accept();
}
/*----------------------------------------------------------------------------------*/
void client::CloseFromSomewhereSlot()
{
    qDebug() << "Main Client window is apparently closed from some other window";
    close();
}
/*----------------------------------------------------------------------------------*/
client::~client()
{
    delete ui;
}
