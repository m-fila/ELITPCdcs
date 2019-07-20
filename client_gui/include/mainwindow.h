#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "opc_client.h"
#include "state.h"
#include "lvpsuwidget.h"
#include "hvpsuwidget.h"
#include "tpgwidget.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent* e);

private slots:
    void startLVPSU();
    void startHVPSU();
    void startPM1();
    void startPM2();

private:
    Ui::MainWindow *ui;
    opc_client *client;
    stateMachine* statemachine;
    LVpsuWidget *lvPSU;
    HVpsuWidget *hvPSU;
    TPGWidget *pm1;
    TPGWidget *pm2;

    void connectSignals();


};

#endif // MAINWINDOW_H
