#include "fillgridwindow.h"
#include "ui_fillgridwindow.h"

/*----------------------------------------------------------------------------------*/
FillGridWindow::FillGridWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FillGridWindow)
{
    ui->setupUi(this);

    setWindowTitle("Place your ships on the grid");

    ui->JoinButton->setEnabled(false);

    /* Associate each square on grid with its name in QString format
        for convinience, as all incoming and outgoing messages will have Coordinates in QString format,
        thus we will simply find certain square which need to change the color */
    LinkSquareMap();

    qDebug() << "Fill Grid Window thread started with ID" << GetQStrThreadId(std::this_thread::get_id());

    SetRandomButtonClicked(false);

    SetInterrupted(false);

}
/*----------------------------------------------------------------------------------*/
void FillGridWindow::ShowGridSlot()
{
    qDebug() << "Entered slot to show grid window";
    /* in case we get in here after Victory, need to clear previous fleet */
    on_ClearAllButton_clicked();
    show();
}
/*----------------------------------------------------------------------------------*/
void FillGridWindow::GetCoordinatesOfGreenSquares()
{
    /* if we are here, new green squares were chosen */
    //CoordinatesOfGreenSquares.clear();
    //CoordinatesOfGreySquares.clear(); /* was commented previously. Why??? Because it is used during ClearWhatever() */
    NumberOfGreenSquares = 0;

    std::map<QString, QGridSquare *>::iterator it;

    for (it = Square.begin(); it != Square.end(); it++)
    {
        if (it->second->getColor() == green)
        {
            CoordinatesOfGreenSquares.push_back(it->first);
            qDebug() << "Green Square was found in" << CoordinatesOfGreenSquares.back() << "square!";
            NumberOfGreenSquares++;
            qDebug() << "There are" << NumberOfGreenSquares << "of Green Squares now";
        }
    }

}
/*----------------------------------------------------------------------------------*/
void FillGridWindow::PrintCurrentGreenSquaresVector()
{
    std::vector <QString> :: iterator it;
    QDebug dbg(QtDebugMsg);
    dbg << "Current CoordinatesOfGreenSquares vector: ";

    for (it = CoordinatesOfGreenSquares.begin(); it != CoordinatesOfGreenSquares.end(); it++)
    {
        dbg << (*it) << " ";
    }
    dbg << "\n";
}
/*----------------------------------------------------------------------------------*/
/*void FillGridWindow::PrintCurrentGreySquaresVector()
{
    std::vector <QString> :: iterator it;
    QDebug dbg(QtDebugMsg);
    dbg << "Current CoordinatesOfGreySquares vector: ";

    for (it = CoordinatesOfGreySquares.begin(); it != CoordinatesOfGreySquares.end(); it++)
    {
        dbg << (*it) << " ";
    }
    dbg << "\n";
}*/

/*----------------------------------------------------------------------------------*/
void FillGridWindow::ClearCoordinatesOfGreenSquares()
{
    std::vector<QString>::iterator it;

    for (it = CoordinatesOfGreenSquares.begin(); it != CoordinatesOfGreenSquares.end(); it++)
    {
        Square.find((*it))->second->changeColor(white);
    }

    qDebug() << "All pending green squares were cleared";
}
/*----------------------------------------------------------------------------------*/
void FillGridWindow::SortCoordinatesOfGreenSquares()
{
    std::vector<QString>::iterator it;
    std::vector<QString>::iterator jit;

    QDebug dbg(QtDebugMsg);

    if (!CoordinatesOfGreenSquares.empty())
    {
        for (it = CoordinatesOfGreenSquares.begin(); it != (CoordinatesOfGreenSquares.end() - 1); it++)
        {
            if (it->right(2) == "10")
            {
                dbg << "Rename" << (*it) << "to:";
                it->replace(1, 2, ":");
                dbg << (*it) << "for sorting purposes only\n";
            }
            for ( jit = it + 1; jit != CoordinatesOfGreenSquares.end(); jit++)
            {
               if ((*it) > (*jit))
               {
                   QString swap = *it;
                   *it = *jit;
                   *jit = swap;
               }
            }
        }

        dbg << "\nSorted Coordinates of Green Squares: ";
        for (it = CoordinatesOfGreenSquares.begin(); it != CoordinatesOfGreenSquares.end(); it++)
        {
            if (it->right(1) == ":")
            {
                it->replace(1, 2, "10");
            }

            dbg << (*it);
        }
    }
    else
    {
        qDebug() << "Apparently, there are no Green Squares chosen";
    }
}
/*----------------------------------------------------------------------------------*/
/*
  Create a parent class for both FillGridWindow and Client window
  and implement there virtual ConfirmGreenSquares which would be inherited by those two
*/
void FillGridWindow::ConfirmGreenSquares()
{
    qDebug() << "About to turn pending Green Squares to confirmed Red Squares (ships)";

    std::vector<QString>::iterator it;

    for (it = CoordinatesOfGreenSquares.begin(); it != CoordinatesOfGreenSquares.end(); it++)
    {
        if (Square.find(*it)->second->getColor() == green)
        {
            FindBlankNeighbours(*it, grey);
            //qDebug() << "Paint square" << (*it) << "into RED";
            Square.find(*it)->second->changeColor(red);
        }
    }

    qDebug() << "About to turn all neighbours of Green Squares to Grey (where it's impossible to place a ship)";

    /*for (it = CoordinatesOfGreySquares.begin(); it != CoordinatesOfGreySquares.end(); it++)
    {
        Square.find(*it)->second->changeColor("grey");
    }

    PrintCurrentGreySquaresVector();*/

    /* As all chosen squares changed their color, no need to remember
            formerly green squares, make some space for new ones */
    CoordinatesOfGreenSquares.clear();
}
/*----------------------------------------------------------------------------------*/
void FillGridWindow::FindBlankNeighbours(QString square, color_t color)
{
    std::pair<char, int> coord;
    coord = SplitName(square);

    QString NeighbourName;

    std::vector<QString>::iterator it;

    /* eight blank neigbours maximum */
    for (int row_how = -1; row_how <= 1; row_how++)
    {
        for (int col_how = -1; col_how <= 1; col_how++)
        {
            if (!(coord.first + col_how < 'A') && !(coord.first + col_how > 'J'))
            {
                if (!(coord.second + row_how < 1) && !(coord.second + row_how > 10))
                {
                    NeighbourName = QString(coord.first + col_how) + QString::number(coord.second + row_how);

                    QGridSquare * SquarePtr = Square.find(NeighbourName)->second;

                    if (color == grey)
                    {
                        /* we don't need to turn Green and Red squares to Grey, as it is part of the ship */
                        if (SquarePtr->getColor() != green &&
                            SquarePtr->getColor() != red)
                        {
                            /* no need to store duplicates */
                            /*CoordinatesOfGreySquares.erase(std::remove(CoordinatesOfGreySquares.begin(),
                                                     CoordinatesOfGreySquares.end(), NeighbourName), CoordinatesOfGreySquares.end());

                            CoordinatesOfGreySquares.push_back(NeighbourName);*/

                            SquarePtr->changeColor(grey);
                        }
                    }
                    else if (color == white)
                    {
                        if (SquarePtr->getColor() == grey)
                        {
                            SquarePtr->changeColor(white);

                            if (SquarePtr->getOverlap() == 0)
                            {
                                /* if the square is not grey anymore, remove it from respective vector */
                                /*it = FindInGrey(NeighbourName);
                                CoordinatesOfGreySquares.erase(it);*/

                                /*
                                  CoordinatesOfGreySquares.erase(std::remove(CoordinatesOfGreySquares.begin(),
                                                         CoordinatesOfGreySquares.end(), NeighbourName), CoordinatesOfGreySquares.end());
                                */
                            }
                        }
                    }
                }
            }
        }
    }
}
/*----------------------------------------------------------------------------------*/
std::pair<char, int> FillGridWindow::SplitName(QString name)
{
    std::pair<char, int> Coord;

    char Column = name.at(0).toLatin1();
    int Row = name.mid(1).toInt();

    Coord = std::make_pair(Column, Row);
    return Coord;
}
/*----------------------------------------------------------------------------------*/
QString FillGridWindow::AssembleName(std::pair<char, int>  coords)
{
    QString name;

    name = QString(coords.first) + QString::number(coords.second);
    return name;
}
/*----------------------------------------------------------------------------------*/
bool FillGridWindow::NeighbouringSquares(QString square1, QString square2, bool diagonals_included)
{
    std::pair<char, int> coord1;
    std::pair<char, int> coord2;

    coord1 = SplitName(square1);
    coord2 = SplitName(square2);

    qDebug() << "Are squares" << square1 << "and" << square2 << "neighbours?";

    if (!diagonals_included)
        qDebug() << "Only horizontals and verticals are going to be checked now";

    /*
    if (coord1.first == coord2.first)
    {
        qDebug() << "Squares" << square1 << "and" << square2 << "are in the same column";
        if (coord1.second == coord2.second + 1 || coord1.second == coord2.second - 1)
        {
            qDebug() << "Squares" << square1 << "and" << square2 << "are vertical neighbours";
            return true;
        }
        else
        {
            qDebug() << "Squares" << square1 << "and" << square2 << "are NOT neighbours";
            return false;
        }
    }
    else if (coord1.second == coord2.second)
    {
        qDebug() << "Squares" << square1 << "and" << square2 << "are in the same row";
        if (coord1.first == coord2.first + 1 || coord1.first == coord2.first - 1)
        {
            qDebug() << "Squares" << square1 << "and" << square2 << "are horizontal neighbours";
            return true;
        }
        else
        {
            qDebug() << "Squares" << square1 << "and" << square2 << "are NOT neighbours";
            return false;
        }
    }
    else
    {
        qDebug() << "Squares" << square1 << "and" << square2 << "are NOT neighbours";
        return false;
    }*/


    for (int row_how = -1; row_how <= 1; row_how++)
    {
        for (int col_how = -1; col_how <= 1; col_how++)
        {
            if (row_how == 0 && col_how == 0)
            {
                /* We are not going to check if square1 is square2 */
                continue;
            }

            if (!diagonals_included)
            {
                /* If square1 and square2 are compared only as horizontal/vertical neighbours,
                    one of row_how or col_how should be zero (squares should be on the same row/column to be neighbours) */
                if (!(row_how == 0 || col_how == 0))
                        continue;
            }

            if (coord1.first == coord2.first + col_how && coord1.second == coord2.second + row_how)
            {
                qDebug() << "Squares" << square1 << "and" << square2 << "are neighbours";
                return true;
            }
        }
    }

    qDebug() << "Squares" << square1 << "and" << square2 << "are definetely NOT neighbours";
    return false;

}

