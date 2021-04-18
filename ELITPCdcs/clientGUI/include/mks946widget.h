#ifndef MKS946WIDGET_H
#define MKS946WIDGET_H

#include "DCSInputDialog.h"
#include "DCSRelayWidget.h"
#include "abstractwidget.h"
#include "mks946codes.h"
#include "mks946controller.h"
#include "open62541/types_elitpcnodeset_generated.h"
#include <QComboBox>
#include <QFrame>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLCDNumber>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTabWidget>
#include <QVBoxLayout>
#include <vector>
class MKS946Widget : public AbstractWidget {
    Q_OBJECT

  public:
    explicit MKS946Widget(std::string name, QWidget *parent = 0);
    ~MKS946Widget();

  public slots:
    void updateStatus(UA_Variant data) override;
    void updateMeasurements(UA_Variant data) override;
    void updateConfiguration(UA_Variant data) override;
    void updateRelay(UA_Variant data);
    void updatePID(UA_Variant data);
    void updatePIDState(UA_Variant data);

    void changeNamePressed();
    void changeRelay(int nr, RelayStruct values);
  private slots:
    void showPIDDialog();
    void showFlowDialog();
    void showPressureDialog();
    void zeroMFC();
    void setPIDState();

  private:
    std::vector<DCSRelayWidget *> relayWidgets;
    bool connectionState;
    // create layout procedures and variables
    QVBoxLayout *mainLayout;

    QLabel *mUnitLabel;
    QTabWidget *tab;
    QLCDNumber *mVacuum;
    QLCDNumber *mFlow;
    QPushButton *cNameButton;
    QLabel *cNameLabel;
    QString cCustomName;

    QLabel flowMode;
    QLabel flowSetPoint;
    QLabel flowNominalRange;
    QLabel flowScaleFactor;
    QPushButton *flowButton;
    QLabel manometerType;
    QLabel manometerNominalRange;
    QLabel manometerVoltageRange;
    QPushButton *manometerButton;

    QLabel PIDUnits;
    QLabel PIDRecipe;
    QLabel PIDFlowChannel;
    QLabel PIDPressureChannel;
    QLabel PIDPresssureSetPoint;
    QLabel PIDKp;
    QLabel PIDTimeConstant;
    QLabel PIDDerivativeTimeConstant;
    QLabel PIDCeiling;
    QLabel PIDBase;
    QLabel PIDPreset;
    QLabel PIDStart;
    QLabel PIDEnd;
    QLabel PIDCtrlStart;
    QLabel PIDDirection;
    QLabel PIDBand;
    QLabel PIDGain;
    QPushButton *PIDButton;

    QPushButton *PIDStateButtonON;
    QPushButton *PIDStateButtonOFF;
    QLabel *PIDStateLabel;
    QPushButton *zeroMFCButton;

    void createLayout();
    void drawLine();
    void createConnectionSection();
    void createMTab();
    void createCTab();
    void createPIDTab();
    void createRTab();
    void setChannelName();
    void connectSignals();
    void loadConfig();
    void saveConfig();

    void fillGrid(std::vector<std::pair<std::string, QWidget *>> names, QGridLayout *grid,
                  size_t grid_max = 100);

    std::string getDeviceType() const final { return "MKS946"; }
};

#endif  // MKS946WIDGET_H
