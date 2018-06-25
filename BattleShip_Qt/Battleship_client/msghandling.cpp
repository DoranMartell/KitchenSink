#include "client.h"
#include "ui_client.h"

/*----------------------------------------------------------------------------------*/
void client::SendNameSlot(QString name)
{
    QString Msg = BuildMessage("name", name);
    SendMessage(Msg);
    YourName = name;

    qDebug() << "Player's name was set to" << name;

}
/*----------------------------------------------------------------------------------*/
void client::SendBbyeSlot()
{
    QString Msg = BuildMessage("bbye", YourName);
    SendMessage(Msg);

    qDebug() << "\"bbye\" message was sent to the server";

}
/*----------------------------------------------------------------------------------*/
void client::SendFillSlot()
{
    /* anyway, all vectors with coordinates are global variables,
        so Build() function will take them directly, not from argument */
    QString Msg = BuildMessage("fill", "");
    SendMessage(Msg);

    qDebug() << "\"fill\" message was sent to the server";

}
/*----------------------------------------------------------------------------------*/
void client::SendFireSlot(QString Coord)
{
    QString Msg = BuildMessage("fire", Coord);
    SendMessage(Msg);

    ui->FireButton->setEnabled(false);

    qDebug() << "\"fire\" message was sent to the server";

}
/*----------------------------------------------------------------------------------*/
QString client::BuildMessage(QString Header, QString Remainder)
{
    QString Msg;

    if (Header == "name")
    {
        Msg = Header + Remainder;
        //Msg += '|'; /* delimeter character */
        return Msg;
    }
    else if (Header == "fill")
    {
        qDebug() << "Building \"fill\" message...";

        Msg = Header /*+ " " + YourName + " "*/
                     + "Battleship " + GetCoords("BS")
                     + "Cruisers " + GetCoords("CR")
                     + "Destroyers " + GetCoords("DS")
                     + "Torpedo Boats " + GetCoords("TB");
        //Msg += '|';
        qDebug() << "Message Built:" << Msg;
        return Msg;

    }
    else if (Header == "fire")
    {
        qDebug() << "Building \"fire\" message...";
        Msg = Header + Remainder;
        //Msg += '|';
        qDebug() << "Message Built:" << Msg;
        return Msg;

    }
    else if (Header == "bbye")
    {
        Msg = Header + Remainder; /* in case of BBYE message remainder is a name of disconnecting Player,
                                        so the server would know whose connection to close. Is it necessary? */
        //Msg += '|';
        return Msg;
    }

    return "Sorry";
}
/*----------------------------------------------------------------------------------*/
void client::SendMessage(QString Msg)
{
    qDebug() << "Message is going to be sent by the thread with ID" << GetQStrThreadId(std::this_thread::get_id());

    mess_mu.lock();

    sendto(server_sock, Msg.toLatin1(), Msg.size(), 0, (struct sockaddr *) &ServerDetails, sizeof(ServerDetails));

    qDebug() << "Message" << Msg << "was sent to server";

    mess_mu.unlock();
}
/*----------------------------------------------------------------------------------*/
void client::ReceiveMess()
{
    qDebug() << "Receiving thread started with ID" << GetQStrThreadId(std::this_thread::get_id());

    int n;
    QString Mess;
    char buff[BUFFSIZE];
    memset(buff, '\0', BUFFSIZE);

    int Serv_len = sizeof(ServerDetails);

    while (1)
    {
        if ( (n =
              recvfrom(server_sock, buff, BUFFSIZE, 0, (struct sockaddr *) &ServerDetails, (socklen_t *) &Serv_len)
             ) <= 0)
        {
            qDebug() << "If we are here, most probably, server or client was disconnected";
            SetServerUp(false);
            emit Flee();
            return;
        }
        else
        {
            //mess_mu.lock();

            Mess = QString(buff);
            qDebug() << "We received the following message:" << Mess;

            AmountOfSubsequentMessages = 1;
            ParseMessage(Mess);
            memset(buff, '\0', BUFFSIZE);
            Mess.clear();
        }

       // mess_mu.unlock();
       // usleep(3000);
    }
}
/*----------------------------------------------------------------------------------*/
void client::ParseMessage(QString Msg)
{
    QString Header = Msg.left(4);
    QString Remainder = Msg.remove(0, 4);
    int Delimeter;

    qDebug() << "Parsed: Header:" << Header;
    qDebug() << "Parsed: Remainder:" << Remainder;

    short MessageNumber = AmountOfSubsequentMessages;
    qDebug() << "It's the" << MessageNumber << "message in a row";

    /* Two messages from server might come as one! Need to handle it! */
    /*while*/if ((Delimeter = Remainder.indexOf(QChar('|'))) != -1 )
    {
        qDebug() << "Delimeter was found on position" << Delimeter << "of Remainder";

        qDebug() << "Remainder size is" << Remainder.size();
        if (Delimeter == Remainder.size() - 1)
        {
            qDebug() << "Delimeter was found in the end of string. We are good";
            Remainder.remove(Delimeter, 1); /* removing delimeter character from the back of the message */
            //break;
        }
        else
        {
            QString NewMsg = Remainder.right(Remainder.size() - (Delimeter + 1));
            Remainder = Remainder.left(Delimeter);

            qDebug() << "The message was split! Remainder of first message:" << Remainder;
            qDebug() << "Seem to be the second message:" << NewMsg;

            AmountOfSubsequentMessages++;
            ParseMessage(NewMsg);
        }
    }

    if (Header == "name")
    {
        /* Eiter other player sent us his/her name or server informed us that name should be typed again */
        if (Remainder == "error")
        {
            qDebug() << "Apparently, another player chose the same name first!";
            SetNoNameReply(false);
            setNameOk(false);
            //emit hideFillGridWindowSig();
            //emit showAskNameWindowSig();
        }
        else if (Remainder == "ok")
        {
            qDebug() << "Server acknowledged Player's name";
            SetNoNameReply(false);
            setNameOk(true);
        }
        else
        {
            AdversaryName = Remainder;
            qDebug() << "Message with adversary name is received:" << AdversaryName;
            qDebug() << "It means that" << AdversaryName << "placed ships on the grid";
        }
    }
    else if (Header == "fire")
    {
        if (Remainder == "Your Turn")
        {
            qDebug() << "First fire message received, your turn";
            emit writeToStatusBarSig("You shoot first");
            SetStartGame(true);

            /* was added specifically for the case its not the first game of this player */
            ui->FireButton->setEnabled(true);
        }
        else if (Remainder == "Enemy turn")
        {
            qDebug() << "First fire message received, waiting for your enemy to shoot...";
            emit writeToStatusBarSig("Waiting for your enemy to shoot...");

            SetStartGame(true);

            /* button will be enabled once the first player makes a shot */
                /* However, if he/she hits your ship, you will have to wait again */
            ui->FireButton->setEnabled(false);
        }
        else
        {
            /* Remainder contains Coordinates and the color, to which the square should be painted */
            /* if the color is Red or Yellow, it is your turn to shoot again */
            QString Coordinate = Remainder.left(3);
            QString Color;
            QString Who;

            /* NOT NECESSARILY ON ENEMY GRID, CODE IS THE SAME FOR BOTH!!*/
            if (Coordinate.right(1) == "0")
            {
                qDebug() << "Server acknowledged you hit" << Coordinate << "square on enemy's grid";
                Color = Remainder.remove(0, 3).left(1);
                qDebug() << "You should paint the square into" << Color << "color";
                Who = Remainder.remove(0, 1);
            }
            else
            {
                Coordinate = Remainder.left(2);
                qDebug() << "Server acknowledged you hit" << Coordinate << "square on enemy's grid";
                Color = Remainder.remove(0, 2).left(1);
                qDebug() << "You should paint the square into" << Color << "color";
                Who = Remainder.remove(0, 1);
            }

            SetTheTurn(Who, Color); /* depending on who was shooting and how successful, the turn is determined */
            ColorTheSquare(Who, Coordinate, Color, MessageNumber);
        }
    }
    else if (Header == "over")
    {
        GameIsNotOver = false;
        if (Remainder == "Victory")
        {
            qDebug() << "Victory Message was received from the server! Congratulations!";
            emit writeToStatusBarSig("You destroyed all enemy ships! Congratulations!");
        }
        else if (Remainder == "Defeat")
        {
            qDebug() << "Defeat Message was received from the server! All is lost!";
            emit writeToStatusBarSig("Your fleet was entirely destroyed! All is lost!");
        }
        emit showEOGWindowSig(YourName, Remainder);
    }
    else if (Header == "bbye")
    {
        if (Remainder == AdversaryName)
        {
            qDebug() << "BBYE message received from another Player, meaning he/she is left";

            /* UNLESS ANOTHER PLAYER ALREADY WON! IN THAT CASE WE SHOULD NOT GET BOTH DEFEAT AND
                        VICTORY MESSAGES!! */
            if (GameIsNotOver)
            {
                emit writeToStatusBarSig(AdversaryName + "got scared and cowardly fled! Congratulations!");
                emit showEOGWindowSig(YourName, "Victory");
            }
        }
        else if (Remainder == "server")
        {
             qDebug() << "BBYE message received from server, meaning it was shutdown";

             SetServerUp(false);

             qDebug() << "Closing client application...";
             emit Flee(); /* both FillGrid and Client have slots for this signal */
        }
    }
}
/*----------------------------------------------------------------------------------*/
void client::SetTheTurn(QString WhoShot, QString Color)
{
    if (WhoShot == YourName)
    {
        qDebug() << "You should paint on your enemy's grid";

        if (Color == "R" || Color == "Y") /* alternatively, the color can be G for Grey */
        {
            qDebug() << "As you hit the enemy's ship, it is your turn to shoot again!";
            //SetYourTurn(true);
            ui->FireButton->setEnabled(true);
        }
        else
        {
            qDebug() << "You missed! Time for enemy to shoot!";
            //SetYourTurn(false);
            ui->FireButton->setEnabled(false);
        }

    }
    else if (WhoShot == AdversaryName)
    {
        qDebug() << "Server sent coordinates of Square which the enemy shot!";

        if (Color == "R" || Color == "Y") /* alternatively, the color can be G for Grey */
        {
            qDebug() << "As enemy hit your ship, it is NOT your turn now!";
            //SetYourTurn(false);
            ui->FireButton->setEnabled(false);
        }
        else
        {
            qDebug() << "Your enemy missed! It is your turn to shoot!";
            //SetYourTurn(true);
            ui->FireButton->setEnabled(true);
        }
    }

}


