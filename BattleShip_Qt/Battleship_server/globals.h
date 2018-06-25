#ifndef GLOBALS
#define GLOBALS

#include <QString>
#include <vector>
#include <mutex>

#define BUFFSIZE 1024

extern int shouldHideAskWindow;
extern int nameSet[2];

extern bool ServerInitiatedShutdown;

extern QString PendingColor;
extern std::mutex color_mu;

extern QString GetPendingColor();
extern void SetPendingColor(QString );

typedef enum
{
    white, red, yellow, green, grey
} color_t;

extern std::vector<QString> CoordsOfShipKilled;
extern std::mutex shipkilled_mu;

extern void AddCoordShipKilled(QString );
extern void ClearShipKilled();
extern QString GetAndPopShipKilled();
extern bool ShipKilledIsEmpty();

#endif // GLOBALS

