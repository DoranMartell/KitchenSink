#include "rwthread.h"

#include <stdlib.h>
#include <time.h>

#include <QDebug>

RWThread::RWThread()
{
    memset(buff, '\0', sizeof(buff)); /* otherwise will print hellish symbols in first message */
    threadSubscript = -1;
    connectSocket = -1;

    connection_name = QString::number(qrand() % 100000);
    qDebug() << "Database connection name is" << connection_name << "for a new ReadWriteThread";

    /* Connection to Database per thread - isn't it too much ?? */
    /* each connection name should be different from others,
                            otherwise only one default_connection would be used */
    db = QSqlDatabase::addDatabase("QMYSQL", connection_name);
    db.setHostName("localhost");
    //db.setPort(3306); /* this port is default anyway*/
    db.setDatabaseName("IRC_Server"); /* Schema was created for the user */
    db.setUserName("IRC_Server"); /* user was created via CREATE USER ...*/
    db.setPassword("IRC_Server"); /* ... IDENTIFIED BY ... by root user of server */

    if (!db.open())
    {
        qDebug() << db.lastError().text();
    }
    else
    {
        qDebug() << "Successfully connected to database!";
    }

     QSqlQuery set_query = QSqlQuery(db);
     set_query.exec("set autocommit=0");
     qDebug() << "Disable autocommits on MySql server";
     set_query.finish();
}
/*--------------------------------------------------------------------------------------------------------------*/
void RWThread::run()
{
    int n;
    QString msgtype, date, username, actual_message;

    //fcntl(connectSocket, F_SETFL, O_NONBLOCK);

    while (1)
    {
        qDebug() << "Still working: Thread" << threadSubscript << "socket" << connectSocket;

        n = read(connectSocket, buff, BUFFSIZE);
        //qDebug() << "Still working: Thread" << threadSubscript << "socket" << connectSocket << "n =" << n;
        if (n < 0)
        {
            //emit errMessRWThread("Error reading from socket!\n");
            //setRWKilled(true);
            //sleep(1);
        }
        else if (n == 0)
        {
            memset(buff, '\0', sizeof(buff));
            qDebug() << "Thread " << threadSubscript << "socket" << connectSocket;

            usleep(500); // probably it is waiting to die

        }
        else if (n > 0)
        {
            ParseMessage(msgtype, date, username, actual_message );
            memset(buff, '\0', sizeof(buff));

            if (msgtype == "message")
            {
                QString message("Thread number " + QString::number(threadSubscript) + " [socket " + QString::number(connectSocket) + "]:\n"
                                + date + " [" + username + "]: " + actual_message);

                qDebug() << "server received" << n << "bytes:" << message;

                emit PrintMessageRWThread(message);

                BuildRawMessage(msgtype, username, actual_message); /* before sending back to client */

                qDebug() << "RawMsg is about to be sent...";

                /* sending message to all clients */
                QString Msg(RawMsg);
                emit SendToAllClients(Msg);

            }
            else if (msgtype == "sign-on")
            {
                QString message("Thread number " + QString::number(threadSubscript) + " [socket " + QString::number(connectSocket) + "]:\n"
                                + date + " [" + username + "] wants to join.\nChecking authentication data...");

                qDebug() << "Message to be printed:" << n << "bytes:" << message;

                emit PrintMessageRWThread(message);
                qDebug() << "Right after PrintMessageRWThread";

                int DBResponse;
                DBResponse = CheckInDB(username, actual_message); /* when msg_type is "sign-on", password comes in actual_message */

               /* Temporary just checking if login is "admin" or "Joey" or "Chandler" */
               /* There should be request to database */
               if (QString::compare(username, "admin") && QString::compare(username, "Joey") && QString::compare(username, "Chandler")
                     && (DBResponse == -2))
               {
                   /* && - because compare() returns 0 if strings are equal*/
                   message = "User \"" + username + "\" is not registered! Connection Declined!";
                   qDebug() << "Going to send this to client: " << message;
                   emit PrintMessageRWThread(message);

                   BuildRawMessage(msgtype, username, message); /* before sending back to client */
                   memcpy(buff, RawMsg, sizeof(buff));

                   /* sending message to client */
                   if ( (n = write(connectSocket, buff, BUFFSIZE)) < 0)
                   {
                       qDebug() << "Error writing to socket " << connectSocket;
                   }
                   memset(buff, '\0', sizeof(buff));

                   /* Close socket and quit thread */
                    emit RemoveClientNode(threadSubscript);

                   //qDebug() << "Kill thread" << threadSubscript << "! Socket " << connectSocket ;
                   //setRWKilled(true);
                   //::shutdown(connectSocket, SHUT_RDWR);
                   //::close(connectSocket);
                   //return;

               }
               else if ( (!QString::compare(username, "admin") && QString::compare(actual_message, "admin")) ||
                        (!QString::compare(username, "Joey") && QString::compare(actual_message, "pizza")) ||
                        (!QString::compare(username, "Chandler") && QString::compare(actual_message, "Monica"))
                         || (DBResponse == -1))
               {
                   message = "Wrong password for user [" + username + "]! Connection Declined!";
                   qDebug() << message;
                   emit PrintMessageRWThread(message);

                   BuildRawMessage(msgtype, username, message); /* before sending back to client */
                   memcpy(buff, RawMsg, sizeof(buff));

                   /* sending message to client */
                   if ( (n = write(connectSocket, buff, BUFFSIZE)) < 0)
                   {
                       qDebug() << "Error writing to socket " << connectSocket;
                   }
                   memset(buff, '\0', sizeof(buff));

                   /* Close socket and quit thread */
                    emit RemoveClientNode(threadSubscript);

                   //qDebug() << "Kill thread" << threadSubscript << "! Socket " << connectSocket ;
                   //setRWKilled(true);
                   //::shutdown(connectSocket, SHUT_RDWR);
                   //::close(connectSocket);
                   //return;
               }
               else if ( (!QString::compare(username, "admin") && !QString::compare(actual_message, "admin")) ||
                        (!QString::compare(username, "Joey") && !QString::compare(actual_message, "pizza")) ||
                        (!QString::compare(username, "Chandler") && !QString::compare(actual_message, "Monica")) ||
                        (DBResponse == 1) )
               {


                   emit AssignNickToClient(username, threadSubscript);

                   isOnlineAlready = false;

                   emit CheckOnline(username, threadSubscript, &isOnlineAlready);

                   if (isOnlineAlready)
                   {
                       message = "User [" + username + "] already has an active connection to server! Connection Declined!";
                       qDebug() << message;
                       emit PrintMessageRWThread(message);

                       BuildRawMessage(msgtype, username, message); /* before sending back to client */
                       memcpy(buff, RawMsg, sizeof(buff));

                       /* sending message to client */
                       if ( (n = write(connectSocket, buff, BUFFSIZE)) < 0)
                       {
                           qDebug() << "Error writing to socket " << connectSocket;
                       }
                       memset(buff, '\0', sizeof(buff));

                       /* Close socket and quit thread */
                       emit DecreaseNumberOfConnections(); /* first change statusBar as othewise we delete thread before it could send a signal */
                       emit RemoveClientNode(threadSubscript);

                   }
                   else
                   {
                       /* Server also sends LASTONLINE value to client in "sign-on" message */
                       if (username != "admin" && username != "Chandler" && username != "Joey")
                       {
                           message = date + " After " + LastSeenOnline + " [" + username + "] is back online!";
                       }
                       else
                       {
                           message = date + " [" + username + "] is online!";
                       }
                       /* should reflect this in status bar also */
                       qDebug() << message;
                       emit PrintMessageRWThread(message); 

                       BuildRawMessage(msgtype, username, message); /* before sending back to client */
                       memcpy(buff, RawMsg, sizeof(buff));

                       /* sending message to client */
                       if ( (n = write(connectSocket, buff, BUFFSIZE)) < 0)
                       {
                           qDebug() << "Error writing to socket " << connectSocket;
                       }

                       qDebug() << "Message sent to " << username << " containing " << n << "bytes";
                       memset(buff, '\0', sizeof(buff));

                       QString message = username + " joined the chat";

                       msgtype = "message";
                       BuildRawMessage(msgtype, username, message);
                       QString Msg(RawMsg);
                       emit SendToAllClients(Msg);
                    }
               }
               else if (DBResponse == -3)
               {
                   qDebug() << "Notify all clients about Database error";
                   message = "Database error occurred while user [" + username + "] tried to connect! Connection Declined!";

                   BuildRawMessage(msgtype, username, message); /* before sending back to client */
                   memcpy(buff, RawMsg, sizeof(buff));

                   /* sending message to client */
                   if ( (n = write(connectSocket, buff, BUFFSIZE)) < 0)
                   {
                       qDebug() << "Error writing to socket " << connectSocket;
                   }
                   memset(buff, '\0', sizeof(buff));

                   /* Close socket and quit thread */
                   emit RemoveClientNode(threadSubscript);
               }

            }
            else if (msgtype == "log-off")
            {
                qDebug() << "Log-Off message from " << username << "received";
                QString message("Thread number " + QString::number(threadSubscript) + " [socket " + QString::number(connectSocket) + "]:\n"
                                + date + " [" + username + "] is leaving conversation...");
                emit PrintMessageRWThread(message);

                /* building and sending Log-Off response to client */
                msgtype = "message";
                message = username + " left this chat";
                BuildRawMessage(msgtype, username, message); /* before sending back to client */
                //memcpy(buff, RawMsg, sizeof(buff));

                /* Notifying all clients that certain client is gone*/
                QString Msg = RawMsg;
                emit SendToAllClients(Msg);

                /* Update LastSeenOnline for this user in Database - it is done in RemoveClientNode*/
                //UpdateLastOnline(username);

                /* send signal to ConnectionThread to delete this thread from list of opened connections */
                emit DecreaseNumberOfConnections(); /* first change statusBar as othewise we delete thread before it could send a signal */
                emit RemoveClientNode(threadSubscript);


                //qDebug() << "About to close socket " << connectSocket << " and kill thread " << threadSubscript;
                //setRWKilled(true);
                //::shutdown(connectSocket, SHUT_RDWR);
                //::close(connectSocket);

            }
            else if (msgtype == "reg-new")
            {
                qDebug() << "New user with nickname" << username << "wants to be registered";
                QString message;

                /* first check (with select query) if there is no such user prior to inserting same*/
                int result = CheckInDB(username, actual_message);
                msgtype = "reg-new";

                if (result == 1 || result == -1)
                {
                    message = "User [" + username + "] already exists!";
                    result = 0;
                }
                else if (result == -2)
                {
                    result = InsertToDB(username, actual_message);

                    if (result == 1)
                    {
                        result = CheckInDB(username, actual_message);
                        if (result == 1)
                        {
                            message = "User [" + username + "] was successfully registered!";

                            /* after registration associate this thread with registered client */
                            emit AssignNickToClient(username, threadSubscript);

                        }

                        result = 0;
                    }
                }

                if (result == -3)
                {
                    message = "DataBase error occured!";
                }

                BuildRawMessage(msgtype, username, message);
                memcpy(buff, RawMsg, sizeof(buff));

                /* sending message to client */
                if ( (n = write(connectSocket, buff, BUFFSIZE)) < 0)
                {
                    qDebug() << "Error writing to socket " << connectSocket;
                }

                emit PrintMessageRWThread("Registration: " + message);
                qDebug() << "Message sent to " << username << " containing " << n << "bytes";
                memset(buff, '\0', sizeof(buff));

                if (result == 1)
                {
                    QString message = "New user [" + username + "] was registered!";

                    msgtype = "message";
                    BuildRawMessage(msgtype, username, message);
                    QString Msg(RawMsg);
                    emit SendToAllClients(Msg);
                }

                /* Close socket and quit thread */
                emit DecreaseNumberOfConnections();
                emit RemoveClientNode(threadSubscript);

            }
            else
            {
                qDebug() << "Unexpected or incorrect message received from " << "Thread number " << threadSubscript << " [socket " << connectSocket;
            }

            memset(buff, '\0', sizeof(buff));


        }

        //qDebug() << "Still working: Thread" << threadSubscript << "socket" << connectSocket;

        if (RWKilled())
        {
            qDebug() << "Reading-Writing thread" << threadSubscript << "is going to die!";
            return;
        }
    }
}

