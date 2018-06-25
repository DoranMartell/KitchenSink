#ifndef SERVER_H
#define SERVER_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>
#include <QDateTime>
#include <QFileDialog>
#include <QCloseEvent>

#include "globals.h"
#include "qgridsquare.h"

#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <fcntl.h>

#include <thread>
#include <mutex>
#include <map>

#include <string>
#include <sstream>

#include <errno.h>

namespace Ui {
class server;
}

class server : public QMainWindow
{
    Q_OBJECT

public:

    explicit server(QWidget *parent = 0);
    ~server();

    /* class linking Cooordinates (of all squares) in format like "A1" with
        actual GUI object */
    class Grid : public std::map<QString, QGridSquare *>
    {
    public:

        void WhitenAllSquares()
        {
            for (char i = 'A'; i <= 'J'; i++)
            {
                for (short j = 1; j <= 10; j++)
                {
                    QString name = QString(i) + QString::number(j);
                    this->find(name)->second->changeColor(white);
                }
            }
        }
    };

    void LinkCoordinatesAndActualSquares();

    /* type linking PlayerName and GUI grid */
    typedef std::map<QString , Grid > namegridmap;
    Grid RedPlayerGrid;
    Grid BluePlayerGrid;
    namegridmap NameGridMap;

    class Ship
    {
    public:
        std::vector<QString> Coordinates;
        short HitPoints;
    };

    /* class, containing Player's ships coordinates (ONLY!) in one vector and ships hitpoins in the other */
    class Fleet
    {
    public:
        Fleet() {ClearAll();}

        QString AdversaryName;

        std::vector<QString> Coordinates;
            /* index of Coordinate explicitly tells us which ship it was
                0 - 3 battleship,
                4 - 5 first cruiser,
                6 - 7 second cruiser, .... */
        short HitPoints[10];
        //???std::vector<QString> ShipType;

        void ClearAll()
        {
            Coordinates.clear();

            short j = 4;
            for (short i = 0; i < 10; i++)
            {
                if (i == 1 || i == 3 || i == 6)
                    j--;

                HitPoints[i] = j;
            }
        }

        short GetShipIDByCoord(QString Coord)
        {
            short i = 0;
            short j = -1;

            std::vector<QString>::iterator it;

            for (it = Coordinates.begin(); it != Coordinates.end(); it++)
            {
                if (Coord == (*it))
                {
                    if (i < 4)
                        j = 0;
                    else if (i < 7)
                        j = 1;
                    else if (i < 10)
                        j = 2;
                    else if (i < 12)
                        j = 3;
                    else if (i < 14)
                        j = 4;
                    else if (i < 16)
                        j = 5;
                    else if (i < 17)
                        j = 6;
                    else if (i < 18)
                        j = 7;
                    else if (i < 19)
                        j = 8;
                    else if (i < 20)
                        j = 9;

                    break;
                }

                i++;
            }

            return j;
        }

        short GetHitPointsBasedOnCoord(QString Coord)
        {
            short j = GetShipIDByCoord(Coord);
            if (j >= 0)
                return HitPoints[j];
            else
                return j;
        }

        void DecreaseHitPointsBasedOnCoord(QString Coord)
        {
            short j = GetShipIDByCoord(Coord);
            HitPoints[j]--;
        }

        bool IsFleetCompletelyDestroyed()
        {
            bool result = true;

            for (int i = 0 ; i < 10; i++)
            {
                qDebug() << "Health of ship with ID" << i << "is" << HitPoints[i];
                if (HitPoints[i] != 0)
                {
                    result = false;
                }
            }

            return result;
        }

    };

    /* type, linking Player name and Fleet class (Coordinates and Hit Points) */
    typedef std::map<QString , Fleet> namefleetmap;
    namefleetmap NameFleetMap;
    Fleet RedPlayerFleet;
    Fleet BluePlayerFleet;

    //std::mutex mu;
    std::recursive_mutex mu;

    struct sockaddr_in ServerDetails;

    std::mutex closing_mu;
    bool Closed;
    void SetClosedValue(bool );
    bool GetClosedValue();

    class ClientDetails
    {
    public:
        struct sockaddr_in ClientAddress;
        socklen_t ClientLen;
        int client_socket;

        QString ClientIP;
        int ClientPort;
        QString ClientName;
        int MessagesReceived;

    };

    //std::list<int> ClientSocket; /* unfortunately, we had to create a separate list of sockets,
    //                                so on thread could get messages from one socket and transmit to another */

    //std::list<std::thread> ClientThread;

