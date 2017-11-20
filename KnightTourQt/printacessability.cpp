#include "knighttour.h"
#include "ui_knighttour.h"


int KnightTour::printAccessability()
{
    int i, j;

    QDebug dbg(QtDebugMsg);
    dbg << "";

    dbg << "Board's accessability:\n";

    for (i = 0; i < SIZE; i++)
    {
        for (j = 0 ; j < SIZE; j++)
        {
            dbg << accessability[i][j];
            if ( ui->ShowAccessability->checkState() == 2 )
            {
                PrintAccessSquare(i, j, 1);
            }
            else
            {
                PrintAccessSquare(i, j, 0);
            }
        }
        dbg << "\n";
    }

    return 1; /* always, as it is not void only to be inside of while condition */
}


void KnightTour::PrintAccessSquare(int x, int y, int checked)
{
    switch (x)
    {
        case 0: PrintAccessSquareZero(y, checked); break;
        case 1: PrintAccessSquareOne(y, checked); break;
        case 2: PrintAccessSquareTwo(y, checked); break;
        case 3: PrintAccessSquareThree(y, checked); break;
        case 4: PrintAccessSquareFour(y, checked); break;
        case 5: PrintAccessSquareFive(y, checked); break;
        case 6: PrintAccessSquareSix(y, checked); break;
        case 7: PrintAccessSquareSeven(y, checked); break;
        default : break;
    }
}

void KnightTour::PrintAccessSquareZero(int y, int checked)
{
    switch (y)
    {
        case 0: if (checked) ui->square00->setText(QString::number(accessability[0][0]));
                    else ui->square00->setText("");
                break;
        case 1: if (checked) ui->square10->setText(QString::number(accessability[1][0]));
                    else ui->square10->setText("");
                break;
        case 2: if (checked) ui->square20->setText(QString::number(accessability[2][0]));
                    else ui->square20->setText("");
                break;
        case 3: if (checked) ui->square30->setText(QString::number(accessability[3][0]));
                    else ui->square30->setText("");
                break;
        case 4: if (checked) ui->square40->setText(QString::number(accessability[4][0]));
                    else ui->square40->setText("");
                break;
        case 5: if (checked) ui->square50->setText(QString::number(accessability[5][0]));
                    else ui->square50->setText("");
                break;
        case 6: if (checked) ui->square60->setText(QString::number(accessability[6][0]));
                    else ui->square60->setText("");
                break;
        case 7: if (checked) ui->square70->setText(QString::number(accessability[7][0]));
                    else ui->square70->setText("");
                break;
        default : break;
    }
}

void KnightTour::PrintAccessSquareOne(int y, int checked)
{
    switch (y)
    {
        case 0: if (checked) ui->square01->setText(QString::number(accessability[0][1]));
                    else ui->square01->setText("");
                break;
        case 1: if (checked) ui->square11->setText(QString::number(accessability[1][1]));
                    else ui->square11->setText("");
                break;
        case 2: if (checked) ui->square21->setText(QString::number(accessability[2][1]));
                    else ui->square21->setText("");
                break;
        case 3: if (checked) ui->square31->setText(QString::number(accessability[3][1]));
                    else ui->square31->setText("");
                break;
        case 4: if (checked) ui->square41->setText(QString::number(accessability[4][1]));
                    else ui->square41->setText("");
                break;
        case 5: if (checked) ui->square51->setText(QString::number(accessability[5][1]));
                    else ui->square51->setText("");
                break;
        case 6: if (checked) ui->square61->setText(QString::number(accessability[6][1]));
                    else ui->square61->setText("");
                break;
        case 7: if (checked) ui->square71->setText(QString::number(accessability[7][1]));
                    else ui->square71->setText("");
                break;
        default : break;
    }
}

void KnightTour::PrintAccessSquareTwo(int y, int checked)
{
    switch (y)
    {
        case 0: if (checked) ui->square02->setText(QString::number(accessability[0][2]));
                    else ui->square02->setText("");
                break;
        case 1: if (checked) ui->square12->setText(QString::number(accessability[1][2]));
                    else ui->square12->setText("");
                break;
        case 2: if (checked) ui->square22->setText(QString::number(accessability[2][2]));
                    else ui->square22->setText("");
                break;
        case 3: if (checked) ui->square32->setText(QString::number(accessability[3][2]));
                    else ui->square32->setText("");
                break;
        case 4: if (checked) ui->square42->setText(QString::number(accessability[4][2]));
                    else ui->square42->setText("");
                break;
        case 5: if (checked) ui->square52->setText(QString::number(accessability[5][2]));
                    else ui->square52->setText("");
                break;
        case 6: if (checked) ui->square62->setText(QString::number(accessability[6][2]));
                    else ui->square62->setText("");
                break;
        case 7: if (checked) ui->square72->setText(QString::number(accessability[7][2]));
                    else ui->square72->setText("");
                break;
        default : break;
    }
}

