#ifndef DCS_RELAY_WIDGET_H
#define DCS_RELAY_WIDGET_H

#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <algorithm>
#include <iostream>
#include <map>
#include <string>
enum class RelayDirectionPolicy { Auto, Below, Above };

struct RelayStruct {
    int status;
    int direction;
    int enabled;
    double setpoint;
    double hysteresis;
    std::string unit;
};

class DCSRelayWidget : public QWidget {
    Q_OBJECT
  public:
    explicit DCSRelayWidget(
        size_t nr, RelayDirectionPolicy directionPolicy = RelayDirectionPolicy::Auto,
        QWidget *parent = nullptr);
    void setValues(const RelayStruct &newVal);
    void setEnabledLabels(const std::map<int, std::string> &labels) {
        enabledLabels = labels;
        setValues(value);
    }

  protected:
    const size_t number;
    const RelayDirectionPolicy directionPolicy;
    std::map<int, std::string> enabledLabels = {{0, "OFF"}, {1, "ON"}};
    QLabel *hysteresis;
    QLabel *setpoint;
    QLabel *direction;
    QLabel *enabled;
    QLabel *status;
    QLabel *units;
    QPushButton *setButton;
    RelayStruct value;

    void connectSignals();
  public slots:
    void showDialog();
};

class RelayDialog : public QDialog {
    Q_OBJECT
  public:
    explicit RelayDialog(size_t number, RelayStruct init,
                         const std::map<int, std::string> &labels,
                         RelayDirectionPolicy directionPolicy, QWidget *parent = nullptr);
    RelayStruct getValue();

  private:
    const RelayDirectionPolicy directionPolicy;
    QLabel text;
    QComboBox enabled;
    QLineEdit direction;
    QDoubleSpinBox hysteresis;
    QDoubleSpinBox setpoint;
    QLabel img;
    const std::string unit;
  public slots:
    void updateDirection(double = 0.);
    void updateHysteresis(double = 0.);
};

#endif  // DCS_RELAY_WIDGET_H
