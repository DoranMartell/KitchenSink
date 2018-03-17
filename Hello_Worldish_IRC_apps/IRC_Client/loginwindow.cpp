#include "loginwindow.h"
#include "ui_loginwindow.h"

#include <QDebug>

LogInWindow::LogInWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogInWindow)
{
    ui->setupUi(this);

    setSignOnConfirmed(false);
    ui->LogInButton->setEnabled(true);

}
/*--------------------------------------------------------------------------------------------------------------*/
LogInWindow::~LogInWindow()
{
    delete ui;
}
/*--------------------------------------------------------------------------------------------------------------*/
void LogInWindow::ShowWindow(QString s, QString p)
{
    servIp = s;
    servPortNo = p;

    QString title("Log In to server " + servIp + ":" + servPortNo);
    //qDebug() << "Supposed window title" << title;
    this->setWindowTitle(title);

    this->show();
}
/*--------------------------------------------------------------------------------------------------------------*/
void LogInWindow::on_CancelButton_clicked()
{
    hide();
    emit StepBack();
    //QApplication::exit();
}
/*--------------------------------------------------------------------------------------------------------------*/
void LogInWindow::on_LogInButton_clicked()
{
    QString login(ui->LoginLine->text());
    QString passwrd(ui->PasswrdLine->text());

    qDebug() << "User entered:";
    qDebug() << "Login:" << login;
    qDebug() << "Password:" << passwrd;

    if (login.isEmpty())
    {
        QMessageBox::critical(this, "LogIn Error", "You forgot to fill your username!");
        return;
    }

    if (passwrd.isEmpty())
    {
        QMessageBox::critical(this, "LogIn Error", "You forgot to fill your password!");
        return;
    }

    //emit CheckLogNPass(login, passwrd);
    SendSignOn(login, passwrd);
}
/*--------------------------------------------------------------------------------------------------------------*/
void LogInWindow::SendSignOn(QString l, QString p)
{
    bool success = true;
    emit OpenSocketClient(servIp, servPortNo, success); /* always after we press LogIn */

    if (!success)
    {
        qDebug() << "Socket was not opened - no need to send SignOn message to nowhere";
        return;
    }

    setSignOnConfirmed(3); /* 3 - for undetermined */

    emit SendMessageClient("sign-on", l, p);
    ui->LogInButton->setEnabled(false);

    /* waits for response */
    float sleptTime = 0;
    while ( sleptTime < 30.00 && SignOnConfirmed() == 3)
    {
        usleep(1000); /* microseconds */
        QCoreApplication::processEvents();
        sleptTime += 0.001;

        if (((int)(sleptTime * 1000) - int(sleptTime) * 1000) == 0)
            qDebug() << sleptTime;

        /* if Sign-On was approved by server, then open Main Client's Window */
        if ( SignOnConfirmed() == 1)
        {
            qDebug() << "sign-on confirmed for " << l;
            emit ShowClientWindow(l); /* passes UserName to MainWindow to include it in title */
            hide();
            ui->LogInButton->setEnabled(true);
            //emit CloseSocket();
            return;
        }
        else if ( SignOnConfirmed() == 2)
        {
            qDebug() << "sign-on  declined for " << l;
            //hide();
            ui->LogInButton->setEnabled(true);
            //emit CloseSocket();
            return;
        }
    }

    /* after 30 seconds of silence try sign-on with different login and password */
    if ( SignOnConfirmed() == 3 )
    {
        qDebug() << "sign-on declined for " << l << " by TIME-OUT!!";
        //hide();
        ui->LogInButton->setEnabled(true);
        return;
    }

}
/*--------------------------------------------------------------------------------------------------------------*/
void LogInWindow::ErrorLogIn(QString errmsg)
{
    QMessageBox::critical(this, "Error", errmsg);
    ui->LoginLine->setText("");
    ui->PasswrdLine->setText("");

}

/*--------------------------------------------------------------------------------------------------------------*/
int LogInWindow::SignOnConfirmed() const
{
    return m_SignOnConfirmed;
}
/*--------------------------------------------------------------------------------------------------------------*/
void LogInWindow::setSignOnConfirmed(int SignOnConfirmed)
{
    if (m_SignOnConfirmed == SignOnConfirmed)
        return;

    m_SignOnConfirmed = SignOnConfirmed;
    emit SignOnConfirmedChanged(SignOnConfirmed);
}
/*--------------------------------------------------------------------------------------------------------------*/
void LogInWindow::on_RegisterButton_clicked()
{
    this->setEnabled(false);
    emit RegWndShow(servIp, servPortNo);

}
/*--------------------------------------------------------------------------------------------------------------*/
void LogInWindow::Enable()
{
    this->setEnabled(true);
}
