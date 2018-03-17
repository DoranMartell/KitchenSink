#include "servcredentials.h"
#include "ui_servcredentials.h"

#include <QDebug>

ServCredentials::ServCredentials(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServCredentials)
{
    ui->setupUi(this);
}
/*--------------------------------------------------------------------------------------------------------------*/
ServCredentials::~ServCredentials()
{
    delete ui;
}
/*--------------------------------------------------------------------------------------------------------------*/
void ServCredentials::ShowServCrWindow() /* called from Client window if we dissconnect (intentionally or not)*/
{
    qDebug() << "Show Server Credentials Input window";

    ui->firstPartIp->setText("127");
    ui->scndPartIp->setText("0");
    ui->thrdPartIp->setText("0");
    ui->fourthPartIp->setText("1");
    ui->PortLine->setValue(2323);

    this->show();
}

/*--------------------------------------------------------------------------------------------------------------*/
void ServCredentials::on_OkButton_clicked()
{
    QString Part1 = ui->firstPartIp->text();
    QString Part2 = ui->scndPartIp->text();
    QString Part3 = ui->thrdPartIp->text();
    QString Part4 = ui->fourthPartIp->text();

    bool isNum1, isNum2, isNum3, isNum4;

    int value1 = Part1.toInt(&isNum1);
    int value2 = Part2.toInt(&isNum2);
    int value3 = Part3.toInt(&isNum3);
    int value4 = Part4.toInt(&isNum4);

    if (!isNum1 || !isNum2 || !isNum3 || !isNum4 )
    {
        SetDefaultIp("IP address can't contain anything but digits!");
        return;
    }

    if (value1 == 0)
    {
        SetDefaultIp("IP address can't start with zero!");
        return;
    }

    if (value1 < 0 || value2 < 0 || value3 < 0 || value4 < 0)
    {
        SetDefaultIp("IP address shouldn't have negative numbers as its part!");
        return;
    }

    if (value1 > 255 || value2 > 255 || value3 > 255 || value4 > 255)
    {
        SetDefaultIp("Incorrect IP address!");
        return;
    }

    QString IpAddr = QString::number(value1) + "." + QString::number(value2) +
            "." + QString::number(value3) + "." + QString::number(value4);

    //qDebug() << "User input IP of server: " << IpAddr;

    //QString Port = ui->PortLine->text(); /* used to be so when we had LineEdit instead of SpinBox */
    QString Port = QString::number(ui->PortLine->value());

    bool isNumPort;

    int valuePort = Port.toInt(&isNumPort);

    //qDebug() << "User input port number of server: " << Port;

    if (!isNumPort || valuePort <= 0 || valuePort >= 65535)
    {
        SetDefaultPort("Incorrect port number!");
        return;
    }

    hide();
    //emit OpenSocketClient(IpAddr, Port); /* here is where receiving thread is started */
    emit ShowLoginWindow(IpAddr, Port);

}
/*--------------------------------------------------------------------------------------------------------------*/
void ServCredentials::SetDefaultIp(QString errmsg)
{
    QMessageBox::critical(this, "Error", errmsg);
    ui->firstPartIp->setText("127");
    ui->scndPartIp->setText("0");
    ui->thrdPartIp->setText("0");
    ui->fourthPartIp->setText("1");
    ui->PortLine->setValue(2323); /* Just in case, port checks go after all for IP */
}
/*--------------------------------------------------------------------------------------------------------------*/
void ServCredentials::SetDefaultPort(QString errmsg)
{
    QMessageBox::critical(this, "Error", errmsg);
    ui->PortLine->setValue(2323);
}
/*--------------------------------------------------------------------------------------------------------------*/
void ServCredentials::on_CancelButton_clicked()
{
    /* gently kill receiving thread */
    emit KillRecvThread(true);
    QApplication::exit();
}
