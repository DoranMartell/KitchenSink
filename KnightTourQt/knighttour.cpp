#include "knighttour.h"
#include "ui_knighttour.h"

int KnightTour::x_temp[8] = {-2, -2, -1, -1, 1, 1, 2, 2};
int KnightTour::y_temp[8] = {-1, 1, -2, 2, -2, 2, -1, 1};

KnightTour::KnightTour(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::KnightTour)
{
    ui->setupUi(this);

    RefreshButtonClicked = 0;

    /* Read initial X and Y */
    SetCoordinates();

    /*Initializing number of moves performed by a knight */
    n = 0;

    /* Print a board with knight in inital position */
    printBoard(n);

    /* Initializes accessability matrix */
    refreshAccessability();

    /* Obviously, in the start knight didn't make any move yet,
        hence there are possible moves he can pick */
    canMakeFurtherMove = 1;

    /* Set IDs for each of radio buttons */
    ui->MethodButtons->setId(ui->radioRandom, 1);
    ui->MethodButtons->setId(ui->radioAccessab, 2);
    ui->MethodButtons->setId(ui->radioBacktrack, 3);

    /* Depending on which Radio Button is switched
     * we pick a method of traversal */
    Method = ui->MethodButtons->checkedId();

}

void KnightTour::on_SetCoordButton_clicked()
{
    RefreshButtonClicked = 1;

    SetCoordinates();
    /* Once this button clicked we start from scratch */
    /* Refreshing all moves made by knight so far*/
    n = 0;
    printBoard(n);
    refreshAccessability();
    printAccessability();
    canMakeFurtherMove = 1;

    /* Depending on which Radio Button is switched
     * we pick a method of traversal */
    Method = ui->MethodButtons->checkedId();

    if (Method == 3)
    {
        ui->ShowAccessability->setChecked(false);
        ui->ShowAccessability->setEnabled(false);
    }
    else
        ui->ShowAccessability->setEnabled(true);

    ui->statusBar->showMessage("The chessboard is refreshed", 5000);

}

void KnightTour::SetCoordinates()
{
    /* Refreshes a board */
    for(int i = 0; i < SIZE; i++)
    {
        for(int j = 0 ; j < SIZE; j++)
        {
            board[i][j] = 0;
        }
    }

    /* Knight's initial coordinates */
    x = ui->XLine->text().toInt();
    y = ui->YLine->text().toInt();

    if (x < 0 || x > SIZE-1 || y < 0 || y > SIZE-1)
    {
            QMessageBox::critical(this, "Invalid Initial Coordinates",
              "Initial coordinates you entered are invalid. Default values were used.");

        x = 3;
        y = 3;

        ui->XLine->setText("3");
        ui->YLine->setText("3");
    }

    /* Knight is in this point now */
    board[y][x] = 1;
}

KnightTour::~KnightTour()
{
    delete ui;
}

/*-------------------------------------------*/
/* Returns 1 if there is at least one square */
/* where the knight has not been yet		 */
/*-------------------------------------------*/
int KnightTour::printBoard(int step)
{
    int i, j;
    int res = 0;

//    QDebug dbg(QtDebugMsg);

//    qDebug() << "-----------------------";
//    qDebug() <<"Board after %d move(s)" << step;
//    qDebug() << "-----------------------";
//    dbg << "";
    for (i = 0; i < SIZE; ++i)
    {
        for (j = 0; j < SIZE; ++j)
        {
            if (x == j && y == i)
            {
//                dbg << "K";
            }
            else if (board[i][j] == 0)
            {

//                dbg << "0";
                /* Change color of square only if we refresh board */
                //if (!step)
                    /*not with backtracking as it requires
                        to return to previously visited squares */
                //{
                    changeColorKnight(j, i, 0, 1);
                //}
                res = 1;
            }
            else
            {
//                dbg << "1";
                changeColorKnight(j, i, 0, 0);
            }
        }
//        dbg << "\n";
    }

    changeColorKnight(x, y, 1, 0);

//    qDebug() << "-----------------------";
    return res;
}


int KnightTour::moveKnight(int * step)
{
    int res = 0;

    if (Method == 1)
    {
        res = moveKnightRandom(step);
    }
    else if (Method == 2)
    {
        res = moveKnightAccessability(step);
    }
    else if (Method == 3)
    {
        ui->MakeAMoveButton->setEnabled(false);
        res = moveKnightBacktrack(step);
        ui->MakeAMoveButton->setEnabled(true);

    }

    return res;

}

void KnightTour::on_MakeAMoveButton_clicked()
{

        srand(time(NULL));
        int result = 0;
        QString MessToBar;

        RefreshButtonClicked = 0;

        if (!canMakeFurtherMove )
        {
            if (n != 63)
            {
                QMessageBox::critical(this, "Stuck",
                    "There are no more squares where knight can go from there. Please, refresh the board and start again.");

                MessToBar = "I'm stuck after " + QString::number(n) + " moves! Let's try again from the start";
                ui->statusBar->showMessage(MessToBar, 0);
                return;
            }
            else
            {
                QMessageBox::information(this, "Tour Completed",
                    "Knight visited all squares. Please, refresh the board if you want to start again.");

                MessToBar = "Knight completed his tour";
                ui->statusBar->showMessage(MessToBar, 0);
                return;
            }
        }

        /* Otherwise there would be end-of-line
         * after each output to qDebug() */
        QDebug dbg(QtDebugMsg);
        dbg << "";


        // do
        // {
                result = moveKnight(&n); /* returns 1 if knight made a move */
        // } while ( printBoard(n) && result);

        printBoard(n);
        printAccessability();

        if (result)
        {
            MessToBar = "Knight made " + QString::number(n) + " moves so far";
            ui->statusBar->showMessage(MessToBar, 5000);
            MessToBar="";
            dbg << "Total moves: %d\n" << n;
        }
}


void KnightTour::on_ShowAccessability_stateChanged()
{
    printAccessability();
}

void KnightTour::on_radioRandom_clicked()
{
    ui->statusBar->showMessage("Knight will randomly choose available square after board refresh", 5000);
    QMessageBox::information(this, "Another Method Selected",
      "Chosen method will be applied to knight's decision making strategy after you click \"Refresh\".");
}

void KnightTour::on_radioAccessab_clicked()
{
    ui->statusBar->showMessage("Knight will rely on squares accessability after board refresh", 5000);
    QMessageBox::information(this, "Another Method Selected",
      "Chosen method will be applied to knight's decision making strategy after you click \"Refresh\".");
}

void KnightTour::on_radioBacktrack_clicked()
{
    ui->statusBar->showMessage("Backtracking method will be used after board refresh", 5000);
    QMessageBox::information(this, "Another Method Selected",
      "Chosen method will be applied to knight's decision making strategy after you click \"Refresh\".");

}

void KnightTour::on_CloseButon_clicked()
{
    RefreshButtonClicked = 1;
    QApplication::exit();
}
