#include "queens.h"
#include "ui_queens.h"

#include "queenthread.h"

/*------------------------------------------------------------------------------------------------*/
Queens::Queens(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Queens)
{
    ui->setupUi(this);
    ui->centralWidget->setFixedSize(395, 265);

    /* Initially there is nothing to stop */
    ui->StopButton->setEnabled(false);

    speed = 100.0 /ui->dial->value();
    emit SpeedChange(speed);

    QObject::connect(ui->dial, &QAbstractSlider::valueChanged, this, &Queens::SliderValueChanged);

    thread.InitializeBoardArray();

    /* the main backtracking loop which puts a queen on board
       doesn't run initially, until we press "Start/Resume" */
    thread.setRunning(false);
    thread.setFinished(false);

}
/*------------------------------------------------------------------------------------------------*/
Queens::~Queens()
{
    delete ui;
}
/*------------------------------------------------------------------------------------------------*/

void Queens::on_StartButton_clicked()
{
    thread.setRunning(true);

    srand(time(NULL));
    rand();

    /* Can't refresh and start while processing */
    ui->RefreshButton->setEnabled(false);
    ui->StartButton->setEnabled(false);

    thread.start();

    /* Can stop while processing */
    ui->StopButton->setEnabled(true);

}
/*------------------------------------------------------------------------------------------------*/
void Queens::RefreshOnStopOff() /* after we received a signal from thread that backtracking loop stopped */
{
    /* Once it is done we are able to click Refresh again */
    ui->RefreshButton->setEnabled(true);
    /* ... and unable to click Stop as program stopped anyway */
    ui->StopButton->setEnabled(false);
}

/*------------------------------------------------------------------------------------------------*/

void Queens::on_StopButton_clicked()
{
    thread.setRunning(false);
    ui->RefreshButton->setEnabled(true);
    ui->StartButton->setEnabled(true);
    ui->StopButton->setEnabled(false);
}
/*------------------------------------------------------------------------------------------------*/

void Queens::on_RefreshButton_clicked()
{
    thread.InitializeBoardArray();
//    thread.start();

    printBoard("Chessboard refreshed", 3000);

    ui->StopButton->setEnabled(false);
    ui->StartButton->setEnabled(true);

}

/*------------------------------------------------------------------------------------------------*/
void Queens::on_CloseButton_clicked()
{
    thread.setFinished(true);
    QApplication::exit();
}

/*------------------------------------------------------------------------------------------------*/
void Queens::SliderValueChanged(int val)
{
    speed = 100.0/val;
    emit SpeedChange(speed);
}