    /* associate each client socket with a thread which handles messages from/to this client */
    std::map<int, std::thread *> ClientSockAndThread;

    short ClientsConnected;

    short ClientsFieldFilled; /* tracks if none/one/both players  ships to the grid */

    std::thread AcceptingThread;

    /* thread-safe Get of ClientsConnected value */
    short GetClientsConnected()
    {
        short s;

        mu.lock();
        s = ClientsConnected;
        mu.unlock();

        return s;
    }

    /* thread-safe Set of ClientsConnected value */
    void SetClientsConnected(short val)
    {
        mu.lock();
        ClientsConnected = val;
        mu.unlock();

        qDebug() << "Clients Connected is set to " << ClientsConnected;
    }

    /* thread-safe Get of ClientsFieldFilled value */
    short GetClientsFieldFilled()
    {
        short s;

        mu.lock();
        s = ClientsFieldFilled;
        mu.unlock();

        qDebug() << ClientsFieldFilled << "clients placed their ships";

        return s;
    }

    /* thread-safe Set of ClientsFieldFilled value */
    void SetClientsFieldFilled(short val)
    {
        mu.lock();
        ClientsFieldFilled = val;
        mu.unlock();
    }

    /* thread-safe push back to the list of clients sockets */
    /*void ClientSocketPushBack(int sck)
    {
        mu.lock();
        ClientSocket.push_back(sck);
        mu.unlock();
    }*/

    /* mind move() and two ampersands !!*/
    /*void ThreadSafePushBack(std::thread&& tr)
    {
        qDebug() << GetQStrThreadId(tr.get_id()) << "was added to the list";

        mu.lock();
        ClientThread.push_back(std::move(tr));
        mu.unlock();
    }*/

    void ThreadSafeInsertToMap(int sck, std::thread * tr)
    {
         mu.lock();

         /* pass pointer to thread as we can't copy it to the array */
         ClientSockAndThread.insert(std::pair<int, std::thread *>(sck, tr));

         qDebug() << "Socket" << sck << "and thread with id" << GetQStrThreadId(tr->get_id()) << "was added to the map";
         mu.unlock();
    }

    void SendToAnotherClient(QString , int ThisClientSocket);
                /* passing this client socket so it would find socket of another client,
                    fortunately, there are only two of them */
    void SendToThisClient(QString , int ThisClientSocket);
                /* as we need to respond to client with results of clients shot */

    int listen_sock; /* socket which is used to handle incoming connection requests from new clients */

    QString RedPlayerName;
    QString BluePlayerName;

    void ChangeColorSlot(QString , QString );

    void FillShipsSlot(QString , QString );

signals:

    void WriteToLogSig(QString );
    void SetPlayerNameOnWindowSig(QString , int );

    void JoinThreadSig(int , bool /*QString */);
    void CloseConnectionSig(int , QString , bool /*, QString */);

    void SendToAllSig(QString );

    void ChangeColorSig(QString , QString );

    void FillShipsSig(QString , QString );

    void ClearGridsButNotConnectionsSig();

public slots:

    void setPortNumSlot(QString ); /* switching to server once we pressed "Choose" in AskPort window */
    void startServAndShowSlot(); /* shows Server GUI window and starts a thread to accept connection requests */
    void WriteToLogSlot(QString );
    void SetPlayerNameOnWindowSlot(QString , int );
    void JoinThreadSlot(int , bool /*QString */);

    void CloseConnectionSlot(int , QString, bool /*, QString */);

    void SendToAllSlot(QString );

    void ClearGridsButNotConnectionsSlot();

private slots:

    void on_ShutDown_clicked();

    void on_ClearAll_clicked();

    void on_SaveLog_clicked();

private:

    Ui::server *ui;

    void ShowErrDescription(int );
    void BuildServerAddr(int ); /* takes PortNum as an argument */
    int CreatSockAndBindToAddr(); /* uses ServerDetails structure
                                filled in BuildServerAddr() to bind a socket to it */

    void HandleConnectionRequest(); /* thread for accept() new connections */
    void HandleClient(ClientDetails ); /* per each client thread to handle recfrom() and sendto() */

    short ParseMessage(ClientDetails & , QString ); /* parses message and decides what to do next depending on message Header */
          // had to pass (obviously) ClientDetails by reference, as otherwise the thread doesn't remember changes once left ParseMessage() */

    /* thread-safe as messages from different clients/players should be logged */
    void WriteToLogWindow(QString );

    QString GetQStrThreadId(std::thread::id );

    void KillAllConnections(bool );

    void closeEvent(QCloseEvent * );
};

#endif // SERVER_H