/*----------------------------------------------------------------------------------*/
void FillGridWindow::on_BattleshipButton_clicked()
{
   AsIf_BattleshipButton_clicked();
}
/*----------------------------------------------------------------------------------*/
void FillGridWindow::on_CruisersButton_clicked()
{
    AsIf_CruisersButton_clicked();
}
/*----------------------------------------------------------------------------------*/
void FillGridWindow::on_DestroyersButton_clicked()
{
    AsIf_DestroyersButton_clicked();
}
/*----------------------------------------------------------------------------------*/
void FillGridWindow::on_TorpedoBoatsButton_clicked()
{
    AsIf_TopedoBoatsButton_clicked();
}
/*----------------------------------------------------------------------------------*/
short FillGridWindow::AsIf_BattleshipButton_clicked()
{
    ui->BattleshipButton->setEnabled(false);

    GetCoordinatesOfGreenSquares();

    SortCoordinatesOfGreenSquares();

    std::vector<QString>::iterator it;

    /*bool neighbours = true;*/

    if (CoordinatesOfGreenSquares.empty())
    {
        ErrorMess("You didn't choose any squares (press on a square, it should change its color to green, green square is a chosen square)");
        ui->BattleshipButton->setEnabled(true);
        return -1;
    }

    /*for (it = CoordinatesOfGreenSquares.begin(); it != (CoordinatesOfGreenSquares.end() - 1); it++)
    {
        if (!NeighbouringSquares(*it, *(it + 1), false))
        {
            neighbours = false;
            break;
        }
    }*/

    if (NumberOfGreenSquares == 4) /* this check was absent while previous code was in power */
    {
        std::vector <std::pair<char, int>> GreenNeighboursCoords;
        int MiddleSquaresHaveTwoNeighbours = 0;
        for (it = CoordinatesOfGreenSquares.begin(); it != (CoordinatesOfGreenSquares.end() - 1); it++)
        {
            /* MiddleSquare of suppossed Battleship should have TWO neighbours (left and right or above and below) */
            if (CheckGreenNeighbours(*it ,GreenNeighboursCoords) == 2)
            {
                MiddleSquaresHaveTwoNeighbours++;
                qDebug() << "Number of middlesquares which have two neighbours in row or column was increased to"
                                << MiddleSquaresHaveTwoNeighbours;
            }
        }

        /* Battleship should have two MiddleSquares who have two green neighbours each */
        if (MiddleSquaresHaveTwoNeighbours != 2)
        /*if (!neighbours)*/
        {
            ErrorMess("Battleship should take 4 neighbouring squares vertically or horizontally. Please, place your Battleship once again.");
            ClearCoordinatesOfGreenSquares();
        }
        else /* apparently, the ship was placed correctly. Freeze it and neighbouring squares too! */
        {
            SetBattleshipCoords(); /* coordinates of Green Squares copied to global array BattleShipCoords */
            ConfirmGreenSquares();
            ui->BattleshipAmnt->setText("0");

            CheckAllShipsPlaced();

            return 0;
        }
    }
    else if (NumberOfGreenSquares > 4)
    {
        ErrorMess("You filled more than 4 squares! Please, place your Battleship once again.");
        ClearCoordinatesOfGreenSquares();
    }
    else if (NumberOfGreenSquares < 4)
    {
        ErrorMess("One or more squares of Battleship were not filled. Please, proceed.");
    }
    /* previously had 'else'-section here, now moved to 'if (MiddleSquaresHaveTwoNeighbours != 2)'*/

    ui->BattleshipButton->setEnabled(true);
    return -1;
}
/*----------------------------------------------------------------------------------*/
short FillGridWindow::AsIf_CruisersButton_clicked()
{
    ui->CruisersButton->setEnabled(false);

    GetCoordinatesOfGreenSquares();

    SortCoordinatesOfGreenSquares();

    /*bool neighbours;*/
    std::vector<QString>::iterator it;

    if (CoordinatesOfGreenSquares.empty())
    {
        ErrorMess("You didn't choose any squares (press on a square, it should change its color to green, green square is a chosen square)");
        ui->CruisersButton->setEnabled(true);
        return -1;
    }

    if (NumberOfGreenSquares == 3)
    {
        qDebug() << "Apparently, Player tried to place only one Cruiser now. Check if it was done correctly";

        qDebug() << "As it is only one ship, need to check if all chosen squares are in the same row or in the same column";

        /*neighbours = true;
        for (it = CoordinatesOfGreenSquares.begin(); it != (CoordinatesOfGreenSquares.end() - 1); it++)
        {
            if (!NeighbouringSquares(*it, *(it + 1), false))
            {
                neighbours = false;
                break;
            }
        }

        if (!neighbours)
        {
            ErrorMess("One cruiser should take 3 neighbouring squares vertically or horizontally. Please, place your Cruisers once again.");
            ClearCoordinatesOfGreenSquares();
        }*/

        std::vector <std::pair<char, int>> GreenNeighboursCoords;

        for (it = CoordinatesOfGreenSquares.begin(); it != CoordinatesOfGreenSquares.end(); it++)
        {
            if (CheckGreenNeighbours(*it, GreenNeighboursCoords) == 2)
            {
                qDebug() << "We found smth looking like a Cruiser!";

                ////GreenNeighboursCoords.clear();

                SetCruisersCoords();

                ConfirmGreenSquares();

                int num = ui->CruiserAmnt->text().toInt();
                ui->CruiserAmnt->setText(QString::number(--num));

                if (ui->CruiserAmnt->text().toInt() == 0)
                {
                    qDebug() << "All Cruisers were placed now";
                    ui->CruisersButton->setEnabled(false);
                    CheckAllShipsPlaced();

                    return 0;
                }

                ui->CruisersButton->setEnabled(true);
                return 0;
            }
        }

        ErrorMess("One cruiser should take 3 neighbouring squares vertically or horizontally. Please, place your Cruisers once again.");
        ClearCoordinatesOfGreenSquares();
    }
    else if (NumberOfGreenSquares == 6)
    {
        if (ui->CruiserAmnt->text().toInt() == 2)
        {
            qDebug() << "Apparently, Player tried to place both Cruisers at once";

            std::vector <QString> OriginalGreenSquares = CoordinatesOfGreenSquares;

            std::vector <QString>::iterator it;

            std::vector <std::pair<char, int>> GreenNeighboursCoords;

            std::vector<QString> FirstCruiser;
            std::vector<QString> SecondCruiser;

            for (it = OriginalGreenSquares.begin(); it != OriginalGreenSquares.end(); it++)
            {
                if (CheckGreenNeighbours(*it, GreenNeighboursCoords) == 2)
                {
                    qDebug() << "We found smth looking like a Cruiser!";

                    FirstCruiser = RemoveActualShipFromPendingPart(GreenNeighboursCoords);

                    break;
                }
            }

            if (FirstCruiser.empty())
            {
                qDebug() << "Not even one Cruiser was obtained from given coordinates";
            }
            else
            {
                //GreenNeighboursCoords.clear();
                PrintCurrentGreenSquaresVector();

                for (it = CoordinatesOfGreenSquares.begin(); it != CoordinatesOfGreenSquares.end(); it++)
                {
                    if (CheckGreenNeighbours(*it, GreenNeighboursCoords) == 2)
                    {
                        qDebug() << "We found smth looking like a SECOND Cruiser!";

                        /* Now we need to check if Cruisers DO NOT TOUCH EACH OTHER! */
                        /* if EVEN ONE square of already-set Cruiser Coordinates is neighbour with one
                                of Remainder of original GreenNeighboursCoords, then we should report ERROR! */

                        bool collide = ShipsCollide(FirstCruiser);

                        //CoordinatesOfGreenSquares = OriginalGreenSquares;

                        /* now we're setting coordinates of both Cruisers */
                        CruisersCoords.clear();

                        if (collide)
                        {
                            ErrorMess("Ships are not supposed to touch each other! Please, place Cruisers once again, so they would have at least one square between them");
                            ClearCoordinatesOfGreenSquares();
                            goto end_of_CruisersButtonClicked;
                        }


                        //SetCruisersCoords();
                        SecondCruiser = CoordinatesOfGreenSquares;
                        SetCruisersCoords(FirstCruiser);
                        SetCruisersCoords(SecondCruiser);
                        CoordinatesOfGreenSquares = OriginalGreenSquares;

                        ConfirmGreenSquares();

                        qDebug() << "Both Cruisers were set!";
                        ui->CruiserAmnt->setText("0");
                        ui->CruisersButton->setEnabled(false);
                        CheckAllShipsPlaced();

                        return 0;
                    }

                }
            }

            CoordinatesOfGreenSquares = OriginalGreenSquares;
            ErrorMess("The squares you've chosen do not form two Cruisers. Please, place Cruisers once again");
            ClearCoordinatesOfGreenSquares();
        }
        else
        {
            ErrorMess("Apparently you tried to place two more Cruisers, but it is possible to place only one");
            ClearCoordinatesOfGreenSquares();
        }

    }
    else if (NumberOfGreenSquares < 3)
    {
        ErrorMess("The squares you've chosen are less than needed for even one Cruiser. Please, place Cruisers again");
        ClearCoordinatesOfGreenSquares();
    }
    else if (NumberOfGreenSquares < 6 && NumberOfGreenSquares > 3)
    {
        ErrorMess("If you want to place 2 Cruisers at once, you need to fill 6 squares totally, 3 for each Cruiser");
        ClearCoordinatesOfGreenSquares();
    }
    else if (NumberOfGreenSquares > 6)
    {
        ErrorMess("You've chosen more squares than it is required for 2 Cruisers. Each Cruiser is 3 squares long");
        ClearCoordinatesOfGreenSquares();
    }

    end_of_CruisersButtonClicked:
    ui->CruisersButton->setEnabled(true);
    return -1;
}
/*----------------------------------------------------------------------------------*/
short FillGridWindow::AsIf_DestroyersButton_clicked()
{
    ui->DestroyersButton->setEnabled(false);

    GetCoordinatesOfGreenSquares();

    SortCoordinatesOfGreenSquares();

    std::vector<QString>::iterator it;

    std::vector <std::pair<char, int>> GreenNeighboursCoords;
    std::vector<QString> FirstDestroyer;
    std::vector<QString> SecondDestroyer;
    std::vector<QString> ThirdDestroyer;

    if (CoordinatesOfGreenSquares.empty())
    {
        ErrorMess("You didn't choose any squares (press on a square, it should change its color to green, green square is a chosen square)");
        goto end_of_DestroyersButtonClicked;
    }

    if (NumberOfGreenSquares == 2)
    {
        qDebug() << "Apparently, Player tried to place only one Destroyer now. Check if it was done correctly";
        qDebug() << "As it is only one ship, need to check if all chosen squares are in the same row or in the same column";


        for (it = CoordinatesOfGreenSquares.begin(); it != CoordinatesOfGreenSquares.end(); it++)
        {
            if (CheckGreenNeighbours(*it, GreenNeighboursCoords) == 1)
            {
                qDebug() << "We found smth looking like a Destroyer!";

                //GreenNeighboursCoords.clear();

                SetDestroyersCoords();

                ConfirmGreenSquares();

                int num = ui->DestroyerAmnt->text().toInt();
                ui->DestroyerAmnt->setText(QString::number(--num));

                if (ui->DestroyerAmnt->text().toInt() == 0)
                {
                    qDebug() << "All Destroyers were placed now";
                    CheckAllShipsPlaced();
                    return 0;
                }

                goto end_of_DestroyersButtonClicked;
            }
        }

        ErrorMess("One destroyer should take 2 neighbouring squares vertically or horizontally. Please, place your Destroyers once again.");
        ClearCoordinatesOfGreenSquares();
    }
    else if (NumberOfGreenSquares == 4)
    {
        if (ui->DestroyerAmnt->text().toInt() >= 2)
        {
            qDebug() << "Apparently, Player tried to place two Destroyers";

            std::vector <QString> OriginalGreenSquares = CoordinatesOfGreenSquares;

            for (it = OriginalGreenSquares.begin(); it != OriginalGreenSquares.end(); it++)
            {
                if (CheckGreenNeighbours(*it, GreenNeighboursCoords) == 1)
                {
                    qDebug() << "We found smth looking like a Destroyer!";

                    FirstDestroyer = RemoveActualShipFromPendingPart(GreenNeighboursCoords);

                    break;
                }
            }

            if (FirstDestroyer.empty())
            {
                qDebug() << "Not even one Destroyer was obtained from given coordinates";
            }
            else
            {
                //GreenNeighboursCoords.clear();
                PrintCurrentGreenSquaresVector();

                for (it = CoordinatesOfGreenSquares.begin(); it != CoordinatesOfGreenSquares.end(); it++)
                {
                    if (CheckGreenNeighbours(*it, GreenNeighboursCoords) == 1)
                    {
                        qDebug() << "We found smth looking like a SECOND Destroyer!";

                        /* for case when we fill all three destroyers, we need to cross-check collission of them all */
                        bool collide = ShipsCollide(FirstDestroyer);

                        //CoordinatesOfGreenSquares = OriginalGreenSquares;

                        if (collide)
                        {
                            ErrorMess("Ships are not supposed to touch each other! Please, place Destroyers once again, so they would have at least one square between them");
                            ClearCoordinatesOfGreenSquares();
                            goto end_of_DestroyersButtonClicked;
                        }

                        /* Should have we cleared coords before refilling them?? */
                        //SetDestroyersCoords();

                        SecondDestroyer = CoordinatesOfGreenSquares;
                        SetDestroyersCoords(FirstDestroyer);
                        SetDestroyersCoords(SecondDestroyer);
                        CoordinatesOfGreenSquares = OriginalGreenSquares;

                        ConfirmGreenSquares();

                        qDebug() << "Two Destroyers were set!";
                        QString currDestroyerAmnt = QString::number(ui->DestroyerAmnt->text().toInt() - 2);
                        qDebug() << "Amount of Destroyers left to be placed on grid" << currDestroyerAmnt;
                        ui->DestroyerAmnt->setText(currDestroyerAmnt);

                        if (currDestroyerAmnt == "0")
                        {
                            CheckAllShipsPlaced();
                            return 0;
                        }

                        goto end_of_DestroyersButtonClicked;
                    }

                }
            }

            CoordinatesOfGreenSquares = OriginalGreenSquares;
            ErrorMess("The squares you've chosen do not form two Destroyers. Please, place Destroyers once again");
            ClearCoordinatesOfGreenSquares();
        }
        else
        {
            ErrorMess("Apparently you tried to place two more Destroyers, but it is possible to place only one");
            ClearCoordinatesOfGreenSquares();
        }
    }
    else if (NumberOfGreenSquares == 6)
    {
        if (ui->DestroyerAmnt->text().toInt() == 3)
        {
            qDebug() << "Apparently, Player tried to place all three Destroyers at once";

            std::vector <QString> OriginalGreenSquares = CoordinatesOfGreenSquares;

            for (it = OriginalGreenSquares.begin(); it != OriginalGreenSquares.end(); it++)
            {
                if (CheckGreenNeighbours(*it, GreenNeighboursCoords) == 1)
                {
                    qDebug() << "We found smth looking like a Destroyer!";

                    FirstDestroyer = RemoveActualShipFromPendingPart(GreenNeighboursCoords);

                    break;
                }
            }

            if (FirstDestroyer.empty())
            {
                qDebug() << "Not even one Destroyer was obtained from given coordinates";
            }
            else
            {
                //GreenNeighboursCoords.clear();
                PrintCurrentGreenSquaresVector();

                for (it = CoordinatesOfGreenSquares.begin(); it != CoordinatesOfGreenSquares.end(); it++)
                {
                    if (CheckGreenNeighbours(*it, GreenNeighboursCoords) == 1)
                    {
                        qDebug() << "We found smth looking like a SECOND Destroyer!";

                        /* check if second ship touches first one */
                        bool collide = ShipsCollide(FirstDestroyer);

                        if (collide)
                        {
                            ErrorMess("Ships are not supposed to touch each other! Please, place Destroyers once again, so they would have at least one square between them");
                            CoordinatesOfGreenSquares = OriginalGreenSquares;
                            ClearCoordinatesOfGreenSquares();
                            goto end_of_DestroyersButtonClicked;
                        }

                        SecondDestroyer = RemoveActualShipFromPendingPart(GreenNeighboursCoords);

                        break;
                    }
                }
            }
            if (SecondDestroyer.empty())
            {
                qDebug() << "Given coordinates do not form two Destroyers";
            }
            else
            {
                //GreenNeighboursCoords.clear();
                PrintCurrentGreenSquaresVector();

                for (it = CoordinatesOfGreenSquares.begin(); it != CoordinatesOfGreenSquares.end(); it++)
                {
                    if (CheckGreenNeighbours(*it, GreenNeighboursCoords) == 1)
                    {
                        qDebug() << "We found smth looking like a THIRD Destroyer!";

                        /* check if the third ship touches the first one... */
                        bool collide1 = ShipsCollide(FirstDestroyer);
                        /* ... or the second one */
                        bool collide2 = ShipsCollide(SecondDestroyer);


                        //CoordinatesOfGreenSquares = OriginalGreenSquares;

                        if (collide1 || collide2)
                        {
                            ErrorMess("Ships are not supposed to touch each other! Please, place Destroyers once again, so they would have at least one square between them");
                            ClearCoordinatesOfGreenSquares();
                            goto end_of_DestroyersButtonClicked;
                        }

                        //SetDestroyersCoords();

                        ThirdDestroyer = CoordinatesOfGreenSquares;
                        SetDestroyersCoords(FirstDestroyer);
                        SetDestroyersCoords(SecondDestroyer);
                        SetDestroyersCoords(ThirdDestroyer);
                        CoordinatesOfGreenSquares = OriginalGreenSquares;

                        ConfirmGreenSquares();

                        qDebug() << "All three Destroyers were set!";
                        ui->DestroyerAmnt->setText("0");
                        ui->DestroyersButton->setEnabled(false);
                        CheckAllShipsPlaced();

                        return 0;
                    }
                 }
             }
        }
        else
        {
            ErrorMess("Apparently you tried to place three more Destroyers, "
                                                "which is impossible, as you have already one or more Destroyers placed on the grid");
            ClearCoordinatesOfGreenSquares();
        }
    }
    else if (NumberOfGreenSquares < 2)
    {
        ErrorMess("Amount of currently chosen green squares is not enough to form even one Destroyer!"
                                            " One Destroyer take two squares, which are horizontal or vertical neighbours");
        ClearCoordinatesOfGreenSquares();
    }
    else if (NumberOfGreenSquares < 6)
    {
        ErrorMess("Chosen green squares do not form an integer number of Destroyers");
        ClearCoordinatesOfGreenSquares();
    }
    else if (NumberOfGreenSquares > 6)
    {
        ErrorMess("Too many squares chosen! You can place maximum 3 Destroyers, 2 squares each!");
        ClearCoordinatesOfGreenSquares();
    }

    end_of_DestroyersButtonClicked:
    ui->DestroyersButton->setEnabled(true);
    return -1;
}
/*----------------------------------------------------------------------------------*/
short FillGridWindow::AsIf_TopedoBoatsButton_clicked()
{

    ui->TorpedoBoatsButton->setEnabled(false);

    GetCoordinatesOfGreenSquares();
    SortCoordinatesOfGreenSquares();

    std::vector<QString>::iterator it;
    std::vector<QString>::iterator jit;

    if (CoordinatesOfGreenSquares.empty())
    {
        ErrorMess("You didn't choose any squares (press on a square, it should change its color to green, green square is a chosen square)");
        goto end_of_TorpedoBoatsButtonClicked;
    }

    /* if NumberOfGreenSquares is bigger than amount of remaining Torpedo Boats to be placed,
            then obviously we tried to place more boats than it is possible */
    if (ui->TorpedoBoatAmnt->text().toInt() - NumberOfGreenSquares >= 0)
    {
        for (it = CoordinatesOfGreenSquares.begin(); it != (CoordinatesOfGreenSquares.end() - 1); it++)
        {
            for (jit = it; jit != CoordinatesOfGreenSquares.end(); jit++)
            {
                if (NeighbouringSquares((*it), (*jit), true))
                {
                    ErrorMess("Torpedo Boats take 1 square each and Torpedo Boats cannot be placed on neighbouring squares! Please, place your Torpedo Boats once again");
                    ClearCoordinatesOfGreenSquares();
                    goto end_of_TorpedoBoatsButtonClicked;
                }
            }
        }

        SetTorpedoBoatsCoords();
        ConfirmGreenSquares();

        qDebug() << NumberOfGreenSquares << "Torpedo Boats were set!";
        ui->TorpedoBoatAmnt->setText(QString::number(ui->TorpedoBoatAmnt->text().toInt() - NumberOfGreenSquares));

        if (ui->TorpedoBoatAmnt->text() == "0")
        {
            CheckAllShipsPlaced();
            return 0;
        }

    }
    else
    {
        ErrorMess("You tried to place more Torpedo Boats than it is possible!");
    }

    end_of_TorpedoBoatsButtonClicked:
    ui->TorpedoBoatsButton->setEnabled(true);
    return -1;
}
/*----------------------------------------------------------------------------------*/
int FillGridWindow::CheckGreenNeighbours(QString square_name, std::vector <std::pair<char, int>> & NeighboursCoordsVector)
{
    int col_how;
    QString checksquare_name;
    int NumberOfNeighbours = 0;

    NeighboursCoordsVector.clear();


    qDebug() << "CheckGreenNeighbours(): checking if square" << square_name << "has Green Squares as vertical or horizontal neighbours";

    std::pair<char, int> coords;
    std::pair<char, int> checksquare_coords;
    coords = SplitName(square_name);
    NeighboursCoordsVector.push_back(coords); /* first element of vector is the square we're checking for green neighbours */


    for (int row_how = -1; row_how <= 1; row_how++)
    {
        if (row_how == 0)
            col_how = -1;
        else
            col_how = 0;

        while (col_how <= 1)
        {
            if (!(coords.first + col_how < 'A') && !(coords.first + col_how > 'J') &&
                  !(coords.second + row_how < 1) && !(coords.second + row_how > 10))
            {
                checksquare_name = QString(coords.first + col_how) + QString::number(coords.second + row_how);

                qDebug() << checksquare_name << "is a neighbour of" << square_name << "but is it green?";

                if (Square.find(checksquare_name)->second->getColor() == green)
                {
                    NumberOfNeighbours++;

                    checksquare_coords = SplitName(checksquare_name);
                    NeighboursCoordsVector.push_back(checksquare_coords);

                    qDebug() << "Neighbour square" << checksquare_name << "is green";
                    qDebug() << "Green Square" << square_name << "number of neighbours was increased to" << NumberOfNeighbours;
                }

                qDebug() << "Neighbour square" << checksquare_name << "is NOT green";

            }
            col_how += 2; /* making sure we enter a loop only fro col_how = -1 and col_how = +1 */
        }
    }

    if (NumberOfNeighbours == 2)
    {
        //check if those neighbours of square_name are in the same row or in the same column
        if (NeighboursCoordsVector.at(1).first == NeighboursCoordsVector.at(2).first)
        {
            qDebug() << "Squares" << AssembleName(NeighboursCoordsVector.at(1)) << "and" <<
                        AssembleName(NeighboursCoordsVector.at(2)) << "are in the same COLUMN. We're good.";
        }
        else if (NeighboursCoordsVector.at(1).second == NeighboursCoordsVector.at(2).second)
        {
            qDebug() << "Squares" << AssembleName(NeighboursCoordsVector.at(1)) << "and" <<
                        AssembleName(NeighboursCoordsVector.at(2)) << "are in the same ROW. We're good.";
        }
        else
        {
            qDebug() << "Squares" << AssembleName(NeighboursCoordsVector.at(1)) << "and" <<
                        AssembleName(NeighboursCoordsVector.at(2)) <<
                        "are not on one line, thus cannot be a part of any ship";
            return 0;
        }


    }
    else if (NumberOfNeighbours >= 3)
    {
        qDebug() << "The Green Square shouldn't have 3 or more neighbours if it is supposed to be a part of any ship";
        return 0;
    }

    return NumberOfNeighbours; /* 2 is enough for middle-square of Cruiser, 1 is enough for Destroyer */

}
/*----------------------------------------------------------------------------------*/
std::vector<QString> FillGridWindow::RemoveActualShipFromPendingPart(std::vector<std::pair<char, int>> NeighboursCoordsVector)
{
    QString current_name;

    std::vector<QString> Ship;

    std::vector <std::pair<char, int>>::iterator it;

    qDebug() << "Temporarily remove suspected part of a ship from Green Squares Vector so we would check the remainder of it";

    for (it = NeighboursCoordsVector.begin(); it != NeighboursCoordsVector.end(); it++)
    {
        current_name = AssembleName(*it);

        Ship.push_back(current_name);
        CoordinatesOfGreenSquares.erase(std::remove(CoordinatesOfGreenSquares.begin(), CoordinatesOfGreenSquares.end(), current_name), CoordinatesOfGreenSquares.end());
    }

    return Ship;
}
/*----------------------------------------------------------------------------------*/
bool FillGridWindow::ShipsCollide(std::vector<QString> ShipCoords)
{
    bool collide = false;

    std::vector<QString>::iterator it;
    std::vector<QString>::iterator jit;

    qDebug() << "Check if placed ships are colliding...";

    for (it = ShipCoords.begin(); it != ShipCoords.end(); it++)
    {
        for (jit = CoordinatesOfGreenSquares.begin(); jit != CoordinatesOfGreenSquares.end(); jit++)
        {
            qDebug() << "Check if Square of one ship" << (*it) << "is not Neighbouring Square to" << (*jit) << "of a second ship";

            /* Need To Check With Diagonals!!!! */
            if (NeighbouringSquares(*it, *jit, true))
            {
                collide = true;
                break;
            }
        }
    }

    return collide;
}

