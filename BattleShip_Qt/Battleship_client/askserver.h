#ifndef ASKSERVER_H
#define ASKSERVER_H

#include <QDialog>
#include <QDebug>
#include <QCloseEvent>

#include "globals.h"

namespace Ui {
class askserver;
}

class askserver : public QDialog
{
    Q_OBJECT

public:
    explicit askserver(QWidget *parent = 0);
    ~askserver();

public slots:

    void showAskServerSlot();

signals:

    /* notify main thread/window that user entered server's address,
            try to connect to it */
    void ServCredentialsCollectedSig(QString , int );

    void showAskNameWindowSig();

private slots:
    void on_ConnectButton_clicked();

    void on_CancelButton_clicked();

private:
    Ui::askserver *ui;

    void closeEvent(QCloseEvent * event);
};

#endif // ASKSERVER_H
