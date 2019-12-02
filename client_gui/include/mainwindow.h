#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QComboBox>
#include "opc_client.h"
#include "state.h"
#include "lvpsuwidget.h"
#include "lv4psuwidget.h"
#include "hvpsuwidget.h"
#include "piweatherwidget.h"
#include "tpgwidget.h"
#include "configWrapper.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Json::Value& root, QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent* e);

private:
    Ui::MainWindow *ui;
    opc_client *client;
    stateMachine* statemachine;
    std::vector<QPushButton*> startButtons;
    std::vector<AbstractWidget*> deviceWidgets;
    QComboBox *stateBox;
    void loadWidgets(Json::Value items);
    void connectSignals();
    void buildStateBox();
};
#endif // MAINWINDOW_H