/*--------------------------------------------------------------------------------------------------------------*/
void RWThread::setConnSocket(int socket_value, int subscript_value)
{
    connectSocket = socket_value;
    threadSubscript = subscript_value;

//    emit ChngNumOfConn(threadSubscript+1);
}
/*--------------------------------------------------------------------------------------------------------------*/
void RWThread::ParseMessage(QString & mType, QString & dateTime, QString & uName, QString & acMess)
{
    QString string(buff);

    qDebug() << "Raw Message: " << string;

    mType = string.mid(0, 7);
    qDebug() << "Parsed Message Type: " << mType;

    dateTime = string.mid(23, 20);
    qDebug() << "Parsed date and time: " << dateTime;

    uName = string.mid(7, 16);
    uName = uName.simplified();
    qDebug() << "Parsed Username: " << uName;

    acMess = string.mid(7 + 16 + 20, BUFFSIZE - 16 - 7 - 20);
    qDebug() << "Parsed Actual Message: " << acMess;
}
/*--------------------------------------------------------------------------------------------------------------*/
void RWThread::BuildRawMessage(QString msgtype, QString username, QString actual_message)
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
    Trim(actual_message, (BUFFSIZE - 7 - 16 - 20)); /* actual_message is buffsize without msgtype and username and date */

    qDebug() << "Padded username: " << username;

    QString all_combined(msgtype + username + date + actual_message);

    strcpy(RawMsg, all_combined.toLatin1());

    qDebug() << "Raw Message: " << RawMsg;

}
/*--------------------------------------------------------------------------------------------------------------*/
void RWThread::Pad(QString &whatever, int lengthLack)
{
    whatever = whatever.leftJustified(lengthLack, ' ' );
}
/*--------------------------------------------------------------------------------------------------------------*/
void RWThread::Trim(QString &whatever, int lengthExcess)
{
    whatever.chop(whatever.size() - lengthExcess); /* FROM THE BEGINNING, NOT THE END!! */
}

