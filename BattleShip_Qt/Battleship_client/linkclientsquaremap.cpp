#include "client.h"
#include "ui_client.h"

/*----------------------------------------------------------------------------------*/
void client::LinkYourSquareMap()
{
    YourSquare.insert(std::pair<QString,QGridSquare *>("A1",ui->YA1));
    ui->YA1->setToolTip("A1");
    ui->YA1->setClickability(false);
    ui->YA1->setName("Player's own A1");
    YourSquare.insert(std::pair<QString,QGridSquare *>("A2",ui->YA2));
    ui->YA2->setToolTip("A2");
    ui->YA2->setClickability(false);
    ui->YA2->setName("Player's own A2");
    YourSquare.insert(std::pair<QString,QGridSquare *>("A3",ui->YA3));
    ui->YA3->setToolTip("A3");
    ui->YA3->setClickability(false);
    ui->YA3->setName("Player's own A3");
    YourSquare.insert(std::pair<QString,QGridSquare *>("A4",ui->YA4));
    ui->YA4->setToolTip("A4");
    ui->YA4->setClickability(false);
    ui->YA4->setName("Player's own A4");
    YourSquare.insert(std::pair<QString,QGridSquare *>("A5",ui->YA5));
    ui->YA5->setToolTip("A5");
    ui->YA5->setClickability(false);
    ui->YA5->setName("Player's own A5");
    YourSquare.insert(std::pair<QString,QGridSquare *>("A6",ui->YA6));
    ui->YA6->setToolTip("A6");
    ui->YA6->setClickability(false);
    ui->YA6->setName("Player's own A6");
    YourSquare.insert(std::pair<QString,QGridSquare *>("A7",ui->YA7));
    ui->YA7->setToolTip("A7");
    ui->YA7->setClickability(false);
    ui->YA7->setName("Player's own A7");
    YourSquare.insert(std::pair<QString,QGridSquare *>("A8",ui->YA8));
    ui->YA8->setToolTip("A8");
    ui->YA8->setClickability(false);
    ui->YA8->setName("Player's own A8");
    YourSquare.insert(std::pair<QString,QGridSquare *>("A9",ui->YA9));
    ui->YA9->setToolTip("A9");
    ui->YA9->setClickability(false);
    ui->YA9->setName("Player's own A9");
    YourSquare.insert(std::pair<QString,QGridSquare *>("A10",ui->YA10));
    ui->YA10->setToolTip("A10");
    ui->YA10->setClickability(false);
    ui->YA10->setName("Player's own A10");

    YourSquare.insert(std::pair<QString,QGridSquare *>("B1",ui->YB1));
    ui->YB1->setToolTip("B1");
    ui->YB1->setClickability(false);
    ui->YB1->setName("Player's own B1");
    YourSquare.insert(std::pair<QString,QGridSquare *>("B2",ui->YB2));
    ui->YB2->setToolTip("B2");
    ui->YB2->setClickability(false);
    ui->YB2->setName("Player's own B2");
    YourSquare.insert(std::pair<QString,QGridSquare *>("B3",ui->YB3));
    ui->YB3->setToolTip("B3");
    ui->YB3->setClickability(false);
    ui->YB3->setName("Player's own B3");
    YourSquare.insert(std::pair<QString,QGridSquare *>("B4",ui->YB4));
    ui->YB4->setToolTip("B4");
    ui->YB4->setClickability(false);
    ui->YB4->setName("Player's own B4");
    YourSquare.insert(std::pair<QString,QGridSquare *>("B5",ui->YB5));
    ui->YB5->setToolTip("B5");
    ui->YB5->setClickability(false);
    ui->YB5->setName("Player's own B5");
    YourSquare.insert(std::pair<QString,QGridSquare *>("B6",ui->YB6));
    ui->YB6->setToolTip("B6");
    ui->YB6->setClickability(false);
    ui->YB6->setName("Player's own B6");
    YourSquare.insert(std::pair<QString,QGridSquare *>("B7",ui->YB7));
    ui->YB7->setToolTip("B7");
    ui->YB7->setClickability(false);
    ui->YB7->setName("Player's own B7");
    YourSquare.insert(std::pair<QString,QGridSquare *>("B8",ui->YB8));
    ui->YB8->setToolTip("B8");
    ui->YB8->setClickability(false);
    ui->YB8->setName("Player's own B8");
    YourSquare.insert(std::pair<QString,QGridSquare *>("B9",ui->YB9));
    ui->YB9->setToolTip("B9");
    ui->YB9->setClickability(false);
    ui->YB9->setName("Player's own B9");
    YourSquare.insert(std::pair<QString,QGridSquare *>("B10",ui->YB10));
    ui->YB10->setToolTip("B10");
    ui->YB10->setClickability(false);
    ui->YB10->setName("Player's own B10");

    YourSquare.insert(std::pair<QString,QGridSquare *>("C1",ui->YC1));
    ui->YC1->setToolTip("C1");
    ui->YC1->setClickability(false);
    ui->YC1->setName("Player's own C1");
    YourSquare.insert(std::pair<QString,QGridSquare *>("C2",ui->YC2));
    ui->YC2->setToolTip("C2");
    ui->YC2->setClickability(false);
    ui->YC2->setName("Player's own C2");
    YourSquare.insert(std::pair<QString,QGridSquare *>("C3",ui->YC3));
    ui->YC3->setToolTip("C3");
    ui->YC3->setClickability(false);
    ui->YC3->setName("Player's own C3");
    YourSquare.insert(std::pair<QString,QGridSquare *>("C4",ui->YC4));
    ui->YC4->setToolTip("C4");
    ui->YC4->setClickability(false);
    ui->YC4->setName("Player's own C4");
    YourSquare.insert(std::pair<QString,QGridSquare *>("C5",ui->YC5));
    ui->YC5->setToolTip("C5");
    ui->YC5->setClickability(false);
    ui->YC5->setName("Player's own C5");
    YourSquare.insert(std::pair<QString,QGridSquare *>("C6",ui->YC6));
    ui->YC6->setToolTip("C6");
    ui->YC6->setClickability(false);
    ui->YC6->setName("Player's own C6");
    YourSquare.insert(std::pair<QString,QGridSquare *>("C7",ui->YC7));
    ui->YC7->setToolTip("C7");
    ui->YC7->setClickability(false);
    ui->YC7->setName("Player's own C7");
    YourSquare.insert(std::pair<QString,QGridSquare *>("C8",ui->YC8));
    ui->YC8->setToolTip("C8");
    ui->YC8->setClickability(false);
    ui->YC8->setName("Player's own C8");
    YourSquare.insert(std::pair<QString,QGridSquare *>("C9",ui->YC9));
    ui->YC9->setToolTip("C9");
    ui->YC9->setClickability(false);
    ui->YC9->setName("Player's own C9");
    YourSquare.insert(std::pair<QString,QGridSquare *>("C10",ui->YC10));
    ui->YC10->setToolTip("C10");
    ui->YC10->setClickability(false);
    ui->YC10->setName("Player's own C10");

    YourSquare.insert(std::pair<QString,QGridSquare *>("D1",ui->YD1));
    ui->YD1->setToolTip("D1");
    ui->YD1->setClickability(false);
    ui->YD1->setName("Player's own D1");
    YourSquare.insert(std::pair<QString,QGridSquare *>("D2",ui->YD2));
    ui->YD2->setToolTip("D2");
    ui->YD2->setClickability(false);
    ui->YD2->setName("Player's own D2");
    YourSquare.insert(std::pair<QString,QGridSquare *>("D3",ui->YD3));
    ui->YD3->setToolTip("D3");
    ui->YD3->setClickability(false);
    ui->YD3->setName("Player's own D3");
    YourSquare.insert(std::pair<QString,QGridSquare *>("D4",ui->YD4));
    ui->YD4->setToolTip("D4");
    ui->YD4->setClickability(false);
    ui->YD4->setName("Player's own D4");
    YourSquare.insert(std::pair<QString,QGridSquare *>("D5",ui->YD5));
    ui->YD5->setToolTip("D5");
    ui->YD5->setClickability(false);
    ui->YD5->setName("Player's own D5");
    YourSquare.insert(std::pair<QString,QGridSquare *>("D6",ui->YD6));
    ui->YD6->setToolTip("D6");
    ui->YD6->setClickability(false);
    ui->YD6->setName("Player's own D6");
    YourSquare.insert(std::pair<QString,QGridSquare *>("D7",ui->YD7));
    ui->YD7->setToolTip("D7");
    ui->YD7->setClickability(false);
    ui->YD7->setName("Player's own D7");
    YourSquare.insert(std::pair<QString,QGridSquare *>("D8",ui->YD8));
    ui->YD8->setToolTip("D8");
    ui->YD8->setClickability(false);
    ui->YD8->setName("Player's own D8");
    YourSquare.insert(std::pair<QString,QGridSquare *>("D9",ui->YD9));
    ui->YD9->setToolTip("D9");
    ui->YD9->setClickability(false);
    ui->YD9->setName("Player's own D9");
    YourSquare.insert(std::pair<QString,QGridSquare *>("D10",ui->YD10));
    ui->YD10->setToolTip("D10");
    ui->YD10->setClickability(false);
    ui->YD10->setName("Player's own D10");

    YourSquare.insert(std::pair<QString,QGridSquare *>("E1",ui->YE1));
    ui->YE1->setToolTip("E1");
    ui->YE1->setClickability(false);
    ui->YE1->setName("Player's own E1");
    YourSquare.insert(std::pair<QString,QGridSquare *>("E2",ui->YE2));
    ui->YE2->setToolTip("E2");
    ui->YE2->setClickability(false);
    ui->YE2->setName("Player's own E2");
    YourSquare.insert(std::pair<QString,QGridSquare *>("E3",ui->YE3));
    ui->YE3->setToolTip("E3");
    ui->YE3->setClickability(false);
    ui->YE3->setName("Player's own E3");
    YourSquare.insert(std::pair<QString,QGridSquare *>("E4",ui->YE4));
    ui->YE4->setToolTip("E4");
    ui->YE4->setClickability(false);
    ui->YE4->setName("Player's own E4");
    YourSquare.insert(std::pair<QString,QGridSquare *>("E5",ui->YE5));
    ui->YE5->setToolTip("E5");
    ui->YE5->setClickability(false);
    ui->YE5->setName("Player's own E5");
    YourSquare.insert(std::pair<QString,QGridSquare *>("E6",ui->YE6));
    ui->YE6->setToolTip("E6");
    ui->YE6->setClickability(false);
    ui->YE6->setName("Player's own E6");
    YourSquare.insert(std::pair<QString,QGridSquare *>("E7",ui->YE7));
    ui->YE7->setToolTip("E7");
    ui->YE7->setClickability(false);
    ui->YE7->setName("Player's own E7");
    YourSquare.insert(std::pair<QString,QGridSquare *>("E8",ui->YE8));
    ui->YE8->setToolTip("E8");
    ui->YE8->setClickability(false);
    ui->YE8->setName("Player's own E8");
    YourSquare.insert(std::pair<QString,QGridSquare *>("E9",ui->YE9));
    ui->YE9->setToolTip("E9");
    ui->YE9->setClickability(false);
    ui->YE9->setName("Player's own E9");
    YourSquare.insert(std::pair<QString,QGridSquare *>("E10",ui->YE10));
    ui->YE10->setToolTip("E10");
    ui->YE10->setClickability(false);
    ui->YE10->setName("Player's own E10");

    YourSquare.insert(std::pair<QString,QGridSquare *>("F1",ui->YF1));
    ui->YF1->setToolTip("F1");
    ui->YF1->setClickability(false);
    ui->YF1->setName("Player's own F1");
    YourSquare.insert(std::pair<QString,QGridSquare *>("F2",ui->YF2));
    ui->YF2->setToolTip("F2");
    ui->YF2->setClickability(false);
    ui->YF2->setName("Player's own F2");
    YourSquare.insert(std::pair<QString,QGridSquare *>("F3",ui->YF3));
    ui->YF3->setToolTip("F3");
    ui->YF3->setClickability(false);
    ui->YF3->setName("Player's own F3");
    YourSquare.insert(std::pair<QString,QGridSquare *>("F4",ui->YF4));
    ui->YF4->setToolTip("F4");
    ui->YF4->setClickability(false);
    ui->YF4->setName("Player's own F4");
    YourSquare.insert(std::pair<QString,QGridSquare *>("F5",ui->YF5));
    ui->YF5->setToolTip("F5");
    ui->YF5->setClickability(false);
    ui->YF5->setName("Player's own F5");
    YourSquare.insert(std::pair<QString,QGridSquare *>("F6",ui->YF6));
    ui->YF6->setToolTip("F6");
    ui->YF6->setClickability(false);
    ui->YF6->setName("Player's own F6");
    YourSquare.insert(std::pair<QString,QGridSquare *>("F7",ui->YF7));
    ui->YF7->setToolTip("F7");
    ui->YF7->setClickability(false);
    ui->YF7->setName("Player's own F7");
    YourSquare.insert(std::pair<QString,QGridSquare *>("F8",ui->YF8));
    ui->YF8->setToolTip("F8");
    ui->YF8->setClickability(false);
    ui->YF8->setName("Player's own F8");
    YourSquare.insert(std::pair<QString,QGridSquare *>("F9",ui->YF9));
    ui->YF9->setToolTip("F9");
    ui->YF9->setClickability(false);
    ui->YF9->setName("Player's own F9");
    YourSquare.insert(std::pair<QString,QGridSquare *>("F10",ui->YF10));
    ui->YF10->setToolTip("F10");
    ui->YF10->setClickability(false);
    ui->YF10->setName("Player's own F10");

    YourSquare.insert(std::pair<QString,QGridSquare *>("G1",ui->YG1));
    ui->YG1->setToolTip("G1");
    ui->YG1->setClickability(false);
    ui->YG1->setName("Player's own G1");
    YourSquare.insert(std::pair<QString,QGridSquare *>("G2",ui->YG2));
    ui->YG2->setToolTip("G2");
    ui->YG2->setClickability(false);
    ui->YG2->setName("Player's own G2");
    YourSquare.insert(std::pair<QString,QGridSquare *>("G3",ui->YG3));
    ui->YG3->setToolTip("G3");
    ui->YG3->setClickability(false);
    ui->YG3->setName("Player's own G3");
    YourSquare.insert(std::pair<QString,QGridSquare *>("G4",ui->YG4));
    ui->YG4->setToolTip("G4");
    ui->YG4->setClickability(false);
    ui->YG4->setName("Player's own G4");
    YourSquare.insert(std::pair<QString,QGridSquare *>("G5",ui->YG5));
    ui->YG5->setToolTip("G5");
    ui->YG5->setClickability(false);
    ui->YG5->setName("Player's own G5");
    YourSquare.insert(std::pair<QString,QGridSquare *>("G6",ui->YG6));
    ui->YG6->setToolTip("G6");
    ui->YG6->setClickability(false);
    ui->YG6->setName("Player's own G6");
    YourSquare.insert(std::pair<QString,QGridSquare *>("G7",ui->YG7));
    ui->YG7->setToolTip("G7");
    ui->YG7->setClickability(false);
    ui->YG7->setName("Player's own G7");
    YourSquare.insert(std::pair<QString,QGridSquare *>("G8",ui->YG8));
    ui->YG8->setToolTip("G8");
    ui->YG8->setClickability(false);
    ui->YG8->setName("Player's own G8");
    YourSquare.insert(std::pair<QString,QGridSquare *>("G9",ui->YG9));
    ui->YG9->setToolTip("G9");
    ui->YG9->setClickability(false);
    ui->YG9->setName("Player's own G9");
    YourSquare.insert(std::pair<QString,QGridSquare *>("G10",ui->YG10));
    ui->YG10->setToolTip("G10");
    ui->YG10->setClickability(false);
    ui->YG10->setName("Player's own G10");

    YourSquare.insert(std::pair<QString,QGridSquare *>("H1",ui->YH1));
    ui->YH1->setToolTip("H1");
    ui->YH1->setClickability(false);
    ui->YH1->setName("Player's own H1");
    YourSquare.insert(std::pair<QString,QGridSquare *>("H2",ui->YH2));
    ui->YH2->setToolTip("H2");
    ui->YH2->setClickability(false);
    ui->YH2->setName("Player's own H2");
    YourSquare.insert(std::pair<QString,QGridSquare *>("H3",ui->YH3));
    ui->YH3->setToolTip("H3");
    ui->YH3->setClickability(false);
    ui->YH3->setName("Player's own H3");
    YourSquare.insert(std::pair<QString,QGridSquare *>("H4",ui->YH4));
    ui->YH4->setToolTip("H4");
    ui->YH4->setClickability(false);
    ui->YH4->setName("Player's own H4");
    YourSquare.insert(std::pair<QString,QGridSquare *>("H5",ui->YH5));
    ui->YH5->setToolTip("H5");
    ui->YH5->setClickability(false);
    ui->YH5->setName("Player's own H5");
    YourSquare.insert(std::pair<QString,QGridSquare *>("H6",ui->YH6));
    ui->YH6->setToolTip("H6");
    ui->YH6->setClickability(false);
    ui->YH6->setName("Player's own H6");
    YourSquare.insert(std::pair<QString,QGridSquare *>("H7",ui->YH7));
    ui->YH7->setToolTip("H7");
    ui->YH7->setClickability(false);
    ui->YH7->setName("Player's own H7");
    YourSquare.insert(std::pair<QString,QGridSquare *>("H8",ui->YH8));
    ui->YH8->setToolTip("H8");
    ui->YH8->setClickability(false);
    ui->YH8->setName("Player's own H8");
    YourSquare.insert(std::pair<QString,QGridSquare *>("H9",ui->YH9));
    ui->YH9->setToolTip("H9");
    ui->YH9->setClickability(false);
    ui->YH9->setName("Player's own H9");
    YourSquare.insert(std::pair<QString,QGridSquare *>("H10",ui->YH10));
    ui->YH10->setToolTip("H10");
    ui->YH10->setClickability(false);
    ui->YH10->setName("Player's own H10");

    YourSquare.insert(std::pair<QString,QGridSquare *>("I1",ui->YI1));
    ui->YI1->setToolTip("I1");
    ui->YI1->setClickability(false);
    ui->YI1->setName("Player's own I1");
    YourSquare.insert(std::pair<QString,QGridSquare *>("I2",ui->YI2));
    ui->YI2->setToolTip("I2");
    ui->YI2->setClickability(false);
    ui->YI2->setName("Player's own I2");
    YourSquare.insert(std::pair<QString,QGridSquare *>("I3",ui->YI3));
    ui->YI3->setToolTip("I3");
    ui->YI3->setClickability(false);
    ui->YI3->setName("Player's own I3");
    YourSquare.insert(std::pair<QString,QGridSquare *>("I4",ui->YI4));
    ui->YI4->setToolTip("I4");
    ui->YI4->setClickability(false);
    ui->YI4->setName("Player's own I4");
    YourSquare.insert(std::pair<QString,QGridSquare *>("I5",ui->YI5));
    ui->YI5->setToolTip("I5");
    ui->YI5->setClickability(false);
    ui->YI5->setName("Player's own I5");
    YourSquare.insert(std::pair<QString,QGridSquare *>("I6",ui->YI6));
    ui->YI6->setToolTip("I6");
    ui->YI6->setClickability(false);
    ui->YI6->setName("Player's own I6");
    YourSquare.insert(std::pair<QString,QGridSquare *>("I7",ui->YI7));
    ui->YI7->setToolTip("I7");
    ui->YI7->setClickability(false);
    ui->YI7->setName("Player's own I7");
    YourSquare.insert(std::pair<QString,QGridSquare *>("I8",ui->YI8));
    ui->YI8->setToolTip("I8");
    ui->YI8->setClickability(false);
    ui->YI8->setName("Player's own I8");
    YourSquare.insert(std::pair<QString,QGridSquare *>("I9",ui->YI9));
    ui->YI9->setToolTip("I9");
    ui->YI9->setClickability(false);
    ui->YI9->setName("Player's own I9");
    YourSquare.insert(std::pair<QString,QGridSquare *>("I10",ui->YI10));
    ui->YI10->setToolTip("I10");
    ui->YI10->setClickability(false);
    ui->YI10->setName("Player's own I10");

    YourSquare.insert(std::pair<QString,QGridSquare *>("J1",ui->YJ1));
    ui->YJ1->setToolTip("J1");
    ui->YJ1->setClickability(false);
    ui->YJ1->setName("Player's own J1");
    YourSquare.insert(std::pair<QString,QGridSquare *>("J2",ui->YJ2));
    ui->YJ2->setToolTip("J2");
    ui->YJ2->setClickability(false);
    ui->YJ2->setName("Player's own J2");
    YourSquare.insert(std::pair<QString,QGridSquare *>("J3",ui->YJ3));
    ui->YJ3->setToolTip("J3");
    ui->YJ3->setClickability(false);
    ui->YJ3->setName("Player's own J3");
    YourSquare.insert(std::pair<QString,QGridSquare *>("J4",ui->YJ4));
    ui->YJ4->setToolTip("J4");
    ui->YJ4->setClickability(false);
    ui->YJ4->setName("Player's own J4");
    YourSquare.insert(std::pair<QString,QGridSquare *>("J5",ui->YJ5));
    ui->YJ5->setToolTip("J5");
    ui->YJ5->setClickability(false);
    ui->YJ5->setName("Player's own J5");
    YourSquare.insert(std::pair<QString,QGridSquare *>("J6",ui->YJ6));
    ui->YJ6->setToolTip("J6");
    ui->YJ6->setClickability(false);
    ui->YJ6->setName("Player's own J6");
    YourSquare.insert(std::pair<QString,QGridSquare *>("J7",ui->YJ7));
    ui->YJ7->setToolTip("J7");
    ui->YJ7->setClickability(false);
    ui->YJ7->setName("Player's own J7");
    YourSquare.insert(std::pair<QString,QGridSquare *>("J8",ui->YJ8));
    ui->YJ8->setToolTip("J8");
    ui->YJ8->setClickability(false);
    ui->YJ8->setName("Player's own J8");
    YourSquare.insert(std::pair<QString,QGridSquare *>("J9",ui->YJ9));
    ui->YJ9->setToolTip("J9");
    ui->YJ9->setClickability(false);
    ui->YJ9->setName("Player's own J9");
    YourSquare.insert(std::pair<QString,QGridSquare *>("J10",ui->YJ10));
    ui->YJ10->setToolTip("J10");
    ui->YJ10->setClickability(false);
    ui->YJ10->setName("Player's own J10");

    qDebug() << "All Your squares on Client Window were associated with their coordinates in QString format";
}

