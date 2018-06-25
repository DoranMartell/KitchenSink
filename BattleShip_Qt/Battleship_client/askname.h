#ifndef ASKNAME_H
#define ASKNAME_H

#include <QDialog>
#include <QCloseEvent>
#include <QMessageBox>
#include <QDebug>

#include <unistd.h>
#include "globals.h"

#include <vector>
#include <QTime>

namespace Ui {
class askname;
}

class askname : public QDialog
{
    Q_OBJECT

public:
    explicit askname(QWidget *parent = 0);
    ~askname();

signals:

    void SendNameSig(QString ); /* client class is responsible for sending messages */

    void ShowGridSig();

    void CloseConnAndJoinThreadSig();

    void showAskServerSig();

public slots:

    void showAskNameWindowSlot();

private slots:

    void on_CancelButton_clicked();

    void on_ConfirmButton_clicked();

    void on_RandButton_clicked();

private:
    Ui::askname *ui;

    void closeEvent(QCloseEvent * event);

    QString RandomlyChooseName();

    std::vector <QString> RandomNames = {"Admiral Ackbar", "Devon Goldam", "Allyn Fera",
                                         "Aran Nomante", "Kyp Astar", "Rayce Farelle",
                                         "Aran Gunnar", "Nom Pulastra", "Jori Carver",
                                         "Scur Jal'Daan", "Ash Ulrand", "Tighe Bodhala",
                                         "Thrak Gorshun", "Lan Olesa", "Doran Krotan",
                                         "Jacen Qe-Cora"};
};

#endif // ASKNAME_H
