#ifndef QUEENS_H
#define QUEENS_H

#include <QMainWindow>
#include <QObject>
#include <QAbstractSlider> /* for valueChanged() signal */
#include "queenthread.h"

namespace Ui {
class Queens;
}

class Queens : public QMainWindow
{
    Q_OBJECT

public:
    explicit Queens(QWidget *parent = 0);
    ~Queens();

    QueenThread thread;


    /* thread tells where to enable/disable buttons */
    void RefreshOnStopOff();

    /* Slot handling change of speed value on Dial */
    double speed;
    void SliderValueChanged(int );

    /* now prints both Board and Status */
    void printBoard(QString , int );

    /* depending on third argument it is decided
     * if the square should grey-out or marked as
     * having a queen on it */
    void printQueen(int, int, int);

    /* for each y */
    void printQueenZero(int , QString );
    void printQueenOne(int , QString );
    void printQueenTwo(int , QString );
    void printQueenThree(int , QString );
    void printQueenFour(int , QString );
    void printQueenFive(int , QString );
    void printQueenSix(int , QString );
    void printQueenSeven(int , QString );

private slots:
    void on_StartButton_clicked();

    void on_StopButton_clicked();

    void on_RefreshButton_clicked();

    void on_CloseButton_clicked();

signals:
    /* Increasing of value on slider reduces sleeptime to print board */
    void SpeedChange(double );

private:
    Ui::Queens *ui;


};

#endif // QUEENS_H
