#ifndef QGRIDSQUARE_H
#define QGRIDSQUARE_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QToolTip>
#include <QEvent>

#include <QDebug>

#include "globals.h"

class QGridSquare : public QLabel
{
    Q_OBJECT

public:

    QGridSquare(QWidget * parent = 0);
    ~QGridSquare();

    void changeColor(/*QString*/ color_t );
    /*QString*/ color_t getColor() const;

private:

    /*QString*/ color_t color;
    QString name;

};

#endif // QGRIDSQUARE_H
