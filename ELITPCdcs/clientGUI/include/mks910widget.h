#ifndef MKS910WIDGET_H
#define MKS910WIDGET_H

#include "DCSRelayWidget.h"
#include "abstractwidget.h"
#include "mks910codes.h"
#include "mks910controller.h"
#include "open62541/types_dcsnodeset_generated.h"
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
class MKS910Widget : public AbstractWidget {
    Q_OBJECT

  public:
    explicit MKS910Widget(std::string name, QWidget *parent = 0);
    explicit MKS910Widget(std::string name, std::string address, std::string port,
                          QWidget *parent = 0);
    ~MKS910Widget();

  public slots:
    void deviceConnect();
    void deviceDisconnect();

    void updateStatus(void *data) override;
    void updateMeasurements(void *data);
    void updateConfiguration(void *data);
    void controllerInit(UA_Client *client, UA_ClientConfig *config,
                        UA_CreateSubscriptionResponse resp) override;

    void changeNamePressed();
    void updateStatusLabel(QString info);

    void changeUnits(int u);

  private:
    //    std::string instanceName;
    MKS910_controller *controller;
    bool connectionState;
    // create layout procedures and variables
    QVBoxLayout *mainLayout;
    QLabel *statusLabel;

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

#endif  // MKS910WIDGET_H
