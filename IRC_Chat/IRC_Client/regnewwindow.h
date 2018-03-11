#ifndef REGNEWWINDOW_H
#define REGNEWWINDOW_H

#include <QDialog>
#include <QMessageBox>
#include <QDebug>

#include <QCloseEvent> /* for custom closeEvent() */

#include <unistd.h> /* for sleep function */

namespace Ui {
class RegNewWindow;
}

class RegNewWindow : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(short RegResponse READ RegResponse WRITE setRegResponse NOTIFY RegResponseChanged)

public:
    explicit RegNewWindow(QWidget *parent = 0);
    ~RegNewWindow();

    /* User Input in LineEdits: */
    QString DesirblUname;
    QString DesirblPass;
    QString ConfPasswrd;

    /* taken from loginwindow */
    QString servIp;
    QString servPortNo;

    /* Check if field is Empty or longer than 16 characters */
    bool FieldValueAllowed(QString , QString ); /* 1st argument - field value, 2nd - field name */

    short RegResponse() const;

signals:

    void OpenSocketRegWind(QString , QString, bool & ); /* twin of OpenSocketClient */

    void SendRegMessage(QString, QString, QString ); /* sends Register New User message to server */

    void RegResponseChanged(short RegResponse);

    void EnableLogInWindow();

public slots:

    void Appear(QString , QString ); /* shows window and fills server credentials */

    /* setRegResponse() overloaded :
               for Q_PROPERTY should have only what is set,
               for slot connected to signal from receiving thread - should also have the message */
    void setRegResponse(short );
    void setRegResponse(short , QString ); /* first argument values -
                                                1 - success, 2 - failure, 3 - undetermined (time-out)
                                              second argument value - actual response message from server */

private slots:

    void on_RegButton_clicked();

    void on_CancelButton_clicked();

private:

    Ui::RegNewWindow *ui;

    QString response_message; /* to print out success/failures of registration */

    short m_RegResponse;

    void SendRegNew(QString , QString ); /* sends suggested username and password to server */

    void closeEvent(QCloseEvent * );
};

#endif // REGNEWWINDOW_H
