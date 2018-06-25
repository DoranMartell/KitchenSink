#include "askport.h"
#include "ui_askport.h"
/*----------------------------------------------------------------------------------*/
AskPort::AskPort(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AskPort)
{
    ui->setupUi(this);
    setWindowTitle("Port Number Request");
}

/*----------------------------------------------------------------------------------*/
void AskPort::on_ChooseButton_clicked()
{

   emit setPortNumSig(ui->PortNumSpinBox->text());

   qDebug() << "ShouldHideAskWindow = " << shouldHideAskWindow;

   if (shouldHideAskWindow)
   {
       hide();
       emit startServAndShowSig();
   }
}
/*----------------------------------------------------------------------------------*/
void AskPort::on_CancelButton_clicked()
{
    close();
}
/*----------------------------------------------------------------------------------*/
void AskPort::closeEvent(QCloseEvent * event)
{
    // it is possible we got to that window while some sockets are opened and connections are still up
    // need to free all resourses


    event->QCloseEvent::accept();
}

/*----------------------------------------------------------------------------------*/
AskPort::~AskPort()
{
    delete ui;
}
