#ifndef MKS910WIDGET_H
#define MKS910WIDGET_H

#include "DCSRelayWidget.h"
#include "abstractwidget.h"
#include "mks910codes.h"
#include "mks910controller.h"
#include "open62541/types_elitpcnodeset_generated.h"
#include <QComboBox>
#include <QFrame>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLCDNumber>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QStackedWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <vector>

class MKS910Widget : public AbstractWidget {
    Q_OBJECT

  public:
    explicit MKS910Widget(std::string name, QWidget *parent = 0);
    ~MKS910Widget();

  public slots:

    void updateStatus(UA_DataValue *data) override;
    void updateMeasurements(UA_DataValue *data) override;
    void updateConfiguration(UA_DataValue *data) override;
    void updateRelay(UA_DataValue *data);

    void changeNamePressed();
    void changeRelay(int nr, RelayStruct values);
    void changeUnits(int u);

  private:
    std::vector<DCSRelayWidget *> relayWidgets;
    bool connectionState;
    // create layout procedures and variables
    QVBoxLayout *mainLayout;

    QLabel *mUnitLabel;
    QTabWidget *tab;
    QGroupBox *mBox;
    QLCDNumber *mVacuum;
    QLabel *mStatusLabel;
    QLabel *mStatus;
    QLabel *mTemp;
    QPushButton *cNameButton;
    QLabel *cNameLabel;
    QString cCustomName;
    QComboBox *unitsBox;
    QLCDNumber combined;
    QLCDNumber pirani;
    QLCDNumber piezo;

    void createLayout();
    void drawLine();
    void createConnectionSection();
    void createMTab();
    void createCTab();
    void createHTab();
    void createRTab();
    void setChannelName();
    void connectSignals();
    void loadConfig();
    void saveConfig();

    std::string getDeviceType() const final { return "MKS910"; }
};

#endif  // MKS910WIDGET_H
