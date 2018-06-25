#ifndef QGRIDSQUARE_H
#define QGRIDSQUARE_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QToolTip>
#include <QEvent>

#include <QMouseEvent>

#include <QDebug>

#include "globals.h"


/* for usleep() function */
#include <unistd.h>

class QGridSquare : public QLabel
{
    Q_OBJECT

public:

    QGridSquare(QWidget * parent = 0);
    ~QGridSquare();

    void changeColor(/*QString*/ color_t );
    /*int*/ color_t getColor() const;
    int getOverlap() const;
    void setClickability(bool );
    void setName(QString );

    bool GreenSquareClickedTracking;

signals:
    void ChangeOthersClickabilitySig(QString );
    void RandomButtonEnableSig(bool );

protected:

    void mousePressEvent(QMouseEvent * mouse_event);

private:

    /*int*/ color_t color;
    int overlap; /* if we set same color on a square twice */
    bool SquareClickable;

    QString name;

};

#endif // QGRIDSQUARE_H
