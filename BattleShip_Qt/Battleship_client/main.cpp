#include "client.h"
#include "askname.h"
#include "askserver.h"
#include "fillgridwindow.h"
#include "waitingwindow.h"
#include "endofgamewindow.h"


#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    client cl;
    askserver serv;
    askname name;
    FillGridWindow grid;
    WaitingWindow ww;
    EndOfGameWindow eogw;

    QObject::connect(&serv, &askserver::ServCredentialsCollectedSig, &cl, &client::ServCredentialsCollectedSlot);
    QObject::connect(&serv, &askserver::showAskNameWindowSig, &name, &askname::showAskNameWindowSlot);
    QObject::connect(&name, &askname::CloseConnAndJoinThreadSig, &cl, &client::CloseConnAndJoinThreadSlot);
    QObject::connect(&name, &askname::showAskServerSig, &serv, &askserver::showAskServerSlot);

    QObject::connect(&name, &askname::SendNameSig, &cl, &client::SendNameSlot);
    QObject::connect(&name, &askname::ShowGridSig, &grid, &FillGridWindow::ShowGridSlot);

    QObject::connect(&grid, &FillGridWindow::ShowMainWindowSig, &cl, &client::ShowMainWindowSlot);
    QObject::connect(&grid, &FillGridWindow::SendBbyeSig, &cl, &client::SendBbyeSlot);
    QObject::connect(&grid, &FillGridWindow::SendFillSig, &cl, &client::SendFillSlot);
    QObject::connect(&grid, &FillGridWindow::CloseConnAndJoinThreadSig, &cl, &client::CloseConnAndJoinThreadSlot);

    QObject::connect(&cl, &client::showAskNameWindowSig, &name, &askname::showAskNameWindowSlot);
    QObject::connect(&cl, &client::hideFillGridWindowSig, &grid, &FillGridWindow::hideFillGridWindowSlot);

    QObject::connect(&cl, &client::Flee, &grid, &FillGridWindow::on_FleeButton_clicked);

    QObject::connect(&grid, &FillGridWindow::ShowWaitingWindowSig, &ww, &WaitingWindow::ShowWaitingWindowSlot);
    //QObject::connect(&grid, &FillGridWindow::HideWaitingWindowSig, &ww, &WaitingWindow::HideWaitingWindowSlot);

    QObject::connect(&grid, &FillGridWindow::CloseWaitingWindowSig, &ww, &WaitingWindow::CloseFromFillGridSlot);

    QObject::connect(&eogw, &EndOfGameWindow::ShowFillTheGridWindowSig, &grid, &FillGridWindow::ShowGridSlot);
    QObject::connect(&cl, &client::showEOGWindowSig, &eogw, &EndOfGameWindow::showEOGWindowSlot);

    QObject::connect(&eogw, &EndOfGameWindow::CloseAllOthersSig, &cl, &client::CloseFromSomewhereSlot);
    QObject::connect(&eogw, &EndOfGameWindow::DisableClientWindowSig, &cl, &client::QWidget::setEnabled);
    QObject::connect(&eogw, &EndOfGameWindow::HideClientWindowSig, &cl, &client::hide);
    QObject::connect(&eogw, &EndOfGameWindow::SendBbyeSig, &cl, &client::SendBbyeSlot);

    serv.show();

    return a.exec();
}
