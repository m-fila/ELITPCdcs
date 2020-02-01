#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QComboBox>
#include <json.hpp>
#include "eli_client_gui.h"
#include "state.h"
#include "lvpsuwidget.h"
#include "lv4psuwidget.h"
#include "hvpsuwidget.h"
#include "piweatherwidget.h"
#include "tpgwidget.h"

using json = nlohmann::json;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(json& config, QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent* e);

private:
    Ui::MainWindow *ui;
    eli_client *client;
    stateMachine* statemachine;
    std::vector<QPushButton*> startButtons;
    std::vector<AbstractWidget*> deviceWidgets;
    QComboBox *stateBox;
    void loadWidgets(json &items);
    void connectSignals();
    void buildStateBox();
};
#endif // MAINWINDOW_H
