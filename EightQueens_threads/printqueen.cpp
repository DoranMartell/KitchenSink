#include "queens.h"
#include "ui_queens.h"
#include <math.h>
#include "queenthread.h"

/*------------------------------------------------------------------------------------------------*/
void Queens::printBoard(QString Message, int timeout)
{
    int i, j;

//    if (!thread.finished())
//    {
        //    QDebug dbg(QtDebugMsg);
        //    dbg << "";

        //    dbg << "----------------------------\n";
        //    dbg << "Board with "<< thread.queensOnBoard << " queens placed:\n";
        //    dbg << "----------------------------\n";

            qDebug() << thread.queensOnBoard;
        //    ui->statusBar->showMessage(Message, timeout);

            for ( i = 0; i < SIZE; i++)
            {
                for (j = 0; j < SIZE; j++)
                {

                    if (thread.board[i][j] >= 900)
                    {
        //                dbg << "Q";
                        printQueen(i, j, 2);
                    }
                    else if (thread.board[i][j] > 0 && thread.board[i][j] < 900)
                    {
        //                dbg << "1";
                        printQueen(i, j, 1);
                    }
                    else
                    {
        //                dbg << "0";
                        printQueen(i, j, 0);
                    }
                }
        //        dbg << "\n";
            }

        //    dbg << "\n";

            ui->statusBar->showMessage(Message, timeout);
//    }
}

/*------------------------------------------------------------------------------------------------*/

void Queens::printQueen(int y, int x, int color)
{
    QString sheetstyle;

    if (color == 2)
    {
        sheetstyle = "background: rgb(255, 0, 0);";
    }
    else if (color == 0)
    {
        if ((x + y) % 2 == 0)
        {
            sheetstyle = "background: rgb(255, 255, 255);";
        }
        else
        {
            sheetstyle = "background: rgb(112, 19, 19);";
        }
    }
    else if (color == 1)
    {
        if ((x + y) % 2 == 0)
        {
            sheetstyle = "background: rgb(172, 172, 172);";
        }
        else
        {
            sheetstyle = "background: rgb(112, 90, 90);";
        }
    }

    switch (x)
    {
        case 0 : printQueenZero(y, sheetstyle); break;
        case 1 : printQueenOne(y, sheetstyle); break;
        case 2 : printQueenTwo(y, sheetstyle); break;
        case 3 : printQueenThree(y, sheetstyle); break;
        case 4 : printQueenFour(y, sheetstyle); break;
        case 5 : printQueenFive(y, sheetstyle); break;
        case 6 : printQueenSix(y, sheetstyle); break;
        case 7 : printQueenSeven(y, sheetstyle); break;
        default : break;
    }
}

void Queens::printQueenZero(int y, QString sheetstyle)
{
    switch (y)
    {
    case 0 : ui->square00->setStyleSheet(sheetstyle); break;
    case 1 : ui->square10->setStyleSheet(sheetstyle); break;
    case 2 : ui->square20->setStyleSheet(sheetstyle); break;
    case 3 : ui->square30->setStyleSheet(sheetstyle); break;
    case 4 : ui->square40->setStyleSheet(sheetstyle); break;
    case 5 : ui->square50->setStyleSheet(sheetstyle); break;
    case 6 : ui->square60->setStyleSheet(sheetstyle); break;
    case 7 : ui->square70->setStyleSheet(sheetstyle); break;
        default : break;
    }
}

void Queens::printQueenOne(int y, QString sheetstyle)
{
    switch (y)
    {
    case 0 : ui->square01->setStyleSheet(sheetstyle); break;
    case 1 : ui->square11->setStyleSheet(sheetstyle); break;
    case 2 : ui->square21->setStyleSheet(sheetstyle); break;
    case 3 : ui->square31->setStyleSheet(sheetstyle); break;
    case 4 : ui->square41->setStyleSheet(sheetstyle); break;
    case 5 : ui->square51->setStyleSheet(sheetstyle); break;
    case 6 : ui->square61->setStyleSheet(sheetstyle); break;
    case 7 : ui->square71->setStyleSheet(sheetstyle); break;
        default : break;
    }
}

