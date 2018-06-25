#ifndef ASKPORT_H
#define ASKPORT_H

#include <QDialog>
#include <QDebug>
#include <QCloseEvent>

#include "globals.h"

namespace Ui {
class AskPort;
}

class AskPort : public QDialog
{
    Q_OBJECT

public:

    explicit AskPort(QWidget *parent = 0);
    ~AskPort();

signals:

    void setPortNumSig(QString );
    void startServAndShowSig();

private slots:

    void on_ChooseButton_clicked();

    void on_CancelButton_clicked();

private:

    Ui::AskPort *ui;
    void closeEvent(QCloseEvent * );

};

#endif // ASKPORT_H