/*----------------------------------------------------------------------------------*/
void FillGridWindow::SetBattleshipCoords()
{
    qDebug() << "Push pending Green Squares to vector of Battleship Coordinates...";
    SetShipCoords("BattleShip");
    qDebug() << "Battleship Coordinates were filled";
}
/*----------------------------------------------------------------------------------*/
void FillGridWindow::SetCruisersCoords()
{
    qDebug() << "Push pending Green Squares to vector of Cruisers Coordinates...";
    SetShipCoords("Cruiser");
    qDebug() << "Cruisers Coordinates were filled";
}
/*----------------------------------------------------------------------------------*/
void FillGridWindow::SetCruisersCoords(std::vector<QString> ActualArray)
{
    qDebug() << "Push pending Green Squares to vector of Cruisers Coordinates...";
    SetShipCoords("Cruiser", ActualArray);
    qDebug() << "Cruisers Coordinates were filled";
}
/*----------------------------------------------------------------------------------*/
void FillGridWindow::SetDestroyersCoords()
{
    qDebug() << "Push pending Green Squares to vector of Destroyers Coordinates...";
    SetShipCoords("Destroyer");
    qDebug() << "Destroyers Coordinates were filled";
}
/*----------------------------------------------------------------------------------*/
void FillGridWindow::SetDestroyersCoords(std::vector<QString> ActualArray)
{
    qDebug() << "Push pending Green Squares to vector of Destroyers Coordinates...";
    SetShipCoords("Destroyer", ActualArray);
    qDebug() << "Destroyers Coordinates were filled";
}
/*----------------------------------------------------------------------------------*/
void FillGridWindow::SetTorpedoBoatsCoords()
{
    qDebug() << "Push pending Green Squares to vector of Torpedo Boats Coordinates...";
    SetShipCoords("TorpedoBoat");
    qDebug() << "Destroyers Coordinates were filled";
}
/*----------------------------------------------------------------------------------*/
void FillGridWindow::SetShipCoords(QString type_of_ship)
{
    std::vector<QString>::iterator it;
    for (it = CoordinatesOfGreenSquares.begin(); it != CoordinatesOfGreenSquares.end(); it++)
    {
        if (type_of_ship == "BattleShip")
            BattleShipCoords.push_back(*it);
        else if (type_of_ship == "Cruiser")
            CruisersCoords.push_back(*it);
        else if (type_of_ship == "Destroyer")
            DestroyersCoords.push_back(*it);
        else if (type_of_ship == "TorpedoBoat")
            TorpedoBoatsCoords.push_back(*it);
    }
}
/*----------------------------------------------------------------------------------*/
void FillGridWindow::SetShipCoords(QString type_of_ship, std::vector<QString> ActualArray)
{
    qDebug() << "Push squares which WERE CHECKED ON BEING SHIPS to vector of"<< type_of_ship << "Coordinates...";
    qDebug() << "This is done so Coords array would not have coordinates of same ships scattered";

    std::vector<QString>::iterator it;

    for(it = ActualArray.begin(); it != ActualArray.end(); it++)
    {
        if (type_of_ship == "Cruiser")
            CruisersCoords.push_back(*it);
        else if (type_of_ship == "Destroyer")
            DestroyersCoords.push_back(*it);
    }
}
/*----------------------------------------------------------------------------------*/
void FillGridWindow::on_ClearBattleshipButton_clicked()
{
    ClearWhatever("BattleShip");
}
/*----------------------------------------------------------------------------------*/
void FillGridWindow::on_ClearCruisersButton_clicked()
{
    ClearWhatever("Cruiser");
}
/*----------------------------------------------------------------------------------*/
void FillGridWindow::on_ClearDestroyersButton_clicked()
{
    ClearWhatever("Destroyer");
}
/*----------------------------------------------------------------------------------*/
void FillGridWindow::on_ClearTorpedoBoatsButton_clicked()
{
    ClearWhatever("TorpedoBoat");
}
/*----------------------------------------------------------------------------------*/
void FillGridWindow::on_ClearAllButton_clicked()
{
    qDebug() << "Gonna clear all ships placed so far...";
    ClearWhatever("BattleShip");
    ClearWhatever("Cruiser");
    ClearWhatever("Destroyer");
    ClearWhatever("TorpedoBoat");

    if (BattleShipCoords.empty() && CruisersCoords.empty() &&
            DestroyersCoords.empty() && TorpedoBoatsCoords.empty())
    {
            RandomButtonEnableSlot(true);
    }
}

