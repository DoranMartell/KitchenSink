#include "regnewwindow.h"
#include "ui_regnewwindow.h"

RegNewWindow::RegNewWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegNewWindow)
{
    ui->setupUi(this);

}
/*--------------------------------------------------------------------------------------------------------------*/
RegNewWindow::~RegNewWindow()
{
    delete ui;
}
/*--------------------------------------------------------------------------------------------------------------*/
void RegNewWindow::Appear(QString ip, QString port)
{
    servIp = ip;
    servPortNo = port;

    /* make Register Window atop LogIn Window */
    //Qt::WindowFlags flags = windowFlags();
    //setWindowFlags(flags | Qt::WindowStaysOnTopHint);

    this->show();

    qDebug() << "Where are we going to send registration data? " << servIp << ":" << servPortNo;
}
/*--------------------------------------------------------------------------------------------------------------*/
void RegNewWindow::on_RegButton_clicked()
{
    DesirblUname = ui->UnameEdit->text();
    DesirblPass = ui->PassEdit->text();
    ConfPasswrd = ui->ConfirmPassEdit->text();

    if (!FieldValueAllowed(DesirblUname, "UserName") ||
        !FieldValueAllowed(DesirblPass, "Password") ||
        !FieldValueAllowed(ConfPasswrd, "Confirm Password") )
    {
        ui->UnameEdit->setText("");
        ui->PassEdit->setText("");
        ui->ConfirmPassEdit->setText("");

        return;
    }

    qDebug() << "Check if user input passwords match each other";
    if (ConfPasswrd != DesirblPass)
    {
        qDebug() << DesirblPass << "doesn't match" << ConfPasswrd;

        QMessageBox::warning(this, "Registration failed", "Password doesn't match its confirmation!");

        ui->PassEdit->setText("");
        ui->ConfirmPassEdit->setText("");

        return;
    }

    qDebug() << "Send reg-new message to server";
    SendRegNew(DesirblUname, DesirblPass);

}
/*--------------------------------------------------------------------------------------------------------------*/
bool RegNewWindow::FieldValueAllowed(QString field_value, QString field_name)
{
    qDebug() << "Checking if field" << field_name << "has a valid value" << field_value;

    QString wndwname = "Registration failed";
    QString mess;

    if (field_value.isEmpty())
    {
        mess = "You forgot to fill " + field_name + " field";

    }
    else if (field_value.length() > 16)
    {
        mess = field_name + " is longer than 16 characters";
    }
    else
    {
        qDebug() << "Field value was verified and could be used";
        return true;
    }

    qDebug() << field_name << "is longer than 16 characters or empty!";
    QMessageBox::warning(this, wndwname, mess);
    return false;

}
/*--------------------------------------------------------------------------------------------------------------*/
void RegNewWindow::SendRegNew(QString uname, QString passwrd)
{
    bool success = false;
    emit OpenSocketRegWind(servIp, servPortNo, success);

    if (!success)
    {
        qDebug() << "Socket was not opened - no need to send SignOn message to nowhere";
        return;
    }

    emit SendRegMessage("reg-new", uname, passwrd);
    ui->RegButton->setEnabled(false);

    /* response_message here is a member of class RegNewWindow */
    response_message = "No response from the server!";
    setRegResponse(3, response_message); /* 3 - undetermined */

    /* waits for response */
    float sleptTime = 0;
    while ( sleptTime < 30.00 && RegResponse() == 3)
    {
        usleep(1000); /* microseconds */
        QCoreApplication::processEvents();
        sleptTime += 0.001;

        if (((int)(sleptTime * 1000) - int(sleptTime) * 1000) == 0)
            qDebug() << sleptTime;

        /*  */
        if ( RegResponse() == 1)
        {
            QMessageBox::information(this, "Registration Completed", response_message);
            qDebug() << "reg-new positive response received" << uname;
            break;
        }
        else if ( RegResponse() == 2)
        {
            QMessageBox::critical(this, "Registration Error", response_message);
            qDebug() << "reg-new negative response received" << uname;
            break;
        }
    }

    /* after 30 seconds of silence try sign-on with different login and password */
    if ( RegResponse() == 3 )
    {
        qDebug() << "reg-new " << uname << "message TIME-OUT!!";
        QMessageBox::critical(this, "Registraction Error", response_message);
    }

    hide();
    emit EnableLogInWindow();
    ui->RegButton->setEnabled(true);
    return;

}
/*--------------------------------------------------------------------------------------------------------------*/
short RegNewWindow::RegResponse() const
{
    return m_RegResponse;
}
/*--------------------------------------------------------------------------------------------------------------*/
void RegNewWindow::setRegResponse(short RegResponse, QString RespMess)
{
    response_message = RespMess; /* what actually came from receiving thread */

    setRegResponse(RegResponse);

}
/*--------------------------------------------------------------------------------------------------------------*/
void RegNewWindow::setRegResponse(short RegResponse)
{
    if (m_RegResponse == RegResponse)
        return;

    m_RegResponse = RegResponse;
    emit RegResponseChanged(RegResponse);
}
/*--------------------------------------------------------------------------------------------------------------*/
void RegNewWindow::on_CancelButton_clicked()
{
    this->close();
}
/*--------------------------------------------------------------------------------------------------------------*/
void RegNewWindow::closeEvent(QCloseEvent * event)
{
    emit EnableLogInWindow();
    qDebug() << "Closing Registration Window";
    event->QCloseEvent::accept();
}
