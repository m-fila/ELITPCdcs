#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "eli_client_gui.h"
#include "kled.h"
#include <QLabel>
#include <QMainWindow>
#include <QString>
#include <QTimer>
#include <utility>
#include <vector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = 0);

  private:
    Ui::MainWindow *ui;
    QTimer timer;
    const std::vector<QString> possibleStates = {"stable", "perfect", "fine"};
    const std::vector<std::pair<QString, QLabel *>> items = {{"HV system", new QLabel()},
                                                             {"LV system", new QLabel()},
                                                             {"Gas system", new QLabel()},
                                                             {"Weather", new QLabel()}};
    void createWidgets();
    void createMenu();
    void aboutAction();

  private slots:
    void updateLabels();
};
#endif  // MAINWINDOW_H