/*----------------------------------------------------------------------------------*/
void FillGridWindow::ClearWhatever(QString ClassOfShip)
{
    if (ClassOfShip == "BattleShip")
    {
        /* Using actual coordinates of Battleship from vector (if it is filled),
                change Red Squares of Battleship and Grey Squares surrounding it to white */
        /* If Battleship was not placed previously, do nothing */
        if (BattleShipCoords.empty())
        {
            qDebug() << "Trying to clear BattleShip squares from grid, but there is no Battleship placed. Nothing to do";
            return;
        }
        else
        {
            std::vector<QString>::iterator it;

            for (it = BattleShipCoords.begin(); it != BattleShipCoords.end(); it++)
            {
                //qDebug() << "Change color of Red Square of BattleShip to White" << *it;
                Square.find(*it)->second->changeColor(white);

                //qDebug() << "Change color of surrounding neighbouring Grey Squares to White or at least reduce their Overlap value";
                FindBlankNeighbours(*it, white);
            }

            BattleShipCoords.clear();
            /* if BattleShip was successfully placed previously,
                 the button was disabled so we won't be able to place more same ships */
            ui->BattleshipButton->setEnabled(true);
            ui->BattleshipAmnt->setText("1");
        }
    }
    else if (ClassOfShip == "Cruiser")
    {
        if (CruisersCoords.empty())
        {
            qDebug() << "Trying to clear Cruisers squares from grid, but there are no Cruisers placed. Nothing to do";
            return;
        }
        else
        {
            std::vector<QString>::iterator it;

            for (it = CruisersCoords.begin(); it != CruisersCoords.end(); it++)
            {
                //qDebug() << "Change color of Red Square of Cruiser to White" << *it;
                Square.find(*it)->second->changeColor(white);

                //qDebug() << "Change color of surrounding neighbouring Grey Squares to White or at least reduce their Overlap value";
                FindBlankNeighbours(*it, white);
            }

            CruisersCoords.clear();

            /* unlock "Confirm Cruisers" button and restore original value of Cruisers */
            ui->CruisersButton->setEnabled(true);
            ui->CruiserAmnt->setText("2");
        }
    }
    else if (ClassOfShip == "Destroyer")
    {
        if (DestroyersCoords.empty())
        {
            qDebug() << "Trying to clear Destroyers squares from grid, but there are no Destroyers placed. Nothing to do";
            return;
        }
        else
        {
            std::vector<QString>::iterator it;

            for (it = DestroyersCoords.begin(); it != DestroyersCoords.end(); it++)
            {
                //qDebug() << "Change color of Red Square of Destroyer to White" << *it;
                Square.find(*it)->second->changeColor(white);

                //qDebug() << "Change color of surrounding neighbouring Grey Squares to White or at least reduce their Overlap value";
                FindBlankNeighbours(*it, white);
            }

            DestroyersCoords.clear();

            /* unlock "Confirm Destroyers" button and restore original value of Destroyers */
            ui->DestroyersButton->setEnabled(true);
            ui->DestroyerAmnt->setText("3");
        }
    }
    else if (ClassOfShip == "TorpedoBoat")
    {
        if (TorpedoBoatsCoords.empty())
        {
            qDebug() << "Trying to clear Torpedo Boats squares from grid, but there are no Torpedo Boats placed. Nothing to do";
            return;
        }
        else
        {
            std::vector<QString>::iterator it;

            for (it = TorpedoBoatsCoords.begin(); it != TorpedoBoatsCoords.end(); it++)
            {
                //qDebug() << "Change color of Red Square of Torpedo Boat to White" << *it;
                Square.find(*it)->second->changeColor(white);

                //qDebug() << "Change color of surrounding neighbouring Grey Squares to White or at least reduce their Overlap value";
                FindBlankNeighbours(*it, white);
            }

            TorpedoBoatsCoords.clear();

            /* unlock "Confirm Torpedo Boats" button and restore original value of Torpedo Boats */
            ui->TorpedoBoatsButton->setEnabled(true);
            ui->TorpedoBoatAmnt->setText("4");
        }
    }

    if (BattleShipCoords.empty() && CruisersCoords.empty() &&
            DestroyersCoords.empty() && TorpedoBoatsCoords.empty())
    {
            RandomButtonEnableSlot(true);
    }

    /* if some ships were cleared, JoinButton should be disabled once again */
    ui->JoinButton->setEnabled(false);

    /*PrintCurrentGreySquaresVector();*/
}
/*----------------------------------------------------------------------------------*/
void FillGridWindow::CheckAllShipsPlaced()
{
    if (!BattleShipCoords.empty() && !CruisersCoords.empty() &&
            !DestroyersCoords.empty() && !TorpedoBoatsCoords.empty() &&
            ui->BattleshipAmnt->text() == "0" && ui->CruiserAmnt->text() == "0" &&
            ui->DestroyerAmnt->text() == "0" && ui->TorpedoBoatAmnt->text() == "0")
    {
        qDebug() << "All ships were placed, now you can confirm that you're ready to join the battle";
        ui->JoinButton->setEnabled(true);
    }
}

