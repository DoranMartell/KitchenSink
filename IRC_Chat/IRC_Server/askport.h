#ifndef ASKPORT_H
#define ASKPORT_H

#include <QDialog>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <QMessageBox>
#include <QDebug>

namespace Ui {
class AskPort;
}

class AskPort : public QDialog
{
    Q_OBJECT

public:
    explicit AskPort(QWidget *parent = 0);
    ~AskPort();

    QString userPort; /* port number user specified */

public slots:

    void ShowAskPortAgain(QString ); /* argument is a string with message "why I had to open this window again" */

signals:
    void ShowMainWindow();
    void SetServerPort(int );

private slots:

    void on_CancelButton_clicked();

    void on_OkButton_clicked();

private:
    Ui::AskPort *ui;
};

#endif // ASKPORT_H
