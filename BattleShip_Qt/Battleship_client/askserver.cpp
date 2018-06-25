#include "askserver.h"
#include "ui_askserver.h"
/*----------------------------------------------------------------------------------*/
askserver::askserver(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::askserver)
{
    ui->setupUi(this);
    setWindowTitle("Server Credentials Request");
}

/*----------------------------------------------------------------------------------*/
void askserver::showAskServerSlot()
{
    show();
}
/*----------------------------------------------------------------------------------*/
void askserver::on_ConnectButton_clicked()
{
    QString ServerIP = ui->IP_1->text() + "." + ui->IP_2->text() + "." + ui->IP_3->text() + "." + ui->IP_4->text();
    int ServerPort = ui->portBox->text().toInt();

    qDebug() << "Server IP:" << ServerIP;
    qDebug() << "Server Port:" << ServerPort;

    emit ServCredentialsCollectedSig(ServerIP, ServerPort);

    if (shouldHideAskServWindow)
    {
        hide();
        emit showAskNameWindowSig();
    }
}
/*----------------------------------------------------------------------------------*/
void askserver::on_CancelButton_clicked()
{
    close();
}
/*----------------------------------------------------------------------------------*/
void askserver::closeEvent(QCloseEvent * event)
{
    event->QCloseEvent::accept();
}
/*----------------------------------------------------------------------------------*/
askserver::~askserver()
{
    delete ui;
}

