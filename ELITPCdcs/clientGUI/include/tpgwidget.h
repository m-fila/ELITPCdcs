#ifndef TPGWIDGET_H
#define TPGWIDGET_H

#include "abstractwidget.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QFrame>
#include <QLCDNumber>
#include <QTabWidget>
#include <vector>
#include "tpgcontroller.h"
#include "tpg362codes.h"
#include "open62541/types_dcsnodeset_generated.h"
class TPGWidget : public AbstractWidget
{
    Q_OBJECT

public:
    explicit TPGWidget(std::string name, QWidget *parent = 0);
    explicit TPGWidget(std::string name, std::string address, std::string port, QWidget *parent = 0);
    ~TPGWidget();

public slots:
    void deviceConnect();
    void deviceDisconnect();

    void updateStatus(void *data) override;
    void updateMeasurements(void *data);
    void updateConfiguration(void *data);
    void controllerInit(UA_Client* client,UA_ClientConfig* config ,UA_CreateSubscriptionResponse resp) override;

    void changeNamePressed();
    void updateStatusLabel(QString info);

private:
//    std::string instanceName;
    tpg_controller *controller;
    bool connectionState;
    //create layout procedures and variables
    QVBoxLayout *mainLayout;
    QLabel *statusLabel;
    QLabel *connectionStatus;
    QLineEdit *connectionIP;
    QLineEdit *connectionPort;
    QPushButton *connectButton;
    QPushButton *disconnectButton;

    QTabWidget* tab;
    QGroupBox* mBox[2];
    QLCDNumber* mVacuum[2];
    QLabel* mStatusLabel[2];
    QLabel* mStatus[2];
    QPushButton* cNameButton[2];
    QLabel*  cNameLabel[2];
    QString cCustomName[2];

    void createLayout();
    void drawLine();
    void createConnectionSection();
    void createMTab();
    void createCTab();
    void createHTab();
    void setChannelName(int channelno);
    void setChannelsNames();
    void connectSignals();
    void loadConfig();
    void saveConfig();
};

#endif // TPGWIDGET_H
