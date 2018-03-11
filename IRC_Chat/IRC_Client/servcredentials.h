#ifndef SERVCREDENTIALS_H
#define SERVCREDENTIALS_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class ServCredentials;
}

class ServCredentials : public QDialog
{
    Q_OBJECT

public:
    explicit ServCredentials(QWidget *parent = 0);
    ~ServCredentials();

public slots:

    void ShowServCrWindow(); /* called from Client window if we dissconnect (intentionally or not)*/

private slots:

    void on_OkButton_clicked();

    void on_CancelButton_clicked();

signals:

    void ShowLoginWindow(QString , QString); /* passing further Server's IP and Port */

    void OpenSocketClient(QString , QString );

    void KillRecvThread(bool ); /* gently kill receiving thread
                                in case we never reached Client's Main Window
                                but tried to send Sign-On request (meaning had to start the thread)*/

private:
    Ui::ServCredentials *ui;
    void SetDefaultIp(QString errmsg); /* restore to 127.0.0.1 */
    void SetDefaultPort(QString errmsg); /* restore to 2323 */
};

#endif // SERVCREDENTIALS_H
