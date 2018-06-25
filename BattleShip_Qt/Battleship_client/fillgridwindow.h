#ifndef FILLGRIDWINDOW_H
#define FILLGRIDWINDOW_H

#include <QDialog>
#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>
#include <QTime>

#include <map>
#include <vector>
#include <algorithm>

/* for usleep() function */
#include <unistd.h>

#include <thread>

#include "qgridsquare.h"
#include "globals.h"

namespace Ui {
class FillGridWindow;
}

class FillGridWindow : public QDialog
{
    Q_OBJECT

public:

    explicit FillGridWindow(QWidget *parent = 0);
    ~FillGridWindow();

    std::map<QString, QGridSquare *> Square;

    void LinkSquareMap();

    void GetCoordinatesOfGreenSquares(); /* Gets coordinates AND amount of green squares, for different types of ships
                                            those values are different */
    void PrintCurrentGreenSquaresVector();

    /*void PrintCurrentGreySquaresVector();*/

    void ClearCoordinatesOfGreenSquares();

    void SortCoordinatesOfGreenSquares();


    /*
      Create a parent class for both FillGridWindow and Client window
      and implement there virtual ConfirmGreenSquares which would be inherited by those two
    */
    void ConfirmGreenSquares(); /* change color of all green squares to red and
                                color of neighbouring (including diagonal) squares to grey */

    void FindBlankNeighbours(QString , /*QString*/ color_t ); /* pushes back coordinates of white square which need to be Greyed
                                                     or erases coordinates of grey squares which need to be Whitened*/

    std::pair<char, int> SplitName(QString ); /* returns pair "column-row" from given Square name */

    QString AssembleName(std::pair<char, int> ); /* returns a Square name from given pair column-row" */

    bool NeighbouringSquares(QString , QString, bool); /* checks if two given squares are on the same vertical/horizontal
                                           and are neighbours (if third argument is set to TRUE than check diagonals too) */

    void SetBattleshipCoords();

    void SetCruisersCoords();
    void SetCruisersCoords(std::vector<QString> );

    void SetDestroyersCoords();
    void SetDestroyersCoords(std::vector<QString> );

    void SetTorpedoBoatsCoords();

    void ClearWhatever(QString );

    void CheckAllShipsPlaced();

    /* compares Remainder of CoordinatesOfGreenSquares and not-yet-red ship */
    bool ShipsCollide(std::vector<QString> );

public slots:

    void ShowGridSlot();

    void on_FleeButton_clicked();

    void RandomButtonEnableSlot(bool );

    void hideFillGridWindowSlot();

private slots:

    void on_BattleshipButton_clicked();

    void on_ClearBattleshipButton_clicked();

    void on_CruisersButton_clicked();

    void on_ClearCruisersButton_clicked();

    void on_ClearAllButton_clicked();

    void on_DestroyersButton_clicked();

    void on_ClearDestroyersButton_clicked();

    void on_TorpedoBoatsButton_clicked();

    void on_ClearTorpedoBoatsButton_clicked();

    void on_JoinButton_clicked();

    void on_RandomButton_clicked();

private:

    Ui::FillGridWindow *ui;

    int NumberOfGreenSquares;
    std::vector <QString> CoordinatesOfGreenSquares;
    /*std::vector <QString> CoordinatesOfGreySquares;*/

    /*std::vector <QString>::iterator FindInGrey(QString );*/

    int CheckGreenNeighbours(QString , std::vector <std::pair<char, int>> & );

    short FillShipLoop(std::pair<char, int> , int , const short , const bool );

    /* returns part of Green Squares, which form a ship and removes this part from CoordinatesOfGreenSquares */
    std::vector<QString> RemoveActualShipFromPendingPart(std::vector<std::pair<char, int>> );

    void SetShipCoords(QString );
    void SetShipCoords(QString , std::vector<QString> );

    short AsIf_BattleshipButton_clicked();
    short AsIf_CruisersButton_clicked();
    short AsIf_DestroyersButton_clicked();
    short AsIf_TopedoBoatsButton_clicked();

    void closeEvent(QCloseEvent * event);

    bool RandomButtonClicked;
    std::mutex rand_mu;

    void ErrorMess(QString );

    std::thread RandomizingThread;
    void RandomButtonThread();

    void SetRandomButtonClicked(bool );
    bool GetRandomButtonClicked();

    std::mutex intrptd_mu;
    bool Interrupted;
    bool WasInterrupted();
    void SetInterrupted(bool );
    void SetInterruptedTrue();

signals:

    void SendBbyeSig();
    void CloseConnAndJoinThreadSig();
    void SendFillSig();
    void ShowMainWindowSig();

    void ShowWaitingWindowSig();
    void HideWaitingWindowSig();

    void CloseWaitingWindowSig();

};

#endif // FILLGRIDWINDOW_H
