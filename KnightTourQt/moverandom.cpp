#include "knighttour.h"
#include "ui_knighttour.h"

int KnightTour::moveKnightRandom(int * step)
{
    int i;
    int res = 0; /* res = 1 if knight made a move */

    /* Otherwise there would be end-of-line
     * after each output to qDebug() */
    QDebug dbg(QtDebugMsg);
    dbg << "";

    int tries;
    tries = 0x00000000;

    // printf("%08x\n", tries);

    while ( !(res) && (tries ^ 0x11111111))
    {

        i = 1 + (rand() % 8);
        // printf("%d\n", i);
        //printf("%08x\n", tries);

        switch (i)
        {
            case 1:
                    if (!(tries & 0x00000001))
                    {
                        res = moveUpLeft();
                        tries |= 0x00000001;
                        // printf("%08x\n", tries);
                    }
                    break;
            case 2:
                    if (!(tries & 0x00000010))
                    {
                        res = moveLeftUp();
                        tries |= 0x00000010;
                        // printf("%08x\n", tries);
                    }
                    break;
            case 3:
                    if (!(tries & 0x00000100))
                    {
                        res = moveUpRight();
                        tries |= 0x00000100;
                        // printf("%08x\n", tries);
                    }
                    break;
            case 4:
                    if (!(tries & 0x00001000))
                    {
                        res = moveRightUp();
                        tries |= 0x00001000;
                        // printf("%08x\n", tries);
                    }
                    break;
            case 5:
                    if (!(tries & 0x00010000))
                    {
                        res = moveDownLeft();
                        tries |= 0x00010000;
                        // printf("%08x\n", tries);
                    }
                    break;
            case 6:
                    if (!(tries & 0x00100000))
                    {
                        res = moveLeftDown();
                        tries |= 0x00100000;
                        // printf("%08x\n", tries);
                    }
                    break;
            case 7:
                    if (!(tries & 0x01000000))
                    {
                        res = moveDownRight();
                        tries |= 0x01000000;
                        // printf("%08x\n", tries);
                    }
                    break;
            case 8:
                    if (!(tries & 0x10000000))
                    {
                        res = moveRightDown();
                        tries |= 0x10000000;
                        // printf("%08x\n", tries);
                    }
                    break;
            default:
                    break;
        }
    }

    /* We left a loop if Knight picked a move (res = 1)
     * or if Knight tried every possible move and failed */
    if (res)
    {
        (*step)++;
        DecreaseAccessabilitySurrounding();
    }
    else if ( (*step) != (SIZE * SIZE -1) )
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

    return res;
}

/*-------------------------------------------*/
int KnightTour::moveUpLeft()
{
    if (y - 2 >= 0 && x - 1 >= 0)
    {
        if ( board[y-2][x-1] == 0 )
        {
            y-=2;
            x-=1;
            board[y][x] = 1;
            return 1;
        }
    }
    return 0;
}
/*-------------------------------------------*/
int KnightTour::moveLeftUp()
{
    if (y - 1 >= 0 && x - 2 >= 0)
    {
        if ( board[y-1][x-2] == 0 )
        {
            y-=1;
            x-=2;
            board[y][x] = 1;
            return 1;
        }
    }
    return 0;
}
/*-------------------------------------------*/
int KnightTour::moveUpRight()
{
    if (y - 2 >= 0 && x + 1 <= 7)
    {
        if ( board[y-2][x+1] == 0 )
        {
            y-=2;
            x+=1;
            board[y][x] = 1;
            return 1;
        }
    }
    return 0;
}
/*-------------------------------------------*/
int KnightTour::moveRightUp()
{
    if (y - 1 >= 0 && x + 2 <= 7)
    {
        if ( board[y-1][x+2] == 0 )
        {
            y-=1;
            x+=2;
            board[y][x] = 1;
            return 1;
        }
    }
    return 0;
}
/*-------------------------------------------*/
int KnightTour::moveDownLeft()
{
    if (y + 2 <= 7 && x - 1 >= 0)
    {
        if ( board[y+2][x-1] == 0 )
        {
            y+=2;
            x-=1;
            board[y][x] = 1;
            return 1;
        }
    }
    return 0;
}
/*-------------------------------------------*/
int KnightTour::moveLeftDown()
{
    if (y + 1 <= 7 && x - 2 >= 0)
    {
        if ( board[y+1][x-2] == 0 )
        {
            y+=1;
            x-=2;
            board[y][x] = 1;
            return 1;
        }
    }
    return 0;
}
/*-------------------------------------------*/
int KnightTour::moveDownRight()
{
    if (y + 2 <= 7 && x + 1 <= 7)
    {
        if ( board[y+2][x+1] == 0 )
        {
            y+=2;
            x+=1;
            board[y][x] = 1;
            return 1;
        }
    }
    return 0;
}
/*-------------------------------------------*/
int KnightTour::moveRightDown()
{
    if (y + 1 <= 7 && x + 2 <= 7)
    {
        if ( board[y+1][x+2] == 0 )
        {
            y+=1;
            x+=2;
            board[y][x] = 1;
            return 1;
        }
    }
    return 0;
}