/*----------------------------------------------------------------------------------*/
/*std::vector<QString>::iterator FillGridWindow::FindInGrey(QString coord)
{
    std::vector<QString>::iterator it;

    qDebug() << "Looking for" << coord << "in the vector of Grey Squares";

    for (it = CoordinatesOfGreySquares.begin(); it != CoordinatesOfGreySquares.end(); it++)
    {
        if (*it == coord)
        {
            return it;
        }
    }

    qDebug() << "Such square is not in vector of Grey Squares! Returned beyond-the-range value of iterator";
    return CoordinatesOfGreySquares.end();
}*/
/*----------------------------------------------------------------------------------*/
void FillGridWindow::RandomButtonEnableSlot(bool val)
{
    qDebug() << "RandomButton enabled?" << val;
    ui->RandomButton->setEnabled(val);
}
/*----------------------------------------------------------------------------------*/
void FillGridWindow::on_RandomButton_clicked()
{
    qDebug() << "Going to create Randomizing thread...";

    QObject::connect(ui->FleeButton, &QPushButton::clicked, this, &FillGridWindow::SetInterruptedTrue);
    QObject::connect(ui->ClearAllButton, &QPushButton::clicked, this, &FillGridWindow::SetInterruptedTrue);
    QObject::connect(ui->ClearBattleshipButton, &QPushButton::clicked, this, &FillGridWindow::SetInterruptedTrue);
    QObject::connect(ui->ClearCruisersButton, &QPushButton::clicked, this, &FillGridWindow::SetInterruptedTrue);
    QObject::connect(ui->ClearDestroyersButton, &QPushButton::clicked, this, &FillGridWindow::SetInterruptedTrue);
    QObject::connect(ui->ClearTorpedoBoatsButton, &QPushButton::clicked, this, &FillGridWindow::SetInterruptedTrue);

    //RandomizingThread = std::thread(&FillGridWindow::RandomButtonThread, this);

    SetRandomButtonClicked(true); /* this will be changed by Randomizing thread */

    /* separate thread for Randomizing was temporarily removed */
    RandomButtonThread();

    while (GetRandomButtonClicked())
    {
        usleep(5000);
    }

    //RandomizingThread.join();

    qDebug() << "Randomizing thread was joined";

    QObject::disconnect(ui->FleeButton, &QPushButton::clicked, this, &FillGridWindow::SetInterruptedTrue);
    QObject::disconnect(ui->ClearAllButton, &QPushButton::clicked, this, &FillGridWindow::SetInterruptedTrue);
    QObject::disconnect(ui->ClearBattleshipButton, &QPushButton::clicked, this, &FillGridWindow::SetInterruptedTrue);
    QObject::disconnect(ui->ClearCruisersButton, &QPushButton::clicked, this, &FillGridWindow::SetInterruptedTrue);
    QObject::disconnect(ui->ClearDestroyersButton, &QPushButton::clicked, this, &FillGridWindow::SetInterruptedTrue);
    QObject::disconnect(ui->ClearTorpedoBoatsButton, &QPushButton::clicked, this, &FillGridWindow::SetInterruptedTrue);

}
/*----------------------------------------------------------------------------------*/
bool FillGridWindow::WasInterrupted()
{
    bool result;

    intrptd_mu.lock();
    result = Interrupted;
    intrptd_mu.unlock();

    return result;
}
/*----------------------------------------------------------------------------------*/
void FillGridWindow::SetInterrupted(bool value)
{
    intrptd_mu.lock();
    Interrupted = value;
    intrptd_mu.unlock();
}
/*----------------------------------------------------------------------------------*/
void FillGridWindow::SetInterruptedTrue()
{
      SetInterrupted(true);
}
/*----------------------------------------------------------------------------------*/
bool FillGridWindow::GetRandomButtonClicked()
{
    bool result;

    rand_mu.lock();
    result = RandomButtonClicked;
    rand_mu.unlock();

    return result;
}
/*----------------------------------------------------------------------------------*/
void FillGridWindow::SetRandomButtonClicked(bool value)
{
    rand_mu.lock();
    RandomButtonClicked = value;
    rand_mu.unlock();

}
/*----------------------------------------------------------------------------------*/
void FillGridWindow::RandomButtonThread()
{
    RandomButtonEnableSlot(false);
    qDebug() << "Place ships randomly on the grid";

    //SetRandomButtonClicked(true);

    short LengthOfShip = 4; /* start with the longest one */
    std::pair<char, int> CurrentSquare;
    int step = 0;
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    //LengthOfShip = 3;
    while (LengthOfShip != 0)
    {
        /* Randomly puts green squares, then confirms that those were ships */
        qDebug() << LengthOfShip << "-long ship," << step+1 << "ship";
        try_again:
        QApplication::processEvents();

        if (WasInterrupted())
        {
            qDebug() << "Randomizing thread was interrupted as one of Buttons was clicked!";
            SetInterrupted(false);
            return;
        }
        CoordinatesOfGreenSquares.clear();

        char Column = 'A' + char(qrand() % ('J' - 'A'));
        int Row = 1 + (qrand() % 10);
        CurrentSquare.first = Column;
        CurrentSquare.second = Row;
        QString CurrentSquareStr = AssembleName(CurrentSquare);

        qDebug() << "Random coordinate:" << CurrentSquareStr;

        int RandomOrientation = qrand() % 2;

        if (RandomOrientation == 0)
        {
            qDebug() << "Ship will be oriented horizontally";

            if (FillShipLoop(CurrentSquare, LengthOfShip, 0, false) < 0)
                goto try_again;

        }
        else
        {
            qDebug() << "Ship will be oriented vertically";
            if (FillShipLoop(CurrentSquare, LengthOfShip, 1, false) < 0)
                goto try_again;
        }

        step++;

        if (LengthOfShip == 4)
        {
            step = 0;

            if (AsIf_BattleshipButton_clicked() == -1)
            {
                goto try_again;
            }

            PrintCurrentGreenSquaresVector();
            GetCoords("BS");
            LengthOfShip--;

        }
        else if (LengthOfShip == 3 && step == 2)
        {
            step = 0;

            if (AsIf_CruisersButton_clicked() == -1)
            {
                ClearWhatever("Cruiser");
                goto try_again;
            }
//            else if (CruisersCoords.size() != 6)
//            {
//                ClearWhatever("Cruiser");
//                goto try_again;
//            }
            PrintCurrentGreenSquaresVector();
            GetCoords("BS");
            GetCoords("CR");
            LengthOfShip--;

        }
        else if (LengthOfShip == 2 && step == 3)
        {
            step = 0;

            if (AsIf_DestroyersButton_clicked() == -1)
            {
                ClearWhatever("Destroyer");
                goto try_again;
            }
//            else if (DestroyersCoords.size() != 6)
//            {
//                ClearWhatever("Destroyer");
//                goto try_again;
//            }

            PrintCurrentGreenSquaresVector();
            GetCoords("BS");
            GetCoords("CR");
            GetCoords("DS");
            LengthOfShip--;

        }
        else if (LengthOfShip == 1 && step == 4)
        {
            step = 0;

            if (AsIf_TopedoBoatsButton_clicked() == -1)
            {
                ClearWhatever("TorpedoBoat");
                goto try_again;
            }
//            else if (TorpedoBoatsCoords.size() != 4)
//            {
//                ClearWhatever("TorpedoBoat");
//                goto try_again;
//            }

            PrintCurrentGreenSquaresVector();
            GetCoords("BS");
            GetCoords("CR");
            GetCoords("DS");
            GetCoords("TB");
            LengthOfShip--;
        }
    }

    qDebug() << "Apparently, all ships were randomly placed";
    SetRandomButtonClicked(false);
    return;
}