void Queens::printQueenTwo(int y, QString sheetstyle)
{
    switch (y)
    {
    case 0 : ui->square02->setStyleSheet(sheetstyle); break;
    case 1 : ui->square12->setStyleSheet(sheetstyle); break;
    case 2 : ui->square22->setStyleSheet(sheetstyle); break;
    case 3 : ui->square32->setStyleSheet(sheetstyle); break;
    case 4 : ui->square42->setStyleSheet(sheetstyle); break;
    case 5 : ui->square52->setStyleSheet(sheetstyle); break;
    case 6 : ui->square62->setStyleSheet(sheetstyle); break;
    case 7 : ui->square72->setStyleSheet(sheetstyle); break;
        default : break;
    }
}

void Queens::printQueenThree(int y, QString sheetstyle)
{
    switch (y)
    {
    case 0 : ui->square03->setStyleSheet(sheetstyle); break;
    case 1 : ui->square13->setStyleSheet(sheetstyle); break;
    case 2 : ui->square23->setStyleSheet(sheetstyle); break;
    case 3 : ui->square33->setStyleSheet(sheetstyle); break;
    case 4 : ui->square43->setStyleSheet(sheetstyle); break;
    case 5 : ui->square53->setStyleSheet(sheetstyle); break;
    case 6 : ui->square63->setStyleSheet(sheetstyle); break;
    case 7 : ui->square73->setStyleSheet(sheetstyle); break;
        default : break;
    }
}

void Queens::printQueenFour(int y, QString sheetstyle)
{
    switch (y)
    {
    case 0 : ui->square04->setStyleSheet(sheetstyle); break;
    case 1 : ui->square14->setStyleSheet(sheetstyle); break;
    case 2 : ui->square24->setStyleSheet(sheetstyle); break;
    case 3 : ui->square34->setStyleSheet(sheetstyle); break;
    case 4 : ui->square44->setStyleSheet(sheetstyle); break;
    case 5 : ui->square54->setStyleSheet(sheetstyle); break;
    case 6 : ui->square64->setStyleSheet(sheetstyle); break;
    case 7 : ui->square74->setStyleSheet(sheetstyle); break;
        default : break;
    }
}

void Queens::printQueenFive(int y, QString sheetstyle)
{
    switch (y)
    {
    case 0 : ui->square05->setStyleSheet(sheetstyle); break;
    case 1 : ui->square15->setStyleSheet(sheetstyle); break;
    case 2 : ui->square25->setStyleSheet(sheetstyle); break;
    case 3 : ui->square35->setStyleSheet(sheetstyle); break;
    case 4 : ui->square45->setStyleSheet(sheetstyle); break;
    case 5 : ui->square55->setStyleSheet(sheetstyle); break;
    case 6 : ui->square65->setStyleSheet(sheetstyle); break;
    case 7 : ui->square75->setStyleSheet(sheetstyle); break;
        default : break;
    }
}

void Queens::printQueenSix(int y, QString sheetstyle)
{
    switch (y)
    {
    case 0 : ui->square06->setStyleSheet(sheetstyle); break;
    case 1 : ui->square16->setStyleSheet(sheetstyle); break;
    case 2 : ui->square26->setStyleSheet(sheetstyle); break;
    case 3 : ui->square36->setStyleSheet(sheetstyle); break;
    case 4 : ui->square46->setStyleSheet(sheetstyle); break;
    case 5 : ui->square56->setStyleSheet(sheetstyle); break;
    case 6 : ui->square66->setStyleSheet(sheetstyle); break;
    case 7 : ui->square76->setStyleSheet(sheetstyle); break;
        default : break;
    }
}

void Queens::printQueenSeven(int y, QString sheetstyle)
{
    switch (y)
    {
    case 0 : ui->square07->setStyleSheet(sheetstyle); break;
    case 1 : ui->square17->setStyleSheet(sheetstyle); break;
    case 2 : ui->square27->setStyleSheet(sheetstyle); break;
    case 3 : ui->square37->setStyleSheet(sheetstyle); break;
    case 4 : ui->square47->setStyleSheet(sheetstyle); break;
    case 5 : ui->square57->setStyleSheet(sheetstyle); break;
    case 6 : ui->square67->setStyleSheet(sheetstyle); break;
    case 7 : ui->square77->setStyleSheet(sheetstyle); break;
        default : break;
    }
}
