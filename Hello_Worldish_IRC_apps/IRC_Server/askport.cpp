#include "askport.h"
#include "ui_askport.h"

AskPort::AskPort(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AskPort)
{
    ui->setupUi(this);
    ui->lineEdit->setValue(2323);
}
/*--------------------------------------------------------------------------------------------------------------*/
AskPort::~AskPort()
{
    delete ui;
}

/*--------------------------------------------------------------------------------------------------------------*/
void AskPort::on_OkButton_clicked()
{
    //userPort = ui->lineEdit->value();
    //bool isNum;
    //int value = userPort.toInt(&isNum);

    userPort = QString::number(ui->lineEdit->value());
    int value = ui->lineEdit->value();

    qDebug() << "User input: " << userPort/*.toStdString().c_str()*/;

    if (/*!isNum ||*/ value <= 0 || value >= 65535)
    {
        QMessageBox::critical(this, "Error", "Please enter correct port value!");
        ui->lineEdit->setValue(2323);
        return;
    }

//    FILE *in;
//    char buff[10];

//    /* perform netstat command */
//    /* Check which functions of Unix API netstat uses */
//    /* And remove it later */
//    if (!(in = popen("netstat -antu | awk '{print $4}' | awk -F \":\" '{print $2}' | awk '!/^$/'", "r")))
//    {
//        QMessageBox::critical(this, "Error", "Error reading busy ports!");
//        ui->lineEdit->setText("2323");
//        return;
//    }

//    qDebug() << "netstat results:";
//    while (fgets(buff, sizeof(buff), in) != NULL)
//    {
//        buff[strlen(buff)-1] = '\0'; /* last character was \n - removing it */
//        qDebug() << buff;

//        if (!strcmp(buff, userPort.toStdString().c_str()))
//        {
//            QMessageBox::critical(this, "Error", "This port is already in use!");
//            ui->lineEdit->setText("2323");
//            pclose(in);
//            return;
//        }
//    }

//    pclose(in);

//    /* Reading port numbers from /etc/services*/
//    if ( ! (in = popen(" cat /etc/services | awk '{print $2}' | awk -F \"/\" '{print $1}'", "r")))
//    {
//        QMessageBox::critical(this, "Error", "Error opening /etc/services!");
//        ui->lineEdit->setText("2323");
//        pclose(in);
//        return;
//    }

//    qDebug() << "port numbers from /etc/services:";
//    while (fgets(buff, sizeof(buff), in) != NULL)
//    {
//        if (buff[0] >= '0' && buff[0] <= '9')
//        {
//            buff[strlen(buff)-1] = '\0';
//            qDebug() << buff;

//            if (!strcmp(buff, userPort.toStdString().c_str()))
//            {
//                QMessageBox::critical(this, "Error", "This port is meant for some other stuff!");
//                ui->lineEdit->setText("2323");
//                pclose(in);
//                return;
//            }
//        }
//    }

//    pclose(in);

    hide();

    emit SetServerPort(userPort.toInt());
    emit ShowMainWindow();

}

/*--------------------------------------------------------------------------------------------------------------*/
void AskPort::ShowAskPortAgain(QString err_string)
{
    QMessageBox::critical(this, "Error binding", err_string);
    ui->lineEdit->setValue(2323);
    show();
}

/*--------------------------------------------------------------------------------------------------------------*/
void AskPort::on_CancelButton_clicked()
{
    QApplication::exit();
}

