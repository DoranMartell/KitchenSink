#include "globals.h"

int shouldHideAskWindow = 0;
int nameSet[2] = {0, 0};

bool ServerInitiatedShutdown;

QString PendingColor;
std::mutex color_mu;

QString GetPendingColor()
{
    QString result;
    color_mu.lock();
    result = PendingColor;
    color_mu.unlock();
    return result;
}

void SetPendingColor(QString col)
{
    color_mu.lock();
    PendingColor = col;
    color_mu.unlock();
}

std::vector<QString> CoordsOfShipKilled;
std::mutex shipkilled_mu;

void AddCoordShipKilled(QString Coordinate)
{
    shipkilled_mu.lock();
    CoordsOfShipKilled.push_back(Coordinate);
    shipkilled_mu.unlock();
}

void ClearShipKilled()
{
    shipkilled_mu.lock();
    CoordsOfShipKilled.clear();
    shipkilled_mu.unlock();
}

QString GetAndPopShipKilled()
{
    QString result;
    shipkilled_mu.lock();
    result = CoordsOfShipKilled.back();
    CoordsOfShipKilled.pop_back();
    shipkilled_mu.unlock();

    return result;
}

bool ShipKilledIsEmpty()
{
    bool result;
    shipkilled_mu.lock();
    result = CoordsOfShipKilled.empty();
    shipkilled_mu.unlock();

    return result;
}
