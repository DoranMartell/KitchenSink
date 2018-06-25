#ifndef WAITINGWINDOW_H
#define WAITINGWINDOW_H

#include <QDialog>
#include <QDebug>
#include <QCloseEvent>

#include <unistd.h>

#include "globals.h"

namespace Ui {
class WaitingWindow;
}

class WaitingWindow : public QDialog
{
    Q_OBJECT

public:
    explicit WaitingWindow(QWidget *parent = 0);
    ~WaitingWindow();


public slots:

    void ShowWaitingWindowSlot();
    void HideWaitingWindowSlot();
    void CloseFromFillGridSlot();

private:

    Ui::WaitingWindow *ui;

    bool BreakTheLoop;

    void closeEvent(QCloseEvent * event);

};

#endif // WAITINGWINDOW_H
