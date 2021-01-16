#ifndef MKS946WIDGET_H
#define MKS946WIDGET_H

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

    void changeNamePressed();
    void changeRelay(int nr, RelayStruct values);

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
    QLCDNumber *mFlow;
    QPushButton *cNameButton;
    QLabel *cNameLabel;
    QString cCustomName;
    QComboBox *unitsBox;

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
};

#endif  // MKS946WIDGET_H