/*--------------------------------------------------------------------------------------------------------------*/
bool RWThread::RWKilled() const
{
    return m_RWKilled;
}
/*--------------------------------------------------------------------------------------------------------------*/
void RWThread::setRWKilled(bool RWKilled)
{
    if (m_RWKilled == RWKilled)
        return;

    m_RWKilled = RWKilled;
}
/*--------------------------------------------------------------------------------------------------------------*/
int RWThread::CheckInDB(QString InputUname, QString InputPass)
{
    qDebug() << "Client sent credentials to verify on DB side";
    qDebug() << "Username:" << InputUname;
    qDebug() << "Password:" << InputPass;

    QSqlQuery select_query = QSqlQuery(db);

    int FetchedId;
    QString FetchedUname;
    QString FetchedPass;
    //QString FetchedLastOnline;

    //bool UnameMatched = false;
    //bool PassMatched = false;

    select_query.prepare("select ID, Username, Passwrd, DATE_FORMAT(lastonline, '%Y-%b-%d %H:%i:%s') as lastonline from Users where Username = :uname");
    select_query.bindValue(":uname", InputUname);

    if (!select_query.exec())
    {
        qDebug() << select_query.lastError().databaseText();
        qDebug() << select_query.lastError().driverText();
        //QMessageBox::critical(this, "Database Error", select_query.lastError().databaseText());
        emit PrintMessageRWThread(select_query.lastError().databaseText());

        return -3; /* database error */
    }

    qDebug() << "Fetched table Users from Database:";
    select_query.next();

    if (!select_query.isNull(2))
    {

        FetchedId = select_query.value("ID").toInt();
        FetchedUname = select_query.value("Username").toString();
        FetchedPass = select_query.value("Passwrd").toString();
        LastSeenOnline = select_query.value("lastonline").toString();
        qDebug() << "ID:" << FetchedId << "UserName:" << FetchedUname << "Password:" << FetchedPass;

        select_query.finish();

        if (FetchedPass == InputPass)
        {
            qDebug() << "Username and Password MATCHED with ones from Database for" << InputUname;

            return 1; /* success */
        }
        else
        {
            //QString WrongPassMess = "Wrong Password for user \"" + InputUname + "\"!";
            //QMessageBox::critical(this, "Log-On failed", WrongPassMess);
            //emit PrintMessageRWThread(WrongPassMess);

            return -1; /* fail - wrong password */
        }
    }
    else
    {
        //QString WrongUNameMess = "No user \"" + InputUname + "\" found in Database!";
        //QMessageBox::critical(this, "Log-On failed", WrongUNameMess);
        //emit PrintMessageRWThread(WrongUNameMess);

        select_query.finish();

        return -2; /* fail - no such user */
    }

    return 0; /* is this even possible ? */
}