/*----------------------------------------------------------------------------------*/
void client::LinkEnemySquareMap()
{
    EnemySquare.insert(std::pair<QString,QGridSquare *>("A1",ui->A1));
    ui->A1->setToolTip("A1");
    QObject::connect(ui->A1, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("A2",ui->A2));
    ui->A2->setToolTip("A2");
    QObject::connect(ui->A2, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("A3",ui->A3));
    ui->A3->setToolTip("A3");
    QObject::connect(ui->A3, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("A4",ui->A4));
    ui->A4->setToolTip("A4");
    QObject::connect(ui->A4, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("A5",ui->A5));
    ui->A5->setToolTip("A5");
    QObject::connect(ui->A5, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("A6",ui->A6));
    ui->A6->setToolTip("A6");
    QObject::connect(ui->A6, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("A7",ui->A7));
    ui->A7->setToolTip("A7");
    QObject::connect(ui->A7, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("A8",ui->A8));
    ui->A8->setToolTip("A8");
    QObject::connect(ui->A8, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("A9",ui->A9));
    ui->A9->setToolTip("A9");
    QObject::connect(ui->A9, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("A10",ui->A10));
    ui->A10->setToolTip("A10");
    QObject::connect(ui->A10, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);


    EnemySquare.insert(std::pair<QString,QGridSquare *>("B1",ui->B1));
    ui->B1->setToolTip("B1");
    QObject::connect(ui->B1, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("B2",ui->B2));
    ui->B2->setToolTip("B2");
    QObject::connect(ui->B2, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("B3",ui->B3));
    ui->B3->setToolTip("B3");
    QObject::connect(ui->B3, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("B4",ui->B4));
    ui->B4->setToolTip("B4");
    QObject::connect(ui->B4, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("B5",ui->B5));
    ui->B5->setToolTip("B5");
    QObject::connect(ui->B5, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("B6",ui->B6));
    ui->B6->setToolTip("B6");
    QObject::connect(ui->B6, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("B7",ui->B7));
    ui->B7->setToolTip("B7");
    QObject::connect(ui->B7, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("B8",ui->B8));
    ui->B8->setToolTip("B8");
    QObject::connect(ui->B8, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("B9",ui->B9));
    ui->B9->setToolTip("B9");
    QObject::connect(ui->B9, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("B10",ui->B10));
    ui->B10->setToolTip("B10");
    QObject::connect(ui->B10, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("C1",ui->C1));
    ui->C1->setToolTip("C1");
    QObject::connect(ui->C1, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("C2",ui->C2));
    ui->C2->setToolTip("C2");
    QObject::connect(ui->C2, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("C3",ui->C3));
    ui->C3->setToolTip("C3");
    QObject::connect(ui->C3, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("C4",ui->C4));
    ui->C4->setToolTip("C4");
    QObject::connect(ui->C4, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("C5",ui->C5));
    ui->C5->setToolTip("C5");
    QObject::connect(ui->C5, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("C6",ui->C6));
    ui->C6->setToolTip("C6");
    QObject::connect(ui->C6, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("C7",ui->C7));
    ui->C7->setToolTip("C7");
    QObject::connect(ui->C7, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("C8",ui->C8));
    ui->C8->setToolTip("C8");
    QObject::connect(ui->C8, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("C9",ui->C9));
    ui->C9->setToolTip("C9");
    QObject::connect(ui->C9, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("C10",ui->C10));
    ui->C10->setToolTip("C10");
    QObject::connect(ui->C10, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("D1",ui->D1));
    ui->D1->setToolTip("D1");
    QObject::connect(ui->D1, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("D2",ui->D2));
    ui->D2->setToolTip("D2");
    QObject::connect(ui->D2, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("D3",ui->D3));
    ui->D3->setToolTip("D3");
    QObject::connect(ui->D3, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("D4",ui->D4));
    ui->D4->setToolTip("D4");
    QObject::connect(ui->D4, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("D5",ui->D5));
    ui->D5->setToolTip("D5");
    QObject::connect(ui->D5, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("D6",ui->D6));
    ui->D6->setToolTip("D6");
    QObject::connect(ui->D6, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("D7",ui->D7));
    ui->D7->setToolTip("D7");
    QObject::connect(ui->D7, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("D8",ui->D8));
    ui->D8->setToolTip("D8");
    QObject::connect(ui->D8, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("D9",ui->D9));
    ui->D9->setToolTip("D9");
    QObject::connect(ui->D9, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("D10",ui->D10));
    ui->D10->setToolTip("D10");
    QObject::connect(ui->D10, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);


    EnemySquare.insert(std::pair<QString,QGridSquare *>("E1",ui->E1));
    ui->E1->setToolTip("E1");
    QObject::connect(ui->E1, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("E2",ui->E2));
    ui->E2->setToolTip("E2");
    QObject::connect(ui->E2, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("E3",ui->E3));
    ui->E3->setToolTip("E3");
    QObject::connect(ui->E3, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("E4",ui->E4));
    ui->E4->setToolTip("E4");
    QObject::connect(ui->E4, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("E5",ui->E5));
    ui->E5->setToolTip("E5");
    QObject::connect(ui->E5, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("E6",ui->E6));
    ui->E6->setToolTip("E6");
    QObject::connect(ui->E6, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("E7",ui->E7));
    ui->E7->setToolTip("E7");
    QObject::connect(ui->E7, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("E8",ui->E8));
    ui->E8->setToolTip("E8");
    QObject::connect(ui->E8, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("E9",ui->E9));
    ui->E9->setToolTip("E9");
    QObject::connect(ui->E9, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("E10",ui->E10));
    ui->E10->setToolTip("E10");
    QObject::connect(ui->E10, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("F1",ui->F1));
    ui->F1->setToolTip("F1");
    QObject::connect(ui->F1, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("F2",ui->F2));
    ui->F2->setToolTip("F2");
    QObject::connect(ui->F2, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("F3",ui->F3));
    ui->F3->setToolTip("F3");
    QObject::connect(ui->F3, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("F4",ui->F4));
    ui->F4->setToolTip("F4");
    QObject::connect(ui->F4, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("F5",ui->F5));
    ui->F5->setToolTip("F5");
    QObject::connect(ui->F5, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("F6",ui->F6));
    ui->F6->setToolTip("F6");
    QObject::connect(ui->F6, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("F7",ui->F7));
    ui->F7->setToolTip("F7");
    QObject::connect(ui->F7, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("F8",ui->F8));
    ui->F8->setToolTip("F8");
    QObject::connect(ui->F8, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("F9",ui->F9));
    ui->F9->setToolTip("F9");
    QObject::connect(ui->F9, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("F10",ui->F10));
    ui->F10->setToolTip("F10");
    QObject::connect(ui->F10, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("G1",ui->G1));
    ui->G1->setToolTip("G1");
    QObject::connect(ui->G1, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("G2",ui->G2));
    ui->G2->setToolTip("G2");
    QObject::connect(ui->G2, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("G3",ui->G3));
    ui->G3->setToolTip("G3");
    QObject::connect(ui->G3, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("G4",ui->G4));
    ui->G4->setToolTip("G4");
    QObject::connect(ui->G4, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("G5",ui->G5));
    ui->G5->setToolTip("G5");
    QObject::connect(ui->G5, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("G6",ui->G6));
    ui->G6->setToolTip("G6");
    QObject::connect(ui->G6, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("G7",ui->G7));
    ui->G7->setToolTip("G7");
    QObject::connect(ui->G7, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("G8",ui->G8));
    ui->G8->setToolTip("G8");
    QObject::connect(ui->G8, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("G9",ui->G9));
    ui->G9->setToolTip("G9");
    QObject::connect(ui->G9, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("G10",ui->G10));
    ui->G10->setToolTip("G10");
    QObject::connect(ui->G10, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("H1",ui->H1));
    ui->H1->setToolTip("H1");
    QObject::connect(ui->H1, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("H2",ui->H2));
    ui->H2->setToolTip("H2");
    QObject::connect(ui->H2, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("H3",ui->H3));
    ui->H3->setToolTip("H3");
    QObject::connect(ui->H3, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("H4",ui->H4));
    ui->H4->setToolTip("H4");
    QObject::connect(ui->H4, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("H5",ui->H5));
    ui->H5->setToolTip("H5");
    QObject::connect(ui->H5, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("H6",ui->H6));
    ui->H6->setToolTip("H6");
    QObject::connect(ui->H6, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("H7",ui->H7));
    ui->H7->setToolTip("H7");
    QObject::connect(ui->H7, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("H8",ui->H8));
    ui->H8->setToolTip("H8");
    QObject::connect(ui->H8, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("H9",ui->H9));
    ui->H9->setToolTip("H9");
    QObject::connect(ui->H9, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("H10",ui->H10));
    ui->H10->setToolTip("H10");
    QObject::connect(ui->H10, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);


    EnemySquare.insert(std::pair<QString,QGridSquare *>("I1",ui->I1));
    ui->I1->setToolTip("I1");
    QObject::connect(ui->I1, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("I2",ui->I2));
    ui->I2->setToolTip("I2");
    QObject::connect(ui->I2, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("I3",ui->I3));
    ui->I3->setToolTip("I3");
    QObject::connect(ui->I3, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("I4",ui->I4));
    ui->I4->setToolTip("I4");
    QObject::connect(ui->I4, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("I5",ui->I5));
    ui->I5->setToolTip("I5");
    QObject::connect(ui->I5, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("I6",ui->I6));
    ui->I6->setToolTip("I6");
    QObject::connect(ui->I6, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("I7",ui->I7));
    ui->I7->setToolTip("I7");
    QObject::connect(ui->I7, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("I8",ui->I8));
    ui->I8->setToolTip("I8");
    QObject::connect(ui->I8, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("I9",ui->I9));
    ui->I9->setToolTip("I9");
    QObject::connect(ui->I9, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("I10",ui->I10));
    ui->I10->setToolTip("I10");
    QObject::connect(ui->I10, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("J1",ui->J1));
    ui->J1->setToolTip("J1");
    QObject::connect(ui->J1, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("J2",ui->J2));
    ui->J2->setToolTip("J2");
    QObject::connect(ui->J2, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("J3",ui->J3));
    ui->J3->setToolTip("J3");
    QObject::connect(ui->J3, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("J4",ui->J4));
    ui->J4->setToolTip("J4");
    QObject::connect(ui->J4, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("J5",ui->J5));
    ui->J5->setToolTip("J5");
    QObject::connect(ui->J5, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("J6",ui->J6));
    ui->J6->setToolTip("J6");
    QObject::connect(ui->J6, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("J7",ui->J7));
    ui->J7->setToolTip("J7");
    QObject::connect(ui->J7, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("J8",ui->J8));
    ui->J8->setToolTip("J8");
    QObject::connect(ui->J8, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("J9",ui->J9));
    ui->J9->setToolTip("J9");
    QObject::connect(ui->J9, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    EnemySquare.insert(std::pair<QString,QGridSquare *>("J10",ui->J10));
    ui->J10->setToolTip("J10");
    QObject::connect(ui->J10, &QGridSquare::ChangeOthersClickabilitySig, this, &client::ChangeOthersClickabilitySlot);

    qDebug() << "All Enemy squares on Client Window were associated with their coordinates in QString format";

}
