#include "globals.h"

int shouldHideAskServWindow = 0;

std::vector<QString> BattleShipCoords;
std::vector<QString> CruisersCoords;
std::vector<QString> DestroyersCoords;
std::vector<QString> TorpedoBoatsCoords;

QString AdversaryName;

QString OneGreenSquareChosen = "";
std::mutex square_mu;

/*----------------------------------------------------------------------------------*/
void SetOneGreenSquareChosen(QString Coordinate)
{
    qDebug() << "Setting Chosen Green Square coordinate to" << Coordinate;

    if (Coordinate.isEmpty())
    {
        qDebug() << "No Green square currently chosen";
    }

    square_mu.lock();
    OneGreenSquareChosen = Coordinate;
    square_mu.unlock();

}
/*----------------------------------------------------------------------------------*/
QString GetOneGreenSquareChosen()
{
    square_mu.lock();
    QString result = OneGreenSquareChosen;
    square_mu.unlock();
    return result;
}
/*----------------------------------------------------------------------------------*/
bool StartGame = false;
std::mutex mess_mu;
std::mutex closing_mu;
bool ServerUp = true;

/*----------------------------------------------------------------------------------*/
void SetStartGame(bool val)
{
    mess_mu.lock();
    StartGame = val;
    mess_mu.unlock();
}
/*----------------------------------------------------------------------------------*/
bool GetStartGame()
{
    mess_mu.lock();

    bool st = StartGame;

    mess_mu.unlock();

    return st;
}

/*----------------------------------------------------------------------------------*/
QString GetQStrThreadId(std::thread::id ThrId)
{
    std::stringstream ThreadID;
    ThreadID << ThrId;
    std::string StrThrId = ThreadID.str();
    QString QStrThrId = QString::fromStdString(StrThrId);

    return QStrThrId;
}
/*----------------------------------------------------------------------------------*/
bool GetServerUp()
{
    bool res;
    closing_mu.lock();
    res = ServerUp;
    closing_mu.unlock();
    return res;
}
/*----------------------------------------------------------------------------------*/
void SetServerUp(bool val)
{
    closing_mu.lock();
    ServerUp = val;
    closing_mu.unlock();
}

/*----------------------------------------------------------------------------------*/
QString GetCoords(QString type_of_ship)
{
    qDebug() << "Getting coordinates of" << type_of_ship << "ship from vector to a single string...";
    std::vector <QString> whatever;

    int i = 1;
    int separator_pos, ending_pos;

    if (type_of_ship == "BS")
    {
        whatever = BattleShipCoords;
        separator_pos = 4;
        ending_pos = 4;
    }
    else if (type_of_ship == "CR")
    {
        whatever = CruisersCoords;
        separator_pos = 3;
        ending_pos = 6;
    }
    else if (type_of_ship == "DS")
    {
        whatever = DestroyersCoords;
        separator_pos = 2;
        ending_pos = 6;
    }
    else if (type_of_ship == "TB")
    {
        whatever = TorpedoBoatsCoords;
        separator_pos = 1;
        ending_pos = 4;
    }

    QString resulting_string;

    while (!whatever.empty())
    {
        if (i == ending_pos && type_of_ship == "TB")
            resulting_string += whatever.back();
        else if (i % separator_pos == 0)
            resulting_string += whatever.back() + ", ";
        else
            resulting_string += whatever.back() + " ";

        qDebug() << "Adding coordinate" << whatever.back();
        whatever.pop_back();
        i++;
    }

    qDebug() << "Coordinates moved to a single string (possibly, to be sent to server)" << resulting_string;
    return resulting_string;
}
/*----------------------------------------------------------------------------------*/
bool NoNameReply;
std::mutex name_reply;
/*----------------------------------------------------------------------------------*/
bool NoNameReplyYet()
{
    bool res;
    name_reply.lock();
    res = NoNameReply;
    name_reply.unlock();
    return res;
}
/*----------------------------------------------------------------------------------*/
void SetNoNameReply(bool val)
{
    name_reply.lock();
    NoNameReply = val;
    name_reply.unlock();
}
/*----------------------------------------------------------------------------------*/
bool NameOk;
std::mutex name_ok;
/*----------------------------------------------------------------------------------*/
bool getNameOk()
{
    bool res;
    name_ok.lock();
    res = NameOk;
    name_ok.unlock();
    return res;
}
/*----------------------------------------------------------------------------------*/
void setNameOk(bool val)
{
    name_ok.lock();
    NameOk = val;
    name_ok.unlock();
}
