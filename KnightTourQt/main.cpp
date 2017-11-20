#include "knighttour.h"
#include "knight_move_fnc.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    KnightTour w;
    w.show();

    return a.exec();
}
