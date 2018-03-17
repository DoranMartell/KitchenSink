#ifndef RECEIVINGTHREAD_H
#define RECEIVINGTHREAD_H

#include <QObject>
#include <QThread>

#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFSIZE 256

class ReceivingThread : public QThread
{
    Q_OBJECT
    Q_PROPERTY(bool KillThread READ KillThread WRITE setKillThread NOTIFY KillThreadChanged)

    bool m_KillThread;

public:

    ReceivingThread();

    struct sockaddr_in server; /* server address structure */

    char buff[BUFFSIZE]; /* Raw data came from server
                            (responses to sign-on/log-off or just) messages from other clients */

    void run(); /* overload QThread method */

    int connectionSocket;

    bool KillThread() const;

    void ParseMessage(QString & mType, QString & dateTime, QString & uName, QString & acMess); /* twin of the same function
                                                                                    of server's RWThread */

public slots:

    void setKillThread(bool KillThread);

signals:

    void KillThreadChanged(bool KillThread);

    void PrintMessageRecvThread(QString ); /* let GUI thread print a message in the TextEdit */

    void SignOnConfirmedToLogInWindow(int ); /* hints main thread if server approved/declined access */

    void FailLogIn(QString ); /* sends signal to loginwindow to pop QMessageBox::critical window */

    void ClientWindowDisconHandler(QString ); /* sends signal to GUI to close window/thread/socket
                                                    and to show error and AskCredentials window */
    void LastSeenOnlineToGUI(QString ); /* sends signal to main window to pop QMessageBox::info window */

    void setRegResponse(short , QString ); /* notifies RegNewWindow about results of server response */

};

#endif // RECEIVINGTHREAD_H
