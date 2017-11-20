#ifndef KNIGHTTOUR_H
#define KNIGHTTOUR_H

#include <QMainWindow>
#include <QDebug>
#include <QMessageBox>

/* for pick random numbers */
#include <stdlib.h>
#include <time.h>

/* we deal with standard chess board */
#define SIZE 8

namespace Ui {
class KnightTour;
}

class KnightTour : public QMainWindow
{
    Q_OBJECT

public:
    explicit KnightTour(QWidget *parent = 0);
    ~KnightTour();

    /* matrix which shows which squares knight did visit
     * and which did not */
    int board[SIZE][SIZE];

    int accessability[SIZE][SIZE];
        /* as if you could get in any square from 8 others imagine limitless board */

    /* Current knight's coordinates */
    int x;
    int y;

    /*Number of moves performed by a knight */
    int n;

    /* Decides what color should be each square
     * a board depending on order and which squares
     * were already visited by knight */
    int printBoard(int );

    /* Prints accessability matrix */
    int printAccessability();

    /* Checks if we go out of board */
    int OutOfBoard(int );

    /* Accessability of the square where knight currently
    stands or where he has already been is zero.
    Accessability of all squares where he can go from current
    should be decreased by 1 - he wouldn't get back to the current square */
    void DecreaseAccessabilitySurrounding();

    /* Refresh accessability matrix minding initial coordinates of the knight */
    void refreshAccessability();

    /* Takes value 1 if knight can make a move,
     * 0 - if this is impossible
     * (out of board or he has visited squares where he could move from current one) */
    int canMakeFurtherMove;

    /* Variable determining a way knight makes his decisions */
    int Method; /* depending on chosen radio button */

    /* Wrapper for all moveKnightWhatever() functions */
    int moveKnight(int * );

    /* Randomly chooses next move of knight
     * only checking if the move is allowed */
    int moveKnightRandom(int * ); /* returns 0 if knight didn't make a move */

    /* Accessability heuristics approach */
    int moveKnightAccessability(int * );

    /* Backtracking approach */
    int moveKnightBacktrack(int * );

    /* for needs of breaking Backtrack loop */
    int RefreshButtonClicked;

private slots:
    void on_MakeAMoveButton_clicked();

    void on_SetCoordButton_clicked();

    void on_ShowAccessability_stateChanged();

    void on_radioRandom_clicked();

    void on_radioAccessab_clicked();

    void on_radioBacktrack_clicked();

    void on_CloseButon_clicked();

private:
    Ui::KnightTour *ui;

    void SetCoordinates();

    /* Arrays of all possible moves - same subscript for two arrays
        means coordinates of one move */

    static int x_temp[8];
    static int y_temp[8];


    /* First two arguments are coordinates, third is
     * 1 if the knight is on the square and 0 otherwise,
     * fourth is 1 if we refresh the board and 0 otherwise */
    void changeColorKnight(int , int , int, int );

    /* Second argument is the line with color */
    void changeColorKnightZero(int , QString );
    void changeColorKnightOne(int , QString );
    void changeColorKnightTwo(int , QString );
    void changeColorKnightThree(int , QString );
    void changeColorKnightFour(int , QString );
    void changeColorKnightFive(int , QString );
    void changeColorKnightSix(int , QString );
    void changeColorKnightSeven(int , QString );

    /* Functions performing each possible move */
    int moveUpLeft();
    int moveLeftUp();
    int moveUpRight();
    int moveRightUp();
    int moveDownLeft();
    int moveLeftDown();
    int moveDownRight();
    int moveRightDown();

    /* Print/hide accessability value right on the square */
    void PrintAccessSquare(int, int, int);

    void PrintAccessSquareZero(int ,int );
    void PrintAccessSquareOne(int ,int );
    void PrintAccessSquareTwo(int ,int);
    void PrintAccessSquareThree(int ,int );
    void PrintAccessSquareFour(int ,int );
    void PrintAccessSquareFive(int ,int );
    void PrintAccessSquareSix(int ,int );
    void PrintAccessSquareSeven(int ,int );

};

#endif // KNIGHTTOUR_H
