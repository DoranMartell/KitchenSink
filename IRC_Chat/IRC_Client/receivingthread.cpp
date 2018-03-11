#include "receivingthread.h"

#include <QDebug>

ReceivingThread::ReceivingThread()
{
    memset(buff, '\0', sizeof(buff));
    setKillThread(false);

}
/*--------------------------------------------------------------------------------------------------------------*/
void ReceivingThread::run() /* nearly a twin of server's RWThread */
{
    int n;
    QString msgtype, date, username, actual_message;

    socklen_t serverlen;
    serverlen = sizeof(server);

    while (1)
    {
        n = recvfrom(connectionSocket, buff, BUFFSIZE, 0, (struct sockaddr *) &server, &serverlen);
        if (n == 0)
        {
            memset(buff, '\0', sizeof(buff));
        }
        else if (n > 0)
        {
            ParseMessage(msgtype, date, username, actual_message );
            QString message;

            if (msgtype == "message")
            {
                message = date + " [" + username + "]: " + actual_message;

                qDebug() << "client received" << n << "bytes:" << message;

                emit PrintMessageRecvThread(message);

            }
            else if (msgtype == "sign-on")
            {
                qDebug() << "Username: " << username;
                qDebug() << "Last 7 characters of incoming message:" << actual_message.right(7);
                qDebug() << "Last 9 characters of incoming message:" << actual_message.right(9);

                if (actual_message.right(7) == "online!")
                {
                    message = date + " [" + username + "]: " + "Connection established";

                    qDebug() << "Sign-On approved. Client received: " << message;

                    qDebug() << "Does actual message contain LastOnline timestamp?" << actual_message.mid(27, 2).toInt();
                    /* Pop "Last seen online" Message Box */
                    if (actual_message.mid(27, 2).toInt() != 0)
                    {
                        qDebug() << "Print Last Seen Online in separate window";
                        QString LastSeenOnline = "You were last seen online on " + actual_message.mid(27, 20);
                        emit LastSeenOnlineToGUI(LastSeenOnline);
                    }

                    emit SignOnConfirmedToLogInWindow(1);
                    qDebug() << "Signal to log-in window to set SignOnConfirmed to true was sent";
                    emit PrintMessageRecvThread(message);
                }
                else if (actual_message.right(9) == "Declined!")
                {
                    qDebug() << "Sign-On declined. Client received: " << message;

                    emit FailLogIn(actual_message);
                    qDebug() << "Signal to log-in window to print a reason why connection was declined is sent";
                    emit SignOnConfirmedToLogInWindow(2);
                }
                /* server actually answered with correct msgtype but....*/
                else /* ...server's response doesn't match client's request */
                {
                    qDebug() << "Neither decline, nor accept. Client received: " << message;

                    emit FailLogIn("Unexpected result of Sign-On request");
                    qDebug() << "Signal to log-in window to print a reason why connection was declined is sent";
                    emit SignOnConfirmedToLogInWindow(3);
                }

            }
            else if (msgtype == "log-off")
            {
                qDebug() << "Username: " << username;
                qDebug() << "Last 4 characters of incoming message:" << actual_message.right(4);

                if ( username == "server")
                {
                    qDebug() << "Seems that connection to server was lost! Close client window and open the one with credentials!";
                    emit ClientWindowDisconHandler("server"); /* notify GUI thread */

                }
            }
            else if (msgtype == "reg-new")
            {
                qDebug() << "Username: " << username;
                qDebug() << "Last 15 characters of incoming message:" << actual_message.right(15);

                if (actual_message.right(11) == "registered!")
                {
                    qDebug() << "User" << username << "was successfully registered";
                    emit setRegResponse(1, actual_message);
                }
                else if (actual_message.right(15) == "error occurred!")
                {
                    qDebug() << "Server received Register message, but Database error had occurred!";
                    emit setRegResponse(2, actual_message);
                }
                else if (actual_message.right(15) == "already exists!")
                {
                    qDebug() << "Server received Register message. Such username already exists in DB";
                    emit setRegResponse(2, actual_message);
                }
                else
                {
                    qDebug() << "Unexpected response on Register message!";
                    emit setRegResponse(2, actual_message);
                }
            }
            else
            {
                qDebug() << "Unexpected or incorrect message received from server (socket " << connectionSocket << ")";
            }

            memset(buff, '\0', sizeof(buff));
        }

        if (KillThread())
        {
            qDebug() << "Client's receiving thread is about to die...";
            return;
        }
    }
}
/*--------------------------------------------------------------------------------------------------------------*/
void ReceivingThread::ParseMessage(QString & mType, QString & dateTime, QString & uName, QString & acMess) /* twin of server's RWThread function */
{
    QString string(buff);

    qDebug() << "Received Raw Message: " << string;

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
bool ReceivingThread::KillThread() const
{
    return m_KillThread;
}
/*--------------------------------------------------------------------------------------------------------------*/
void ReceivingThread::setKillThread(bool KillThread)
{
    if (m_KillThread == KillThread)
        return;

    m_KillThread = KillThread;
    emit KillThreadChanged(KillThread);
}
