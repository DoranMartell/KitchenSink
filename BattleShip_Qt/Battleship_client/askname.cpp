#include "askname.h"
#include "ui_askname.h"
/*----------------------------------------------------------------------------------*/
askname::askname(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::askname)
{
    ui->setupUi(this);
    setWindowTitle("Player's Name Request");
}
/*----------------------------------------------------------------------------------*/
void askname::showAskNameWindowSlot()
{
    qDebug() << "If we are here, user confirmed server details and connected";

    show();
}
/*----------------------------------------------------------------------------------*/
void askname::on_RandButton_clicked()
{
    /* Randomly pick up a name from the list */
    QString RandomName = RandomlyChooseName();

    ui->NickNameEdit->setText(RandomName);
}
/*----------------------------------------------------------------------------------*/
QString askname::RandomlyChooseName()
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    short random_val = qrand() % RandomNames.size();

    return RandomNames[random_val];

}
/*----------------------------------------------------------------------------------*/
void askname::on_ConfirmButton_clicked()
{
    qDebug() << "Player pressed Confirm on Window with NickName request. Send this name to the server";
    SetNoNameReply(true);
    setNameOk(false);

    if (ui->NickNameEdit->text().indexOf('|') != -1)
    {
        QMessageBox::critical(this, "Error", "Please, use only latin characters and digits in your nickname!");
        return;
    }
    else if (ui->NickNameEdit->text() == "error" || ui->NickNameEdit->text() == "<defunct>" ||
             ui->NickNameEdit->text() == "Player1" || ui->NickNameEdit->text() == "Player2" ||
             ui->NickNameEdit->text() == "ok")
    {
        QMessageBox::critical(this, "Error", "Please, try another nickname");
        return;
    }

    emit SendNameSig(ui->NickNameEdit->text());

    float count = 0;
    while(NoNameReplyYet() == true)
    {
        usleep(5000);
        count += 0.01;

        /* What if connection to server is lost?? */
        if (/*count >= 2 || */GetServerUp() == false)
        {
            QMessageBox::critical(this, "Error", "Disconnected From Server!");
            close();
            return;
        }
    }

    if (getNameOk() == true)
    {
        hide(); /* next we are going to place our ships on the grid */

        qDebug() << "Emitted signal to show grid window";
        emit ShowGridSig();
    }
    else
    {
        QMessageBox::critical(this, "Error", "This nickname is already taken by another player!");
        return;
    }
}
/*----------------------------------------------------------------------------------*/
void askname::on_CancelButton_clicked()
{
    emit CloseConnAndJoinThreadSig();
    emit showAskServerSig();

    hide();
}
/*----------------------------------------------------------------------------------*/
void askname::closeEvent(QCloseEvent * event)
{
    event->QCloseEvent::accept();
}
/*----------------------------------------------------------------------------------*/
askname::~askname()
{
    delete ui;
}