/*--------------------------------------------------------------------------------------------------------------*/
void RWThread::UpdateLastOnline(QString u)
{
    QSqlQuery check_query = QSqlQuery(db);

    check_query.prepare("select Username from Users where Username = :uname;");
    check_query.bindValue(":uname", u);

    if (!check_query.exec())
    {
        qDebug() << "Error while checking if username" << u << "has records in Users table";
        qDebug() << check_query.lastError().databaseText();
        qDebug() << check_query.lastError().driverText();

        emit PrintMessageRWThread(check_query.lastError().databaseText());
        return;
    }

    check_query.next();

    if (!check_query.isNull(1))
    {
        qDebug() << "No user" << u << " in database, so can't update its lastonline value.";
        return;
    }

    check_query.finish();

    qDebug() << "QSqlDriver Transaction supported?" << db.driver()->hasFeature(QSqlDriver::Transactions);

    qDebug() << "UPDATE: QSqlDatabase::database.transaction() = " << db.transaction();

    QSqlQuery update_query = QSqlQuery(db);
    if (!update_query.prepare("update Users set lastonline = now() where UserName = :uname_bind;"))
    {
        qDebug() << "Error while preparing UPDATE sql statement";
    }

    update_query.bindValue(":uname_bind", u);

    //QString qr;
    //qr = "update Users set lastonline = now() where UserName = " + QString("'") + u + QString("'");
    //qDebug() << qr;
    //update_query.prepare(qr);

    if (!update_query.exec())
    {
        qDebug() << "Error while updating LastSeenOnline for" << u;
        qDebug() << update_query.lastError().databaseText();
        qDebug() << update_query.lastError().driverText();

        emit PrintMessageRWThread(update_query.lastError().databaseText());
        return;
    }

    update_query.finish(); /* should finish the query before committing */

    qDebug() << "Commiting update statement: " << db.commit();

    check_query.prepare("select DATE_FORMAT(lastonline, '%Y-%b-%d %H:%i:%s') as lastonline from Users where UserName = :uname_bind");
    check_query.bindValue(":uname_bind", u);

    if (!check_query.exec())
    {
        qDebug() << "Error while checking LastSeenOnline for" << u;
        qDebug() << check_query.lastError().databaseText();
        qDebug() << check_query.lastError().driverText();
        return;
    }

    check_query.next();
    qDebug() << "Compare with present time:" << check_query.value("lastonline").toString();

    check_query.finish();

    if (db.isOpen())
    {
        db.close();
    }
    QSqlDatabase::removeDatabase(connection_name);

}
/*--------------------------------------------------------------------------------------------------------------*/
int RWThread::InsertToDB(QString SuggestedUname, QString SuggestedPass)
{
    qDebug() << "Insert into Users table following";
    qDebug() << "Username:" << SuggestedUname;
    qDebug() << "Password:" << SuggestedPass;

    QSqlQuery insert_query = QSqlQuery(db);

    insert_query.prepare("insert into Users (UserName, Passwrd) values (:uname, :passwrd);");
    insert_query.bindValue(":uname", SuggestedUname);
    insert_query.bindValue(":passwrd", SuggestedPass);

    qDebug() << "QSqlDriver Transaction supported?" << db.driver()->hasFeature(QSqlDriver::Transactions);
    qDebug() << "INSERT: QSqlDatabase::database.transaction() = " << db.transaction();
    if (!insert_query.exec())
    {
        qDebug() << insert_query.lastError().databaseText();
        qDebug() << insert_query.lastError().driverText();
        //QMessageBox::critical(this, "Database Error", select_query.lastError().databaseText());
        emit PrintMessageRWThread(insert_query.lastError().databaseText());

        return -3; /* database error */
    }

    qDebug() << "Insert query for user" << SuggestedUname << "was executed. Let's do select query to make sure";

    insert_query.finish(); /* should finish the query before committing */

    qDebug() << "Commiting insert statement: " << db.commit();

    return 1; /* success */
}

/*--------------------------------------------------------------------------------------------------------------*/
void RWThread::EchoReflected()
{
    qDebug() << "Thread " << threadSubscript << "socket" << connectSocket << "is here";
}