/*----------------------------------------------------------------------------------*/
short FillGridWindow::FillShipLoop(std::pair<char, int> StartSquare, int LengthOfShip, const short Which, const bool Reversed)
{
    qDebug() << "Entered FillShipLoop() function...";
    qDebug() << "Trying to put" << LengthOfShip << "-squares ship";
    short SquaresLeft = LengthOfShip;
    short left_limit, right_limit;
    std::pair<char, int> CurrentSquare = StartSquare;
    QString CurrentSquareStr;

    if (Which == 0) { right_limit = 'J'; left_limit = 'A'; }
    else if (Which == 1) { right_limit = 10; left_limit = 1; }

    for (int i = 0; (Reversed) ? (i > -LengthOfShip) : (i < LengthOfShip); ((Reversed) ? i-- : i++))
    {
        if (Which == 0) { CurrentSquare.first = StartSquare.first + i; }
        else if (Which == 1) { CurrentSquare.second = StartSquare.second + i; }

        CurrentSquareStr = AssembleName(CurrentSquare);
        qDebug() << "Is it possible to take square" << CurrentSquareStr;
        qDebug() << Square.find(CurrentSquareStr)->second->getColor() << "is color of square";
        qDebug() << "Column" << QString(CurrentSquare.first) << "has equivalent integer value" << QString::number(CurrentSquare.first);

        if ((Which == 0 && (CurrentSquare.first > right_limit || CurrentSquare.first < left_limit)) ||
                (Which == 1 && (CurrentSquare.second > right_limit || CurrentSquare.second < left_limit)) ||
                (Square.find(CurrentSquareStr)->second->getColor() == grey) ||
                (Square.find(CurrentSquareStr)->second->getColor() == red) ||
                (Square.find(CurrentSquareStr)->second->getColor() == green))
        {
            if (Reversed)
            {
               qDebug() << "Trying to continue the ship in opposite direction did not help!";
               return -1;
            }

            qDebug() << "We are stuck! Try to go in opposite direction (recursive call of FillShipLoop())!";

            if (Which == 0) { CurrentSquare.first = StartSquare.first - 1;  }
            else if (Which == 1) { CurrentSquare.second = StartSquare.second - 1; }
            qDebug() << "Start on square" << AssembleName(CurrentSquare);

            short Result = 0;
            if (!Reversed)
            {
                if ((Result = FillShipLoop(CurrentSquare, SquaresLeft, Which, true)) == -1)
                {
                    qDebug() << "Unable to put a ship here! Fetch a random square again!";

                    ClearCoordinatesOfGreenSquares();
                    //CoordinatesOfGreenSquares.clear();

                    return -2;
                }
            }

            if (Reversed /*|| (Result == -1)*/)
            {
                //qDebug() << "Unable to put a ship here! Fetch a random square again!";
                qDebug() << "Trying to continue the ship in opposite direction did not help!";

                //ClearCoordinatesOfGreenSquares();
                //CoordinatesOfGreenSquares.clear();

                return -1;
            }

            return 0;

        }

        qDebug() << "Square" << CurrentSquareStr << "can be colored to green now";
        Square.find(CurrentSquareStr)->second->changeColor(green);
        SquaresLeft--;
        qDebug() << SquaresLeft << "squares left to complete this ship";

    }

    qDebug() << "Apparently, ship was successfully placed if started on" << AssembleName(StartSquare);
    return 0;
}
/*----------------------------------------------------------------------------------*/
void FillGridWindow::ErrorMess(QString errmsg)
{
    if (!RandomButtonClicked)
        QMessageBox::warning(this, "Error", errmsg);
}

