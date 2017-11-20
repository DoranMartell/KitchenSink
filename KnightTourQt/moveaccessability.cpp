#include "knighttour.h"
#include "ui_knighttour.h"

int KnightTour::moveKnightAccessability(int * step)
{
    int i;
    int MinAccess[8] = {999};
    int MinSubscript[8];
    int MinCount = 0;

    QDebug dbg(QtDebugMsg);
    dbg << "";

    for ( i = 0; i < 8; i++ )
    {

        /* First Check if this place is not out of board*/
        if (!OutOfBoard(y+y_temp[i]) && !OutOfBoard(x+x_temp[i]))
        {
            if ( accessability[y+y_temp[i]][x+x_temp[i]] < MinAccess[0] && accessability[y+y_temp[i]][x+x_temp[i]] > 0)
            {
                MinCount = 0;
                MinAccess[MinCount] = accessability[y+y_temp[i]][x+x_temp[i]];
                MinSubscript[MinCount] = i;
                MinCount++;
            }
            else if ( accessability[y+y_temp[i]][x+x_temp[i]] == MinAccess[0] && accessability[y+y_temp[i]][x+x_temp[i]] > 0)
            {
                MinAccess[MinCount] = accessability[y+y_temp[i]][x+x_temp[i]];
                MinSubscript[MinCount] = i;
                MinCount++;
            }
        }
    }

    if (MinAccess[0] == 999 )
    {
        if ((*step) != (SIZE * SIZE -1))
        {
            dbg << "I'm stuck!";
            canMakeFurtherMove = 0;
            ui->statusBar->showMessage("I'm stuck! Let's try again from the start", 0);
        }
        else
        {
            dbg << "All squares were visited!";
            canMakeFurtherMove = 0;
            ui->statusBar->showMessage("Knight visited all squares", 0);
        }

        return 0;
    }

    /* choose random subscript from array of minimal accessability */
    int Subs = rand() % MinCount;
    x = x + x_temp[MinSubscript[Subs]];
    y = y + y_temp[MinSubscript[Subs]];
    board[y][x] = 1; /* knight is here now */

    if ( (*step) != (SIZE * SIZE - 3 ))
                        /* Exceptional situation: accessability of last square
                            becomes zero becuase of this function despite that
                            the square itself can be reached */
        DecreaseAccessabilitySurrounding();
    else
        accessability[y][x] = 0; /* decrease accessability only of current square */

    (*step)++; /* move made - number of moves increased */

    return 1;
}

int KnightTour::OutOfBoard(int a)
{
    if ( a < 0 || a > SIZE - 1)
        return 1;
    else
        return 0;
}

void KnightTour::DecreaseAccessabilitySurrounding()
{
    int i;

    for ( i = 0; i < 8; i++)
    {
        if ( !OutOfBoard( y + y_temp[i] ))
        {
            if ( !OutOfBoard( x + x_temp[i] ))
            {
                if (accessability[y+ y_temp[i]][x+ x_temp[i]] != 0) /* if it is already zero let it be */
                {
                    accessability[y+ y_temp[i]][x+ x_temp[i]]--;
                }
            }
        }
    }

    accessability[y][x] = 0; /* a visited square should never be accessable again */

}

void KnightTour::refreshAccessability()
{
    int i, j;

    for (i = 0; i < SIZE; i++)
    {
        for (j = 0; j <= i; j++)
        {
            accessability[i][j] = 8;

            if ( OutOfBoard( i - 2 ))
                accessability[i][j] -= 2;
            else
            {
                if ( OutOfBoard( j - 1 ))
                    accessability[i][j]--;
                if ( OutOfBoard( j + 1 ))
                    accessability[i][j]--;
            }

            if ( OutOfBoard( i - 1 ))
                accessability[i][j] -= 2;
            else
            {
                if ( OutOfBoard( j - 2 ))
                    accessability[i][j]--;
                if ( OutOfBoard( j + 2 ))
                    accessability[i][j]--;
            }

            if ( OutOfBoard( i + 1 ))
                accessability[i][j] -= 2;
            else
            {
                if ( OutOfBoard( j - 2 ))
                    accessability[i][j]--;
                if ( OutOfBoard( j + 2 ))
                    accessability[i][j]--;
            }

            if ( OutOfBoard( i + 2 ))
                accessability[i][j] -= 2;
            else
            {
                if ( OutOfBoard( j - 1 ))
                    accessability[i][j]--;
                if ( OutOfBoard( j + 1 ))
                    accessability[i][j]--;
            }

            /* matrix is symmetrical */
            accessability[j][i] = accessability[i][j];
        }
    }

    /* all surrounding initial square */
    DecreaseAccessabilitySurrounding();

}
