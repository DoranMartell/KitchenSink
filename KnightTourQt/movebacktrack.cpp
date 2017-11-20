#include "knighttour.h"
#include "ui_knighttour.h"
#include <math.h>
#include <QThread>

/*-------------------------------------------*/
int KnightTour::moveKnightBacktrack(int * step)
{
    int i;
    int tries;
    tries = 0x00000000;
    int res = 0;
    int a;

    QString MessToBar;

    while (  (tries ^ 0x11111111))
    {
        if (RefreshButtonClicked)
        {
            ui->statusBar->showMessage("\"Refresh\" button clicked. Backtracking was interrupted!", 0);
            return -1;
        }

        i = rand() % 8;

        /* we shouldn't try the same value of i numerous times */
        a = pow(16, i);

        if ((tries & a) == 0)
        {
            if (!OutOfBoard(x+x_temp[i]) && !OutOfBoard(y+y_temp[i]))
            {
                if (board[y + y_temp[i]][x + x_temp[i]] == 0) /* Knight actually can go there */
                {
                    x = x + x_temp[i];
                    y = y + y_temp[i];
                    board[y][x] = 1; /* knight is here */
                    (*step)++;
                    QThread::sleep(0.05);
                    QCoreApplication::processEvents(); /* so the app will not hang */
                    printBoard(*step);
                    MessToBar = "Knight made " + QString::number(*step) + " moves so far";
                    ui->statusBar->showMessage(MessToBar, 5000);

                    if (!moveKnightBacktrack(step))
                    {
//                        printf("The previous move lead us to a dead-end\n");
                        (*step)--;
                        // a = 1;
                        // a = pow(16, i-1);
                        tries |= a;
                        /* cordinates must be restored back */
                        board[y][x] = 0;
                        x -= x_temp[i];
                        y -= y_temp[i];
                        QThread::sleep(0.05);
                        QCoreApplication::processEvents();
                        printBoard(*step);
                        MessToBar = "Knight made " + QString::number(*step) + " moves so far";
                        ui->statusBar->showMessage(MessToBar, 5000);

                    }
                    else
                    {
                        // DecreaseAccessabilitySurrounding();
                        return 1; /* or should we just do break */
                    }
                }
                else
                {
                    // a = 1;
                    // a = pow(16, i-1);
                    tries |= a;
                }
            }
            else
            {
                // a = 1;
                // a = pow(16, i-1);
                tries |= a;
            }
        }
    }

    /* if the knight tried every possible move, then it's time to rethink the previous one */
    /* unless he crossed all the board already */

    if ((*step) == 63)
    {
//        printf("The knight finished his tour!\n");
        ui->statusBar->showMessage("The knight finished his tour!", 0);
        canMakeFurtherMove = 0;
        return 1;
    }
    else
    {
//        printf("The previous move lead us to a dead-end!!!\n");
        ui->statusBar->showMessage("The previous move lead us to a dead-end!!!", 0);
        // step--;
        canMakeFurtherMove = 0;
        return 0;
    }

}