/*----------------------------------------------------------------------------------*/
void FillGridWindow::on_FleeButton_clicked()
{
    qDebug() << "Flee button was pressed, going to send \"bbye\" message to the server"
                " and close all connection and join all threads";

    close();
}
/*----------------------------------------------------------------------------------*/
void FillGridWindow::on_JoinButton_clicked()
{
    qDebug() << "Going to send ships coordinates to the server. If another Player already filled his/her grid and shared it with server,"
                " start the game. Otherwise, wait for second client";

    emit SendFillSig();

    ui->JoinButton->setEnabled(false);

    ui->ClearAllButton->setEnabled(false);
    ui->ClearBattleshipButton->setEnabled(false);
    ui->ClearCruisersButton->setEnabled(false);
    ui->ClearDestroyersButton->setEnabled(false);
    ui->ClearTorpedoBoatsButton->setEnabled(false);

    /*QMessageBox::information(this, "Waiting...", "Waiting for second player to place ships on the grid. "
                                                 "Once server gets coordinates of ships from both players, "
                                                 "the main window will be opened");*/
    if (GetStartGame() == false)
        emit ShowWaitingWindowSig();

    while (GetStartGame() == false)
    {
        /* sleep until both clients fill their grids */
        usleep(5000);

        QCoreApplication::processEvents(); /* in case we pressed "Flee" */

        if (GetServerUp() == false) /* if server notified us that it is shutdown */
        {
            /* server was shutdown, stop waiting then, it wouldn't happen */
            return;
        }
    }

    //emit HideWaitingWindowSig(); /* no need, Waiting Window has a check GetServerUp() for its own */

    qDebug() << "Your adversary name is" << AdversaryName;
    qDebug() << "Server received coordinates of both players ships. Start the game!";
    hide();
    emit ShowMainWindowSig();

    SetOneGreenSquareChosen("");

    RandomButtonEnableSlot(false);

}
/*----------------------------------------------------------------------------------*/
void FillGridWindow::hideFillGridWindowSlot()
{
    hide();
}
/*----------------------------------------------------------------------------------*/
void FillGridWindow::closeEvent(QCloseEvent *event)
{
    on_ClearAllButton_clicked();

    if (GetServerUp())
    {
        qDebug() << "sending BBYE message to the server";
        emit SendBbyeSig();

        qDebug() << "We're currently in the thread with ID" << GetQStrThreadId(std::this_thread::get_id());
        qDebug() << "Close connection and join the receiving thread...";
        emit CloseConnAndJoinThreadSig();
    }
    else if (/*GetStartGame() == false*/this->isVisible()) /* show discon window from here only if Grid is not yet filled */
    {
         QMessageBox::critical(this, "Disconnected from server!", "Disconnected from Server!");
    }

    emit CloseWaitingWindowSig();

    qDebug() << "Closing the \"Fill-the-Grid\" window";
    event->QCloseEvent::accept();
}
/*----------------------------------------------------------------------------------*/
FillGridWindow::~FillGridWindow()
{
    delete ui;
}

