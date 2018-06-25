#include "waitingwindow.h"
#include "ui_waitingwindow.h"

WaitingWindow::WaitingWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WaitingWindow)
{
    ui->setupUi(this);

    /* hide cross button, only Minimize button present */
    setWindowFlags(Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint);
}
/*----------------------------------------------------------------------------------*/
void WaitingWindow::ShowWaitingWindowSlot()
{
    qDebug() << "Going to show waiting window...";

    show();
    raise();

    int Count = 0;
    int Step = 1000;
    int Sec = 0;

    BreakTheLoop = false;

    while (!BreakTheLoop)
    {
        QApplication::processEvents();
        usleep(Step);
        QApplication::processEvents();

        Sec += Step;

        if (Sec == 1000000)
        {
            Count++;
            Sec = 0;
            ui->CountLabel->setText(QString::number(Count));
        }

        if (Count == 30)
        {
            Count = 0;
        }

        if (GetStartGame() == true)
        {
            HideWaitingWindowSlot();
            return;
        }
    }

    qDebug() << "Stepping out of ShowWaitingWindow loop";

}
/*----------------------------------------------------------------------------------*/
void WaitingWindow::HideWaitingWindowSlot()
{

    BreakTheLoop = true;

    qDebug() << "Going to hide waiting window...";

    hide();
}
/*----------------------------------------------------------------------------------*/
void WaitingWindow::closeEvent(QCloseEvent *event)
{
    event->QCloseEvent::accept();
}
/*----------------------------------------------------------------------------------*/
void WaitingWindow::CloseFromFillGridSlot()
{
    HideWaitingWindowSlot();
    close();
}
/*----------------------------------------------------------------------------------*/
WaitingWindow::~WaitingWindow()
{
    delete ui;
}


