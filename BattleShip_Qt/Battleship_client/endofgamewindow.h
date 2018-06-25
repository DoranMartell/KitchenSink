#ifndef ENDOFGAMEW_H
#define ENDOFGAMEW_H

#include <QDialog>
#include <QDebug>
#include <QCloseEvent>

#include "globals.h"

namespace Ui {
class EndOfGameWindow;
}

class EndOfGameWindow : public QDialog
{
    Q_OBJECT

public:
    explicit EndOfGameWindow(QWidget *parent = 0);
    ~EndOfGameWindow();

public slots:

    void showEOGWindowSlot(QString , QString);

private slots:

    void on_EnoughButton_clicked();

    void on_RepeatButton_clicked();

signals:

    void ShowFillTheGridWindowSig();

    void CloseAllOthersSig();

    void DisableClientWindowSig(bool );

    void HideClientWindowSig();

    void SendBbyeSig();

private:

    Ui::EndOfGameWindow *ui;

    void closeEvent(QCloseEvent *event);

};

#endif // ENDOFGAMEW_H
