#ifndef HVPSUWIDGET_H
#define HVPSUWIDGET_H

#include "abstractwidget.h"
#include <QGroupBox>
#include <QLCDNumber>
#include <QPushButton>
#include <QRadioButton>
#include <QLabel>
#include "hvcontroller.h"
#include "kled.h"

#include "open62541/types_dcsnodeset_generated.h"
namespace Ui {
class HVpsuWidget;
}

class HVpsuWidget : public AbstractWidget
{
    Q_OBJECT

public:
    explicit HVpsuWidget(std::string name, QWidget *parent = 0);
    explicit HVpsuWidget(std::string name, std::string address, std::string port,QWidget* parent=0);
    ~HVpsuWidget();
    hv_controller *HVController;
public slots:
    void deviceConnect();
    void deviceDisconnect();

    void updateStatus(void *data);
    void updateMeasurements(void *data);
    void updateConfiguration(void *data);
    void controllerInit(UA_Client*,UA_ClientConfig*,UA_CreateSubscriptionResponse);


    void onPressed();
    void offPressed();
    void setVPressed();
    void changeNamePressed();
    void updateStatusLabel(QString info);
protected:
    void closeEvent(QCloseEvent* e);

private:
    Ui::HVpsuWidget *ui;
    QString CHxCustomName[9];

    //create layout procedures and variables
    QGroupBox *allTabCHx[9];
    QLCDNumber *allTabCHvoltage[9];
    KLed *allTabLed[9];
    QPushButton *allTabKill[9];
    QRadioButton *allTabOn[9];
    QRadioButton *allTabOff[9];
    QLabel *allTabVset[9];
    QPushButton *allTabSetV[9];
    QLabel *allTabImon[9];
    QLabel *allTabIset[9];

    //tab CH x containers;
    QWidget *tabCHx[9];
    QLabel *tabCHxCustomName[9];
    QPushButton *tabCHxChangeName[9];
    QLabel *tabCHxSTATUS[8];
    QPushButton *tabCHxSetVMAX[8];
    QLabel *tabCHxVMAX[8];
    QPushButton *tabCHxSetRUP[8];
    QLabel *tabCHxRUP[8];
    QPushButton *tabCHxSetRDWN[8];
    QLabel *tabCHxRDWN[8];

    bool isRemotePrevious;
    bool initialUpdate;
    bool connectionState;

    void loadConfig();
    void saveConfig();
    void setChannelName(int channelno);
    void setChannelsNames();
    void createAllChannelsTab();
    void createChannelTabs();
    void drawLine(QLayout *layout);
    void connectSignals();
};

#endif // HVPSUWIDGET_H
