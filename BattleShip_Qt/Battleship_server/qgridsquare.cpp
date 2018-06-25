#include "qgridsquare.h"

QGridSquare::QGridSquare(QWidget * parent) : QLabel(parent)
{
    changeColor(white);
}
/*----------------------------------------------------------------------------------*/
color_t QGridSquare::getColor() const
{
    return color;
}
/*----------------------------------------------------------------------------------*/
void QGridSquare::changeColor(color_t col)
{
    color = col;

    if (col == grey)
    {
        setStyleSheet("background: rgb(153, 153, 153);");
    }
    else if (col == green)
    {
        setStyleSheet("background: rgb(0, 255, 51);");
    }
    else if (col == red)
    {
        setStyleSheet("background: rgb(255, 0, 0);");
    }
    else if (col == yellow)
    {
        setStyleSheet("background: rgb(255, 255, 0);");
    }
    else if (col == white)
    {
        setStyleSheet("background: rgb(255, 255, 255);");
    }
}

/*----------------------------------------------------------------------------------*/
QGridSquare::~QGridSquare()
{

}
