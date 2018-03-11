#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include <QMessageBox>

#include <unistd.h> /* for sleep function */
#include "receivingthread.h"
#include "regnewwindow.h"

namespace Ui {
class LogInWindow;
}

class LogInWindow : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(int SignOnConfirmed READ SignOnConfirmed WRITE setSignOnConfirmed NOTIFY SignOnConfirmedChanged)

public:
    explicit LogInWindow(QWidget *parent = 0);
    ~LogInWindow();

    void SendSignOn(QString , QString ); /* Should send message to server
                                           so it would check login and passwrd and allow/deny access */

    QString servIp;
    QString servPortNo;

    int SignOnConfirmed() const;

public slots:

    void ShowWindow(QString, QString); /* has IP and Port as arguments */

    void setSignOnConfirmed(int SignOnConfirmed);

    void ErrorLogIn(QString ); /* shows separate message box with error description
                                    and erases what was written to lineEdits with login and passwrd */
    void Enable(); /* makes window unable after registration */

private slots:
    void on_CancelButton_clicked();

    void on_LogInButton_clicked();

    void on_RegisterButton_clicked();

signals:

    void SendMessageClient(QString, QString, QString ); /* so could send SignOn message */

    void ShowClientWindow(QString ); /* displaying main window */
    void StepBack(); /* Returns to Server Credentials window */

    void OpenSocketClient(QString, QString, bool & );

    void SignOnConfirmedChanged(int SignOnConfirmed);

    void RegWndShow(QString , QString ); /* servern ip and port as arguments */

private:
    Ui::LogInWindow *ui;
    int m_SignOnConfirmed;

};

#endif // LOGINWINDOW_H
