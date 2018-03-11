#ifndef SERVER_H
#define SERVER_H

#include <QMainWindow>
#include <QCloseEvent>
#include "connthread.h"

namespace Ui {
class Server;
}

class Server : public QMainWindow
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = 0);
    ~Server();

    ConnThread ConnectionThread;

signals:

    void KillConnectionThread(bool );
    void KillRWThread(bool );

public slots:

    void PrintMessInWindow(QString ); /* print received message in text box */
    void ErrorMess(QString); /* open error window, kill thread and close application */
    void showWindow(); /* shows Server window and starts a thread */

    void NewNumberOfConnections(int , ConnThread::ClientsParams ** ); /* new connection established - update GUI, let it know new signals */
    void StatBarNumOfConn(); /* only shows number of connections in status bar, but is related to ReadWrite thread, unlike NewNumberOfConnections()
                                which is related to ConnThread. It was needed as NewNumberOfConnections() involved a big chain of objects involved
                                thus causing deadlock */


    void CloseServerWindow(QString );

private slots:

    void on_ShutDownButton_clicked();

    void on_ClearButton_clicked();

    void closeEvent(QCloseEvent * );

private:
    Ui::Server *ui;

};

#endif // SERVER_H
