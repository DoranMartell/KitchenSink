#include "endofgamewindow.h"
#include "ui_endofgamewindow.h"

EndOfGameWindow::EndOfGameWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EndOfGameWindow)
{
    ui->setupUi(this);

    /* hide cross button, only Minimize button present */
    setWindowFlags(Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint);
}
/*----------------------------------------------------------------------------------*/
void EndOfGameWindow::showEOGWindowSlot(QString Name, QString Which)
{
    qDebug() << "Going to show" << Which << "Window...";

    if (Which == "Victory")
    {
        ui->TextLabel->setText(QString("Congratulations, " + Name + ",\nyou are victorious! " +
                "Would you like\nto continue your adventures, reassemble your fleet\nand fight some new enemies?"));
    }
    else if (Which == "Defeat")
    {
        ui->TextLabel->setText(QString("You fought bravely, " + Name + ",\n" +
                "unfortunatelly, luck was not on your side this time.\nWould you like to take a rematch?"));
    }
    this->setWindowTitle(Which);
    emit DisableClientWindowSig(false);
    show();
}

/*----------------------------------------------------------------------------------*/
void EndOfGameWindow::on_RepeatButton_clicked()
{
    qDebug() << "Player chose to play again under previous alias, fetch FillTheGrid Window";

    SetStartGame(false);

    emit HideClientWindowSig();
    emit ShowFillTheGridWindowSig();

    hide();
}
/*----------------------------------------------------------------------------------*/
void EndOfGameWindow::on_EnoughButton_clicked()
{
    qDebug() << "Player chose to leave, so we should gracefully join all threads and close all connections";

    emit SendBbyeSig();

    close();
}
/*----------------------------------------------------------------------------------*/
void EndOfGameWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << "EndOfGameWindow is about to be closed...";
    emit CloseAllOthersSig();
    event->QCloseEvent::accept();
}
/*----------------------------------------------------------------------------------*/
EndOfGameWindow::~EndOfGameWindow()
{
    delete ui;
}

