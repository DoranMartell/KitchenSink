#include "server.h"
#include "ui_server.h"

/*----------------------------------------------------------------------------------*/
short server::ParseMessage(ClientDetails & Client, QString Msg)
{
    QString Header = Msg.left(4);
    QString Remainder = Msg.remove(0, 4);

    qDebug() << "Message from socket " << Client.client_socket << " parsed:";
    qDebug() << "Header: " << Header;
    qDebug() << "Remainder: " << Remainder;
    /* depending on the value of Header, message's Remainder will be parsed differently */

    /* We should be able to parse message with delimeters 0x1F */

    if (Client.MessagesReceived == 0 )
    {
        if (QString::compare(Header, "name", Qt::CaseSensitive))
        {
            qDebug() << "First Message should contain name of the player!";
            qDebug() << "Close connection immediately!";

            emit CloseConnectionSig(Client.client_socket, Client.ClientName, true/*, GetQStrThreadId(std::this_thread::get_id())*/);
            return -1;
        }
    }

    if (Header == "name")
    {

//        if (Client.MessagesReceived > 0)
//        {
//            qDebug() << "It is not possible to get multiple NAME-messages from the same client!";
//            emit CloseConnectionSig(Client.client_socket, Client.ClientName, true/*, GetQStrThreadId(std::this_thread::get_id())*/);
//            return -3;
//        }
//        else
//        {
            Client.ClientName = Remainder;

            if (Client.ClientName == ui->Player1Box->title() ||
                    Client.ClientName == ui->Player2Box->title())
            {
                qDebug() << "Apparantly, the name is already taken by another player";
                SendToThisClient(QString("name") + "error", Client.client_socket);
            }
            else
            {
                qDebug() << "About to print a name of the player";

                emit SetPlayerNameOnWindowSig(Client.ClientName, Client.client_socket);

                WriteToLogWindow("Player \"" + Client.ClientName + "\" connected...");

                qDebug() << "Send a confirmation to client that server acknowledged his/her name";
                SendToThisClient(QString("name") + "ok", Client.client_socket);


                if (GetClientsConnected() == 1)
                {
                    WriteToLogWindow("Awaiting for the second player to connect...");
                }
                else if (GetClientsFieldFilled() == 0)
                {
                    WriteToLogWindow("Awaiting for players to place their ships...");
                }
            }
//        }
    }
    else if (Header == "fill")
    {
        WriteToLogWindow("Player \"" + Client.ClientName + "\" placed his/her ships on the field");
        /* once both players do that, server sends each client a message that it is this client's turn to fire.
                other client is still idle until receives fire message from server
                with coordinates of other player's shoot.
                Only after that it is his/her turn to fire and other's player turn to wait */

        SetClientsFieldFilled(GetClientsFieldFilled() + 1);

        emit FillShipsSig(Client.ClientName, Remainder);

        if (GetClientsFieldFilled() == 1)
            WriteToLogWindow("Awaiting for the second player to place his/her ships...");
        else if (GetClientsFieldFilled() == 2)
        {

            SendToAnotherClient(QString("name") + QString(Client.ClientName), Client.client_socket);

            if (Client.ClientName != ui->Player1Box->title())
            {
                SendToThisClient(QString("name") + ui->Player1Box->title(), Client.client_socket);
                WriteToLogWindow("The Game has begun! Time for the first player \"" + ui->Player1Box->title() + "\" to shoot!");
            }
            else if (Client.ClientName != ui->Player2Box->title())
            {
                SendToThisClient(QString("name") + ui->Player2Box->title(), Client.client_socket);
                WriteToLogWindow("The Game has begun! Time for the first player \"" + ui->Player2Box->title() +  "\" to shoot!");
            }

            NameFleetMap.find(RedPlayerName)->second.AdversaryName = BluePlayerName;
            NameFleetMap.find(BluePlayerName)->second.AdversaryName = RedPlayerName;

            SendToAnotherClient(QString("fire")+ QString("Your Turn"), Client.client_socket);
            SendToThisClient(QString("fire") + QString("Enemy turn"), Client.client_socket);
        }
    }
    else if (Header == "fire")
    {
        WriteToLogWindow("Player " + Client.ClientName + " fired a square with coordinates: " + Remainder );

        /* Mark squares as damaged on other player's Server window*/
        emit ChangeColorSig(Client.ClientName, Remainder);
        /* ......................................... */

        qDebug() << "We should wait until Color is set...";

        while (GetPendingColor() == "")
        {
            usleep(5000);
        }

        /* Color value is set depending on current color of the square */
        if (GetPendingColor() == "R")
        {
            QString GluedMessages;

            while (!ShipKilledIsEmpty())
            {
                if (GluedMessages.isEmpty())
                    GluedMessages = "fire" + GetAndPopShipKilled() + "R" + Client.ClientName;
                else
                    GluedMessages = GluedMessages + "|" + "fire" + GetAndPopShipKilled() + "R" + Client.ClientName;
            }
            qDebug() << "Going to send one big Glued Message:" << GluedMessages
                     << "to change all squares colors of a ship at once. Clients will parse them as separate";
            SendToThisClient(GluedMessages, Client.client_socket);
            SendToAnotherClient(GluedMessages, Client.client_socket);

            QString AdvName = NameFleetMap.find(Client.ClientName)->second.AdversaryName;
            qDebug() << "Check if" << AdvName << "still has any ships in the fleet";
            if (NameFleetMap.find(AdvName)->second.IsFleetCompletelyDestroyed())
            {
                qDebug() << AdvName << "fleet was destroyed! Send respective Victory/Defeat messages";
                WriteToLogWindow("Player \"" + Client.ClientName + "\" was victorious!");

                SendToThisClient(QString("over") + QString("Victory"), Client.client_socket);
                SendToAnotherClient(QString("over") + QString("Defeat"), Client.client_socket);

                /**/emit ClearGridsButNotConnectionsSig();
            }
        }
        else
        {
            SendToThisClient("fire" + Remainder + GetPendingColor() + Client.ClientName, Client.client_socket);
            SendToAnotherClient("fire" + Remainder + GetPendingColor() + Client.ClientName, Client.client_socket);
        }

        SetPendingColor("");
    }
    else if (Header == "bbye")
    {
        WriteToLogWindow("Player \"" + Client.ClientName + "\" is scared and disconnected!");

        /* notify another player that his/her adversary fled */
        SendToAnotherClient("bbye" + Client.ClientName, Client.client_socket);

        //WriteToLogWindow("Makes another player winner by default!");

        /* closing the connection and notifying other player that his/her adversary is left */

        //CloseConnection(Client.client_socket, true/*, GetQStrThreadId(std::this_thread::get_id())*/);
        qDebug() << "Stepping out of ParseMessage() function";
        return -999;
    }
    else
    {
        qDebug() << "Server received smth, but it is not of known type!";
        return -2;
    }

    Client.MessagesReceived++;
    qDebug() << "Number of messages, received by socket" << Client.client_socket << "is" << Client.MessagesReceived;

    return 0;

}
/*----------------------------------------------------------------------------------*/
void server::SendToAnotherClient(QString Msg, int ThisClientSocket)
{
    int AnotherClientSocket;

    Msg = Msg + '|'; /* Adding a delimeter in case two or more messages come as one to client's side */

    std::map<int, std::thread *>::iterator it;

    mu.lock();
    for (it = ClientSockAndThread.begin(); it != ClientSockAndThread.end(); ++it)
    {
        if ( it->first != ThisClientSocket && it->first != listen_sock)
        {
            AnotherClientSocket = it->first;
            write(AnotherClientSocket, Msg.toLatin1(), Msg.size());
            qDebug() << "Message" << Msg << "was sent to socket" << AnotherClientSocket;
            mu.unlock();
            return;
        }
    }

    mu.unlock();
    qDebug() << "Message was not sent as apparently second player is not connected";
}
/*----------------------------------------------------------------------------------*/
void server::SendToThisClient(QString Msg, int ThisClientSocket)
{
    Msg = Msg + '|';

    mu.lock();

    write(ThisClientSocket, Msg.toLatin1(), Msg.size());
    qDebug() << "Message" << Msg << "was sent to socket" << ThisClientSocket;
    mu.unlock();
    return;

}
/*----------------------------------------------------------------------------------*/
void server::SendToAllSlot(QString Msg)
{
    int ClientSocket;

    std::map<int, std::thread *>::iterator it;

    mu.lock();
    for (it = ClientSockAndThread.begin(); it != ClientSockAndThread.end(); ++it)
    {
        ClientSocket = it->first;

        if (ClientSocket != listen_sock)
        {
            //ClientSocket = it->first;
            mu.unlock();
            SendToThisClient(Msg, ClientSocket);
            mu.lock();
        }
    }

    mu.unlock();
    qDebug() << "Message was sent to all players who are connected to server";
}
