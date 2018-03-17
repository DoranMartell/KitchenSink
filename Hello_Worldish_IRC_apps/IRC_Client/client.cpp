#include "client.h"
#include "ui_client.h"

#include <QDebug>
#include <QMessageBox>

Client::Client(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);

    memset(RawMsg, '\0', sizeof(RawMsg));

    QObject::connect(&RecvThread, &ReceivingThread::PrintMessageRecvThread, this, &Client::PrintMessageOnScreen/*, Qt::BlockingQueuedConnection*/);
    QObject::connect(&RecvThread, &ReceivingThread::ClientWindowDisconHandler, this, &Client::Disconnected/*, Qt::BlockingQueuedConnection*/);
}
/*--------------------------------------------------------------------------------------------------------------*/
Client::~Client()
{
    delete ui;
}
/*--------------------------------------------------------------------------------------------------------------*/
void Client::OpenSocket(QString s, QString p, bool & result)
{
    serverIpAddr = s;
    serverPortNumber = p;
    result = true;

    //qDebug() << "server is " << serverIpAddr << ":" << serverPortNumber;

    connectionSocket = socket(AF_INET, SOCK_STREAM, 0);

    BuildServerAddr();

    if (::connect(connectionSocket, (struct sockaddr *) & dest, sizeof(struct sockaddr)) != 0 )
    {
        qDebug() << "Connection failed! Error Code: " << strerror(errno);

        QString errMsg("Connection Failed!\nError Code " +  QString::number(errno) + ": " + QString(strerror(errno)));
        QMessageBox::critical(this, "Error", errMsg);
        ui->HistoryEdit->setText("");
        ui->MessageEdit->setText("");

        emit LoginWindowClose(); /* closes log-in window */
        emit ReturnToStart(); /* shows Window with server credentials again */
        result = false;
        return;
    }

    /* kills thread to start from scratch */
    //RecvThread.setKillThread(true);

    RecvThread.setKillThread(false);
    /* pass socket desciptor to receiving thread */
    RecvThread.connectionSocket = connectionSocket;
    RecvThread.start(); /* client ready to receive responses from server */
}
/*--------------------------------------------------------------------------------------------------------------*/
void Client::ShowConversationWindow(QString u)
{
    userName = u;

    QString title("User \"" + userName + "\" connected to " + serverIpAddr + ":" + serverPortNumber);

    this->setWindowTitle(title);
    this->show();
}


/*--------------------------------------------------------------------------------------------------------------*/
void Client::BuildServerAddr()
{
    memset(&dest, 0, sizeof(struct sockaddr));
    dest.sin_family = AF_INET;
    dest.sin_port = htons(serverPortNumber.toShort());
    inet_aton(serverIpAddr.toLatin1(), &dest.sin_addr);

    /* Just to check that in this structure is stored the same stuff we put in */
    char str1[10];
    short sh2;
    /* converting address in_addr to string */
    inet_ntop(AF_INET, &(dest.sin_addr), str1, INET_ADDRSTRLEN);
    sh2 = ntohs(dest.sin_port);

    qDebug() << "Server addr built: " << QString::fromStdString(str1) << ":" << sh2;

    /* send server address structure to receiving thread as it's used in recvfrom() */
    RecvThread.server = dest;

}
/*--------------------------------------------------------------------------------------------------------------*/
void Client::on_SendButton_clicked()
{
    QString msgType("message");
    SendMessage(msgType, userName, ui->MessageEdit->toPlainText()); /* passes message from MessageEdit to sending thread */
    /* PrintMessageOnScreen(ui->MessageEdit->toPlainText()); */
}
/*--------------------------------------------------------------------------------------------------------------*/
void Client::PrintMessageOnScreen(QString mess)
{
    qDebug() << "Gonna print message FROM server: " << mess;

    ui->HistoryEdit->setText(ui->HistoryEdit->toPlainText() + mess + "\n");

    ui->MessageEdit->setText(""); /* Clear to make some room for new message */
}
/*--------------------------------------------------------------------------------------------------------------*/

void Client::on_DissconButton_clicked()
{
    /* closes socket, sends LogOff message to server
                        and shows ServerCredentials window again */

    SendMessage("log-off", userName, "");
    qDebug() << "Log-Off message sent";

    Disconnected("client");

}
/*--------------------------------------------------------------------------------------------------------------*/
void Client::Disconnected(QString source)
{
    qDebug() << source << "is to blame for this disconnect!";

    if (source == "server")
    {
        QMessageBox::critical(this, "Error", "Disconnected from server");
    }

    /* quits receiving thread as it was opened once new connection is established */
    RecvThread.setKillThread(true);

    ::close(connectionSocket);
    //QMainWindow::close(); /* had to remove that since we added Disconnected() inside CloseEvent() */
    hide();
    emit ReturnToStart(); /* open ServerCredentialsWindow once again */

}
/*--------------------------------------------------------------------------------------------------------------*/

void Client::on_QuitButton_clicked()
{
    /* go to closeEvent() */
    close();
}
/*--------------------------------------------------------------------------------------------------------------*/
void Client::closeEvent(QCloseEvent * event)
{
    on_DissconButton_clicked();

    //QApplication::exit();

    event->accept();
}

/*--------------------------------------------------------------------------------------------------------------*/
void Client::SendMessage(QString msgtype, QString username, QString actual_message)
{
    /* Build RawMsg to be sent */
    BuildRawMessage(msgtype, username, actual_message);

    qDebug() << "Gonna send message TO server: " << RawMsg;
    if (send(connectionSocket, RawMsg, strlen(RawMsg), 0) < 0)
    {
        QMessageBox::critical(this, "Error", "Error sending a message!");
        ui->HistoryEdit->setText(ui->HistoryEdit->toPlainText() +"[" + userName
                                 + "]: " + "Error sending message!\n");

    }
}
/*--------------------------------------------------------------------------------------------------------------*/
void Client::BuildRawMessage(QString msgtype, QString username, QString actual_message)
{
    /* message structure goes as following:
     * MsgType - 7 characters,
     * UserName - 16 characters (if less then padding with spaces)
     * ActualMessage - BUFFSIZE - 23 = 233 characters */

    memset(RawMsg, '\0', sizeof(RawMsg));

    QDateTime dt = QDateTime::currentDateTime();
    QString date = dt.toString("dd-MMM-yyyy hh:mm:ss");
    qDebug() << "Current date-time: " << date;

    Pad(username, 16); /* username length */
    Trim(actual_message, (BUFFSIZE - 7 - 16)); /* actual_message is buffsize without msgtype and username */

    qDebug() << "Padded username: " << username;

    QString all_combined(msgtype + username + date + actual_message);

    strcpy(RawMsg, all_combined.toLatin1());

    qDebug() << "Raw Message: " << RawMsg;

}
/*--------------------------------------------------------------------------------------------------------------*/
void Client::Pad(QString &whatever, int lengthLack)
{
    whatever = whatever.leftJustified(lengthLack, ' ' );
}
/*--------------------------------------------------------------------------------------------------------------*/
void Client::Trim(QString &whatever, int lengthExcess)
{
    whatever.chop(whatever.size() - lengthExcess); /* FROM THE BEGINNING, NOT THE END!! */
}
/*--------------------------------------------------------------------------------------------------------------*/
void Client::LastSeenMsgBox(QString LastSeenOnline)
{
    QMessageBox::information(this, "Last Seen Online", LastSeenOnline);
}
