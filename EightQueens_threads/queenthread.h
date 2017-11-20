#ifndef QUEENTHREAD_H
#define QUEENTHREAD_H

#include <QThread>
#include <QDebug>

#define SIZE 8

class QueenThread : public QThread
{
    Q_OBJECT
    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)
    Q_PROPERTY(bool finished READ finished WRITE setFinished NOTIFY finishedChanged)

    bool m_running;
    bool m_finished;

public:

    QueenThread();
    bool running() const;

    void run(); /* the function which should be rewritten for
                    our child of QThread class. All placeQueen() should be here */

    int board[SIZE][SIZE];

    int queensOnBoard;

    double sleeptime;
    void setSleeptime(double );

    void printBoardNStatus(double , QString , int );

    void InitializeBoardArray();

    /* Main recursive function which picks a square to put a new queen */
    /* All checks if the square is not taken/beaten by other queens are called here */
    int placeQueen();

    /* specific check which hints when it is time
       to place previous queen somewhere else */
    int TriedAll(int a[SIZE][SIZE]);

    /* third argument determines if accessability should be
       increased or decreased */
    void changeAccessability(int , int , int );

    bool finished() const;

public slots:

    void setRunning(bool running);

    void setFinished(bool finished);

signals:

    void runningChanged(bool running);
    void finishedChanged(bool finished);
    void print(QString , int );
//    void printFinal(QString , int );
    void refreshOnStopOff();

};

#endif // QUEENTHREAD_H
