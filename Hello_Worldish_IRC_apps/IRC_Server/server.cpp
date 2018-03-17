#include "server.h"
#include "ui_server.h"
#include <QMessageBox>
#include <QDebug>

Server::Server(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Server)
{
    ui->setupUi(this);

      QObject::connect(&ConnectionThread, &ConnThread::errMessConnThread, this, &Server::ErrorMess );
      QObject::connect(&ConnectionThread, &ConnThread::PrintMessageConnThread, this, &Server::PrintMessInWindow, Qt::BlockingQueuedConnection );
      QObject::connect(&ConnectionThread, &ConnThread::NumOfConnUpd, this, &Server::NewNumberOfConnections, Qt::BlockingQueuedConnection );
                                             /* passing pointer as an argument, so it's essential to pass it safely to another thread */


      /*  for (int i = 0; i < MAX_CONNECTIONS; i++)
      {
          QObject::connect(this, &Server::KillRWThread, &ConnectionThread.ReadWriteThread[i], &RWThread::setRWKilled);
          QObject::connect(&ConnectionThread.ReadWriteThread[i], &RWThread::errMessRWThread, this, &Server::PrintMessInWindow);
          QObject::connect(&ConnectionThread.ReadWriteThread[i], &RWThread::PrintMessageRWThread, this, &Server::PrintMessInWindow);
          QObject::connect(&ConnectionThread.ReadWriteThread[i], &RWThread::SendToAllClients, &ConnectionThread, &ConnThread::SendToAll);
      }*/

}
/*--------------------------------------------------------------------------------------------------------------*/
void Server::showWindow()
{

    ConnectionThread.setConnectKilled(false);
    ConnectionThread.start();

    QString title("Server: port " + QString::number(ConnectionThread.portNo));
    qDebug() << "Supposed window title" << title;

    this->setWindowTitle(title);

    show();

}
/*--------------------------------------------------------------------------------------------------------------*/
void Server::NewNumberOfConnections(int num, ConnThread::ClientsParams ** newThread)
{
    qDebug() << "GUI thread to print number_of_connections in statusBar (ConnThread called): " << num;
    QString mess = QString::number(num) + " users are currently online!";
    ui->statusBar->showMessage(mess);

    if (!ConnectionThread.Clients.isEmpty() && newThread != NULL)
    {

        /* Had to use list of pointer to objects instead of objects*/
        /* Otherwise copy constructor is called, which is not allowed for classes inherited from QObject */
        //QList<ConnThread::ClientsParams*>::iterator curr = QList<ConnThread::ClientsParams*>::iterator();
        //curr = ConnectionThread.Clients.end();
       // ConnThread::ClientsParams * newThread = *curr;

        QObject::connect(&(*newThread)->ReadWriteThread, &RWThread::errMessRWThread, this, &Server::PrintMessInWindow, Qt::BlockingQueuedConnection);
        QObject::connect(&(*newThread)->ReadWriteThread, &RWThread::PrintMessageRWThread, this, &Server::PrintMessInWindow, Qt::BlockingQueuedConnection);
        QObject::connect(&(*newThread)->ReadWriteThread, &RWThread::DecreaseNumberOfConnections, this, &Server::StatBarNumOfConn, Qt::/*Blocking*/QueuedConnection);
    }

    qDebug() << "NewNumberOfConnections() slot exit";
    return;
}
/*--------------------------------------------------------------------------------------------------------------*/
void Server::StatBarNumOfConn() /* simplified twin of NewNumberOfConnections() for ReadWriteThread */
{
    qDebug() << "GUI thread to print number_of_connections in statusBar (RWThread called): " << ConnectionThread.number_of_connections - 1;
    QString mess = QString::number(ConnectionThread.number_of_connections - 1) + " users are currently online!";
    /* the value of number_of_connections is actually changing in ConnThread::RemoveFromList() which is called right after this
            function (but in the other thread) */
    ui->statusBar->showMessage(mess);
}
/*--------------------------------------------------------------------------------------------------------------*/
void Server::PrintMessInWindow(QString string)
{
    qDebug() << "Message printed:" << string;
    ui->textEdit->setText(ui->textEdit->toPlainText() + string + "\n");
    return;
}
/*--------------------------------------------------------------------------------------------------------------*/
void Server::ErrorMess(QString errmsg)
{
    QMessageBox::critical(this, "Error", errmsg);

    if (errmsg != "Can't receive any more connections!")
    {
        emit KillConnectionThread(true);
        QApplication::exit();
    }
}

/*--------------------------------------------------------------------------------------------------------------*/
void Server::on_ClearButton_clicked()
{
    ui->textEdit->setText("");
    ui->statusBar->showMessage("History cleared", 3000);
}

/*--------------------------------------------------------------------------------------------------------------*/
Server::~Server()
{
    delete ui;
}
/*--------------------------------------------------------------------------------------------------------------*/
void Server::CloseServerWindow(QString meaningless)
{
    ::shutdown(ConnectionThread.listenSocket, SHUT_RDWR); /* this should kill hanged accept() */
    ::close(ConnectionThread.listenSocket); /* :: because otherwise Qt is confused which close() to call */
    ConnectionThread.setConnectKilled(true); /* will kill all ReadWrite threads as well as Connection Thread */

    hide();
}

/*--------------------------------------------------------------------------------------------------------------*/
void Server::on_ShutDownButton_clicked()
{    
    /* go to closeEvent() */
    close();
}
/*--------------------------------------------------------------------------------------------------------------*/
void Server::closeEvent(QCloseEvent * event)
{
    qDebug() << "Send all clients a message that server disconnected";
    QString date = QDateTime::currentDateTime().toString("dd-MMM-yyyy hh:mm:ss");
    qDebug() << "Current date-time: " << date;

    QString usr("server");
    ConnectionThread.SendToAll("log-off"+ usr.leftJustified(16, ' ') + date + "");

    qDebug() << "Remove all threads from list";
    while( !ConnectionThread.Clients.isEmpty())
    {
        ConnThread::ClientsParams * current = ConnectionThread.Clients.first();
        qDebug() << "About to remove thread" << current->num;
        ConnectionThread.RemoveFromList(current->num);
        //ConnectionThread.Clients.removeFirst(); /* no need to do it - RemoveFromList() already has erase() */
        qDebug() << "isEmpty = " << ConnectionThread.Clients.isEmpty();

    }

    ::shutdown(ConnectionThread.listenSocket, SHUT_RDWR); /* this should kill hanged accept() */
    ::close(ConnectionThread.listenSocket); /* :: because otherwise Qt is confused which close() to call */
    ConnectionThread.setConnectKilled(true); /* will kill all ReadWrite threads as well as Connection Thread */

    qDebug() << "Closing application";
    event->QCloseEvent::accept();
}
/*--------------------------------------------------------------------------------------------------------------*/
