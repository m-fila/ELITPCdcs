#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "eli_client_gui.h"
#include "hvpsuwidget.h"
#include "kled.h"
#include "lv4psuwidget.h"
#include "lvpsuwidget.h"
#include "mks910widget.h"
#include "mks946widget.h"
#include "piweatherwidget.h"
#include "state.h"
#include "tpgwidget.h"
#include <QComboBox>
#include <QMainWindow>
#include <QPushButton>
#include <json.hpp>

using json = nlohmann::json;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    MainWindow(json &config, QWidget *parent = 0);
    ~MainWindow();

  protected:
    void closeEvent(QCloseEvent *e);

  private:
    Ui::MainWindow *ui;
    eli_client *client;
    stateMachine *statemachine;
    std::vector<QPushButton *> startButtons;
    std::vector<KLed *> controlLeds;
    std::vector<AbstractWidget *> deviceWidgets;
    QComboBox *stateBox;
    void loadWidgets(json &items);
    void connectSignals();
    void buildStateBox();
    void buildMenu();

  private slots:
    void connectionStatusChanged(bool);
    void aboutAction();

  signals:
    void closeConnectionAlert();
};
#endif  // MAINWINDOW_H
