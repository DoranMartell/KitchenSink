#include "queenthread.h"

/*------------------------------------------------------------------------------------------------*/
QueenThread::QueenThread()
{
    setSleeptime(1);
}
/*------------------------------------------------------------------------------------------------*/
bool QueenThread::running() const
{
    return m_running;
}
/*------------------------------------------------------------------------------------------------*/
bool QueenThread::finished() const
{
    return m_finished;
}
/*------------------------------------------------------------------------------------------------*/
void QueenThread::setRunning(bool running)
{
    if (m_running == running)
        return;

    m_running = running;
    emit runningChanged(running);
}
/*------------------------------------------------------------------------------------------------*/
void QueenThread::setFinished(bool finished)
{
    if (m_finished == finished)
        return;

    m_finished = finished;

    emit finishedChanged(finished);
}
/*------------------------------------------------------------------------------------------------*/
void QueenThread::InitializeBoardArray() /* called in constructor and each time Refresh button clicked */
{
    int i, j;

    queensOnBoard = 0;

    setFinished(false);

    for ( i = 0; i < SIZE; i++)
    {
        for (j = 0; j < SIZE; j++)
            board[i][j] = 0;
    }
}
/*------------------------------------------------------------------------------------------------*/
int QueenThread::TriedAll(int a[SIZE][SIZE])
{
    int i, j;

    for (i = 0; i < SIZE; i++)
    {
        for (j = 0; j < SIZE; ++j)
        {
            if (a[i][j] == 0)
            {
                return 0; /* found at least one square where a queen can be placed */
            }
        }
    }

    return 1; /* tried all squares - nowhere to place a queen */

}

/*------------------------------------------------------------------------------------------------*/

/* if how is 1 - then the squares should be unaccessable
    if how is -1 - some squares may become accessible once previous step was reconsidered */
void QueenThread::changeAccessability(int x, int y, int how)
{
    int k;

    /* Horizontal and vertiacal accessability changing */
    for (k = 0 ; k < SIZE; k++)
    {
        board[k][x] += how; /* those squares are beaten by queen */
        board[y][k] += how; /* thus they won't be available for other queens */
    }

    /* Diagonals accessability changing */
    for ( k = 1 ; (x - k) >= 0 && (y - k) >= 0; k++)
    {
        board[y - k][x - k] += how;
    }

    for ( k = 1 ; (x + k) < SIZE && (y - k) >= 0; k++)
    {
        board[y - k][x + k] += how;
    }

    for ( k = 1 ; (x - k) >= 0 && (y + k) < SIZE; k++)
    {
        board[y + k][x - k] += how;
    }

    for ( k = 1 ; (x + k) < SIZE && (y + k) < SIZE; k++)
    {
        board[y + k][x + k] += how;
    }

    /* Accessability of square with a queen, or if how is not +1 then restore initial */
    if (how > 0)
        board[y][x] = 900;
    else
        board[y][x] = 0;

}

/*------------------------------------------------------------------------------------------------*/
int QueenThread::placeQueen()
{

    int i, j;
    int x, y;
    int res;
    int tried[SIZE][SIZE] = {0}; /* 1 - if we already tried
                                    to put a queen on square with coordinates
                                    - subscript of this array */
    QString MessToBar;

    while (1)
    {
        try_again:
        if (running() && !finished())
        {
            /*if (BreakBacktracking == 2)
                return -1;*/

            /* Mark all unavailable squares as "tried" as well */
            for (i = 0; i < SIZE; i++)
            {
                for (j = 0; j < SIZE; ++j)
                {
                    if (board[i][j] > 0)
                        tried[i][j] = 1;
                }
            }

            /* Loop with choosing of positions for the queen */
            if (queensOnBoard == 8)
            {
                qDebug() << "All 8 queens were placed";

                printBoardNStatus(sleeptime, "All 8 queens were placed!", 0);
                setFinished(true);
                setRunning(false);
                return 2;
            }
            else if (TriedAll(tried))
            {
                qDebug() << "Tried all squares, nowhere to put a queen";
                //ui->statusBar->showMessage("Nowhere to put a queen! Gotta rewind a little back", 2000);
                printBoardNStatus(sleeptime, "Nowhere to put a queen! Gotta rewind a little back", 1000);
                return 0; /* only way to replace queens on previous steps */
            }
            else
            {
                /* randomly choose coordinates to place a queen */
                do
                {
                    x = rand() % 8;
                    y = rand() % 8;
                } while (tried[y][x] == 1);

                tried[y][x] = 1;
            }

            /* Change accessability regarding chosen position of queen */
            changeAccessability(x, y, 1);

            /* Increase number of queens currently on the board */
            queensOnBoard++;

            //ui->statusBar->showMessage(MessToBar, 5000);
            //printBoard(queens);

            MessToBar = QString::number(queensOnBoard) + " queen was placed successfully";
            printBoardNStatus(sleeptime, MessToBar, 1000);

            qDebug() << queensOnBoard << " queen was placed";

            if ((res = placeQueen()) == 0)
            {
                /* we were unable to put another queen,
                    let's rethink this step */
                (queensOnBoard)--;
                /* restore values of accessability to ones of prior to this step */
                changeAccessability(x, y, -1);

                //QCoreApplication::processEvents();
                //printBoard(queens);

                printBoardNStatus(sleeptime, "Step Back", 1000);

                /* try to choose a place for queen again */
                goto try_again;
            }
            else if (res == 2)
            {
                qDebug() << "All 8 queens were placed";
                //ui->statusBar->showMessage("All 8 queens were placed!", 0);
//                printBoardNStatus(sleeptime, "All 8 queens were placed!", 0);
//                thread()->quit();
                return 2;
            }
            else if (res == -1)
            {
                qDebug() << "Interrupted";
                //ui->statusBar->showMessage("Interrupted!", 0);
                emit print("Interrupted!", 0);
                return -1;
            }

            /* Queen was placed successfully */
        //    ui->statusBar->showMessage("Queen was placed successfully", 5000);
            return 1;
        }
        else
        {
            printBoardNStatus(sleeptime, "Stopped",0);
            qDebug() << "waiting...";
        }
    }

}

/*------------------------------------------------------------------------------------------------*/
void QueenThread::run()
{
    if (!finished() && running())
    {
        placeQueen();
    }

    if (finished())
    {
        /* Once it is done we are able to click Refresh again */
         /* ... and unable to click Stop as program stopped anyway */
        emit refreshOnStopOff();
        return;
//        printBoardNStatus(sleeptime, "All 8 queens were placed!", sleeptime*1000);
    }

}

/*------------------------------------------------------------------------------------------------*/
void QueenThread::printBoardNStatus(double sleeptime, QString message, int timeout)
{
    //printBoard(queens);
    emit print(message, timeout);
    QThread::sleep(sleeptime);
}
/*------------------------------------------------------------------------------------------------*/
void QueenThread::setSleeptime(double seconds)
{
    sleeptime = seconds;
}

