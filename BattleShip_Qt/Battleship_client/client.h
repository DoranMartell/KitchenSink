#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDebug>

#include <thread>
#include <mutex>
#include <random>

#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <errno.h>

#include "globals.h"
#include "qgridsquare.h"

namespace Ui {
class client;
}

class client : public QMainWindow
{
    Q_OBJECT

public:
    explicit client(QWidget *parent = 0);
    ~client();

    struct sockaddr_in ServerDetails;

    short CreatSockAndConnectToAddr();

    int server_sock;

    //std::mutex mess_mu; /* moved to globals */

    std::thread ReceivingThread;

    class CoordAndColors
    {
        QGridSquare * gr;
        int color;
    };

    /* returns coordinates of any type of ship (two-character second argument)
        whole vector in one string separated by spaces */
    //QString GetCoords(QString );


    class Grid : public std::map<QString, QGridSquare *>
    {

    public:
        short GreenSquaresSelected;

        void ChangeOthersClickability(QString coord)
        {
            qDebug() << "Change clickability of squares others but" << coord;

            std::map<QString, QGridSquare *>::iterator it;

            for (it = this->begin(); it != this->end(); it++)
            {
                if (it->first != coord)
                {
                    if (GreenSquaresSelected)
                    {
                        it->second->setClickability(false);
                    }
                    else if (it->second->getColor() == white ||
                             it->second->getColor() == green)
                    {
                        it->second->setClickability(true);
                    }
                }
            }
        }
    };

    std::map<QString, QGridSquare *> YourSquare;
    Grid EnemySquare;

    void SetTheTurn(QString , QString );

    void ColorTheSquare(QString , QString , QString , short );

public slots:

    void ServCredentialsCollectedSlot(QString , int );

    void SendNameSlot(QString );

    void SendBbyeSlot();

    void SendFillSlot();

    void SendFireSlot(QString Coord);

    void CloseConnAndJoinThreadSlot();

    void ShowMainWindowSlot();

    void ChangeOthersClickabilitySlot(QString coord);

    void on_FleeButton_clicked();

    void writeToStatusBarSlot(QString );
    void writeToStatusBarSlot(QString , QString , QString );

    void CloseFromSomewhereSlot();

signals:

    void Flee();

    void hideFillGridWindowSig();

    void showAskNameWindowSig();

    void showEOGWindowSig(QString , QString);

    void showDefeatWindowSig();

    void writeToStatusBarSig(QString );
    void writeToStatusBarSig(QString , QString , QString );

private slots:


    void on_FireButton_clicked();

private:

    Ui::client *ui;

    void AllWhite();

    /* AdversaryName was moved to global variables */
    QString YourName/*, AdversaryName*/;

    void LinkYourSquareMap();
    void LinkEnemySquareMap();

    void ShowErrDescription(int );

    void BuildServerAddr(QString , int );

    QString BuildMessage(QString Header, QString Remainder);

    void SendMessage(QString );

    void ReceiveMess(); /* eternal function for a daemon thread */

    void ParseMessage(QString ); /* depending on header value of a message we proceed with different actions */

    void InitialShipsPlacement();

    void closeEvent(QCloseEvent *event);

    short AmountOfSubsequentMessages;

    bool GameIsNotOver;


};

#endif // CLIENT_H
