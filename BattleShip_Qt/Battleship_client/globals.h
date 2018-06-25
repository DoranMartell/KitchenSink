#ifndef GLOBALS
#define GLOBALS

#define BUFFSIZE 1024

#include <vector>
#include <QString>
#include <QDebug>

#include <thread>
#include <mutex>
#include <string>
#include <sstream>

extern int shouldHideAskServWindow;

extern std::vector<QString> BattleShipCoords;
extern std::vector<QString> CruisersCoords;
extern std::vector<QString> DestroyersCoords;
extern std::vector<QString> TorpedoBoatsCoords;

extern QString GetQStrThreadId(std::thread::id );

extern QString AdversaryName;

extern QString OneGreenSquareChosen;
extern std::mutex square_mu;
extern void SetOneGreenSquareChosen(QString );
extern QString GetOneGreenSquareChosen();

extern std::mutex mess_mu;

extern bool StartGame;
extern void SetStartGame(bool );
extern bool GetStartGame();

extern std::mutex closing_mu;
extern bool ServerUp;
extern bool GetServerUp();
extern void SetServerUp(bool );

extern bool NoNameReply;
extern std::mutex name_reply;
extern bool NoNameReplyYet();
extern void SetNoNameReply(bool );

extern bool NameOk;
extern std::mutex name_ok;
extern bool getNameOk();
extern void setNameOk(bool );

extern QString GetCoords(QString );

typedef enum
{
    white, red, yellow, green, grey
} color_t;

#endif // GLOBALS

