#ifndef MKS910WIDGET_H
#define MKS910WIDGET_H

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
#include "mks910controller.h"
#include "mks910codes.h"
#include "open62541/types_dcsnodeset_generated.h"
class MKS910Widget : public AbstractWidget
{
    Q_OBJECT

public:
    explicit MKS910Widget(std::string name, QWidget *parent = 0);
    explicit MKS910Widget(std::string name, std::string address, std::string port, QWidget *parent = 0);
    ~MKS910Widget();

public slots:
    void deviceConnect();
    void deviceDisconnect();

    void updateStatus(void *data) override;
    void updateMeasurements(void *data);
    void updateConfiguration(void *data);
    void controllerInit(UA_Client* client,UA_ClientConfig* config ,UA_CreateSubscriptionResponse resp) override;

    void changeNamePressed();
    void updateStatusLabel(QString info);

protected:
    void closeEvent(QCloseEvent* e);

private:
//    std::string instanceName;
    MKS910_controller *controller;
    bool connectionState;
    //create layout procedures and variables
    QVBoxLayout *mainLayout;
    QLabel *statusLabel;
    QLabel *connectionStatus;
    QLineEdit *connectionIP;
    QLineEdit *connectionPort;
    QPushButton *connectButton;
    QPushButton *disconnectButton;
    QLabel* mUnitLabel; 
    QTabWidget* tab;
    QGroupBox* mBox;
    QLCDNumber* mVacuum;
    QLabel* mStatusLabel;
    QLabel* mStatus;
    QPushButton* cNameButton;
    QLabel*  cNameLabel;
    QString cCustomName;

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

#endif // MKS910WIDGET_H
