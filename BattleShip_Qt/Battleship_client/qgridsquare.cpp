#include "qgridsquare.h"

/*----------------------------------------------------------------------------------*/
QGridSquare::QGridSquare(QWidget * parent) : QLabel(parent)
{
    changeColor(white);
    GreenSquareClickedTracking = true;
}

/*----------------------------------------------------------------------------------*/
void QGridSquare::mousePressEvent(QMouseEvent *mouse_event)
{
    if (SquareClickable)
    {
        if (mouse_event->button() == Qt::LeftButton)
        {
            if (getColor() == green)
            {
                changeColor(white);

                /* this is supposed to work only on main client window,
                    not when you choose a place for ships */
                if (GreenSquareClickedTracking)
                {
                    SetOneGreenSquareChosen("");
                }
                //emit RandomButtonEnableSig(true);
            }
            else if (getColor() == white)
            {
                changeColor(green);

                /* this is supposed to work only on main client window,
                    not when you choose a place for ships */
                if (GreenSquareClickedTracking)
                {
                    SetOneGreenSquareChosen(objectName());
                }
                //emit RandomButtonEnableSig(true);
            }
        }

        emit ChangeOthersClickabilitySig(this->objectName());
    }
}

/*----------------------------------------------------------------------------------*/
void QGridSquare::changeColor(color_t col)
{
    if (col == white)
    {
        if (getColor() == grey && overlap > 0)
        {
            overlap--;
        }
        else
        {
            //qDebug() << "I am in changeColor(WHITE), my color was" << getColor();
            setStyleSheet(" background: rgb(255, 255, 255);");
            color = white;
            overlap = 0;
            //qDebug() << "Square color is now" << getColor();

            // This is not supposed to happen anywhere other than FillGridWindow
            setClickability(true);

        }
    }
    else if (col == green)
    {
        setStyleSheet("background: rgb(0, 255, 51);");
        if (SquareClickable)
        {
            setClickability(true);
        }
        color = green;
        overlap = 0;
    }
    else if (col == red)
    {
        //qDebug() << "I am in changeColor(RED)";
        setStyleSheet("background: rgb(255, 0, 0);");
        setClickability(false);
        color = red;
        overlap = 0;
        //qDebug() << "Square color is now" << getColor();
    }
    else if (col == yellow)
    {
        setStyleSheet("background: rgb(255, 255, 0);");
        setClickability(false);
        color = yellow;
        overlap = 0;
    }
    else if (col == grey)
    {
        if (getColor() == grey)
        {
            overlap++;
        }
        else
        {
            setStyleSheet("background: rgb(153, 153, 153);");
            color = grey;
            overlap = 0;
        }
        setClickability(false);
    }

}

/*----------------------------------------------------------------------------------*/
color_t QGridSquare::getColor() const
{
    return color;
}
/*----------------------------------------------------------------------------------*/
int QGridSquare::getOverlap() const
{
    return overlap;
}
/*----------------------------------------------------------------------------------*/
void QGridSquare::setClickability(bool val)
{
    SquareClickable = val;

    /*if (!name.isEmpty())
        qDebug() << "I am" << name << "am I clickable now? - " << SquareClickable;*/

    if (val == true)
    {
        setCursor(Qt::CrossCursor);
    }
    else
    {
        setCursor(Qt::ArrowCursor);
    }
}
/*----------------------------------------------------------------------------------*/
void QGridSquare::setName(QString suggested_name)
{
    name = suggested_name;
}
/*----------------------------------------------------------------------------------*/
QGridSquare::~QGridSquare()
{

}