void KnightTour::PrintAccessSquareThree(int y, int checked)
{
    switch (y)
    {
        case 0: if (checked) ui->square03->setText(QString::number(accessability[0][3]));
                    else ui->square03->setText("");
                break;
        case 1: if (checked) ui->square13->setText(QString::number(accessability[1][3]));
                    else ui->square13->setText("");
                break;
        case 2: if (checked) ui->square23->setText(QString::number(accessability[2][3]));
                    else ui->square23->setText("");
                break;
        case 3: if (checked) ui->square33->setText(QString::number(accessability[3][3]));
                    else ui->square33->setText("");
                break;
        case 4: if (checked) ui->square43->setText(QString::number(accessability[4][3]));
                    else ui->square43->setText("");
                break;
        case 5: if (checked) ui->square53->setText(QString::number(accessability[5][3]));
                    else ui->square53->setText("");
                break;
        case 6: if (checked) ui->square63->setText(QString::number(accessability[6][3]));
                    else ui->square63->setText("");
                break;
        case 7: if (checked) ui->square73->setText(QString::number(accessability[7][3]));
                    else ui->square73->setText("");
                break;
        default : break;
    }
}

void KnightTour::PrintAccessSquareFour(int y, int checked)
{
    switch (y)
    {
        case 0: if (checked) ui->square04->setText(QString::number(accessability[0][4]));
                    else ui->square04->setText("");
                break;
        case 1: if (checked) ui->square14->setText(QString::number(accessability[1][4]));
                    else ui->square14->setText("");
                break;
        case 2: if (checked) ui->square24->setText(QString::number(accessability[2][4]));
                    else ui->square24->setText("");
                break;
        case 3: if (checked) ui->square34->setText(QString::number(accessability[3][4]));
                    else ui->square34->setText("");
                break;
        case 4: if (checked) ui->square44->setText(QString::number(accessability[4][4]));
                    else ui->square44->setText("");
                break;
        case 5: if (checked) ui->square54->setText(QString::number(accessability[5][4]));
                    else ui->square54->setText("");
                break;
        case 6: if (checked) ui->square64->setText(QString::number(accessability[6][4]));
                    else ui->square64->setText("");
                break;
        case 7: if (checked) ui->square74->setText(QString::number(accessability[7][4]));
                    else ui->square74->setText("");
                break;
        default : break;
    }
}

void KnightTour::PrintAccessSquareFive(int y, int checked)
{
    switch (y)
    {
        case 0: if (checked) ui->square05->setText(QString::number(accessability[0][5]));
                    else ui->square05->setText("");
                break;
        case 1: if (checked) ui->square15->setText(QString::number(accessability[1][5]));
                    else ui->square15->setText("");
                break;
        case 2: if (checked) ui->square25->setText(QString::number(accessability[2][5]));
                    else ui->square25->setText("");
                break;
        case 3: if (checked) ui->square35->setText(QString::number(accessability[3][5]));
                    else ui->square35->setText("");
                break;
        case 4: if (checked) ui->square45->setText(QString::number(accessability[4][5]));
                    else ui->square45->setText("");
                break;
        case 5: if (checked) ui->square55->setText(QString::number(accessability[5][5]));
                    else ui->square55->setText("");
                break;
        case 6: if (checked) ui->square65->setText(QString::number(accessability[6][5]));
                    else ui->square65->setText("");
                break;
        case 7: if (checked) ui->square75->setText(QString::number(accessability[7][5]));
                    else ui->square75->setText("");
                break;
        default : break;
    }
}

void KnightTour::PrintAccessSquareSix(int y, int checked)
{
    switch (y)
    {
        case 0: if (checked) ui->square06->setText(QString::number(accessability[0][6]));
                    else ui->square06->setText("");
                break;
        case 1: if (checked) ui->square16->setText(QString::number(accessability[1][6]));
                    else ui->square16->setText("");
                break;
        case 2: if (checked) ui->square26->setText(QString::number(accessability[2][6]));
                    else ui->square26->setText("");
                break;
        case 3: if (checked) ui->square36->setText(QString::number(accessability[3][6]));
                    else ui->square36->setText("");
                break;
        case 4: if (checked) ui->square46->setText(QString::number(accessability[4][6]));
                    else ui->square46->setText("");
                break;
        case 5: if (checked) ui->square56->setText(QString::number(accessability[5][6]));
                    else ui->square56->setText("");
                break;
        case 6: if (checked) ui->square66->setText(QString::number(accessability[6][6]));
                    else ui->square66->setText("");
                break;
        case 7: if (checked) ui->square76->setText(QString::number(accessability[7][6]));
                    else ui->square76->setText("");
                break;
        default : break;
    }
}

void KnightTour::PrintAccessSquareSeven(int y, int checked)
{
    switch (y)
    {
        case 0: if (checked) ui->square07->setText(QString::number(accessability[0][7]));
                    else ui->square07->setText("");
                break;
        case 1: if (checked) ui->square17->setText(QString::number(accessability[1][7]));
                    else ui->square17->setText("");
                break;
        case 2: if (checked) ui->square27->setText(QString::number(accessability[2][7]));
                    else ui->square27->setText("");
                break;
        case 3: if (checked) ui->square37->setText(QString::number(accessability[3][7]));
                    else ui->square37->setText("");
                break;
        case 4: if (checked) ui->square47->setText(QString::number(accessability[4][7]));
                    else ui->square47->setText("");
                break;
        case 5: if (checked) ui->square57->setText(QString::number(accessability[5][7]));
                    else ui->square57->setText("");
                break;
        case 6: if (checked) ui->square67->setText(QString::number(accessability[6][7]));
                    else ui->square67->setText("");
                break;
        case 7: if (checked) ui->square77->setText(QString::number(accessability[7][7]));
                    else ui->square77->setText("");
                break;
        default : break;
    }
}
