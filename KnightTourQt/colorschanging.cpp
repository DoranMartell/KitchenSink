#include "knighttour.h"
#include "ui_knighttour.h"

/* set color of square when knight is on it */
#define KNIGHT "background: rgb(255,0,0); color:white"

/* set color of square which was visited by the knight already */

/* squares which sum of coordinates is even are originally white*/
#define SUM_EVEN "background: rgb(172,172,172)"

/* square which sum of coordinates is odd are originally dark brown */
#define SUM_ODD "background: rgb(112,90,90); color:white"

/* set color of square while refreshing board */

/* original white color for squares having even sum of coordinates*/
#define ORIG_SUM_EVEN "background: rgb(255,255,255)"

/* original dark brown color for squares having odd sum of coordinates*/
#define ORIG_SUM_ODD "background: rgb(112,19,19); color:white"

void KnightTour::changeColorKnight(int x, int y, int knight, int refresh)
{
    QString Style;

    if (knight)
    {
        Style = KNIGHT;
    }
    else if ( (x + y) % 2 == 0)
    {
        if (refresh)
            Style = ORIG_SUM_EVEN;
        else
            Style = SUM_EVEN;
    }
    else
    {
        if (refresh)
            Style = ORIG_SUM_ODD;
        else
            Style = SUM_ODD;
    }

    switch (x)
    {
        case 0: changeColorKnightZero(y, Style); break;
        case 1: changeColorKnightOne(y, Style); break;
        case 2: changeColorKnightTwo(y, Style); break;
        case 3: changeColorKnightThree(y, Style); break;
        case 4: changeColorKnightFour(y, Style); break;
        case 5: changeColorKnightFive(y, Style); break;
        case 6: changeColorKnightSix(y, Style); break;
        case 7: changeColorKnightSeven(y, Style); break;
        default : break;
    }
}

void KnightTour::changeColorKnightZero(int y, QString Style)
{
    switch (y)
    {
        case 0: ui->square00->setStyleSheet(Style); break;
        case 1: ui->square10->setStyleSheet(Style); break;
        case 2: ui->square20->setStyleSheet(Style); break;
        case 3: ui->square30->setStyleSheet(Style); break;
        case 4: ui->square40->setStyleSheet(Style); break;
        case 5: ui->square50->setStyleSheet(Style); break;
        case 6: ui->square60->setStyleSheet(Style); break;
        case 7: ui->square70->setStyleSheet(Style); break;
        default : break;
    }
}

void KnightTour::changeColorKnightOne(int y, QString Style)
{
    switch (y)
    {
        case 0: ui->square01->setStyleSheet(Style); break;
        case 1: ui->square11->setStyleSheet(Style); break;
        case 2: ui->square21->setStyleSheet(Style); break;
        case 3: ui->square31->setStyleSheet(Style); break;
        case 4: ui->square41->setStyleSheet(Style); break;
        case 5: ui->square51->setStyleSheet(Style); break;
        case 6: ui->square61->setStyleSheet(Style); break;
        case 7: ui->square71->setStyleSheet(Style); break;
        default : break;
    }
}

void KnightTour::changeColorKnightTwo(int y, QString Style)
{
    switch (y)
    {
        case 0: ui->square02->setStyleSheet(Style); break;
        case 1: ui->square12->setStyleSheet(Style); break;
        case 2: ui->square22->setStyleSheet(Style); break;
        case 3: ui->square32->setStyleSheet(Style); break;
        case 4: ui->square42->setStyleSheet(Style); break;
        case 5: ui->square52->setStyleSheet(Style); break;
        case 6: ui->square62->setStyleSheet(Style); break;
        case 7: ui->square72->setStyleSheet(Style); break;
        default : break;
    }
}

void KnightTour::changeColorKnightThree(int y, QString Style)
{
    switch (y)
    {
        case 0: ui->square03->setStyleSheet(Style); break;
        case 1: ui->square13->setStyleSheet(Style); break;
        case 2: ui->square23->setStyleSheet(Style); break;
        case 3: ui->square33->setStyleSheet(Style); break;
        case 4: ui->square43->setStyleSheet(Style); break;
        case 5: ui->square53->setStyleSheet(Style); break;
        case 6: ui->square63->setStyleSheet(Style); break;
        case 7: ui->square73->setStyleSheet(Style); break;
        default : break;
    }
}

void KnightTour::changeColorKnightFour(int y, QString Style)
{
    switch (y)
    {
        case 0: ui->square04->setStyleSheet(Style); break;
        case 1: ui->square14->setStyleSheet(Style); break;
        case 2: ui->square24->setStyleSheet(Style); break;
        case 3: ui->square34->setStyleSheet(Style); break;
        case 4: ui->square44->setStyleSheet(Style); break;
        case 5: ui->square54->setStyleSheet(Style); break;
        case 6: ui->square64->setStyleSheet(Style); break;
        case 7: ui->square74->setStyleSheet(Style); break;
        default : break;
    }
}

void KnightTour::changeColorKnightFive(int y, QString Style)
{
    switch (y)
    {
        case 0: ui->square05->setStyleSheet(Style); break;
        case 1: ui->square15->setStyleSheet(Style); break;
        case 2: ui->square25->setStyleSheet(Style); break;
        case 3: ui->square35->setStyleSheet(Style); break;
        case 4: ui->square45->setStyleSheet(Style); break;
        case 5: ui->square55->setStyleSheet(Style); break;
        case 6: ui->square65->setStyleSheet(Style); break;
        case 7: ui->square75->setStyleSheet(Style); break;
        default : break;
    }
}

void KnightTour::changeColorKnightSix(int y, QString Style)
{
    switch (y)
    {
        case 0: ui->square06->setStyleSheet(Style); break;
        case 1: ui->square16->setStyleSheet(Style); break;
        case 2: ui->square26->setStyleSheet(Style); break;
        case 3: ui->square36->setStyleSheet(Style); break;
        case 4: ui->square46->setStyleSheet(Style); break;
        case 5: ui->square56->setStyleSheet(Style); break;
        case 6: ui->square66->setStyleSheet(Style); break;
        case 7: ui->square76->setStyleSheet(Style); break;
        default : break;
    }
}

void KnightTour::changeColorKnightSeven(int y, QString Style)
{
    switch (y)
    {
        case 0: ui->square07->setStyleSheet(Style); break;
        case 1: ui->square17->setStyleSheet(Style); break;
        case 2: ui->square27->setStyleSheet(Style); break;
        case 3: ui->square37->setStyleSheet(Style); break;
        case 4: ui->square47->setStyleSheet(Style); break;
        case 5: ui->square57->setStyleSheet(Style); break;
        case 6: ui->square67->setStyleSheet(Style); break;
        case 7: ui->square77->setStyleSheet(Style); break;
        default : break;
    }
}
