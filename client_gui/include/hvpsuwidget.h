#ifndef HVPSUWIDGET_H
#define HVPSUWIDGET_H

#include <QGroupBox>
#include <QLCDNumber>
#include <QPushButton>
#include <QRadioButton>
#include <QLabel>
#include <map>
#include "abstractwidget.h"
#include "hvcontroller.h"
#include "kled.h"
#include "../common/dt1415etchannelstatus.h"
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

    void allOnPressed();
    void allOffPressed();
    void onPressed();
    void offPressed();
    void setVPressed();
    void setVMAXPressed();
    void setRUPPressed();
    void setRDWNPressed();
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
    QLabel *allTabVset[9];
    QPushButton *allTabSetV[9];
    QLabel *allTabImon[9];
    QLabel *allTabIset[9];
    QRadioButton *allTabOn[8];
    QRadioButton *allTabOff[8];
    QPushButton *allOn;
    QPushButton *allOff;
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
    //tab CH x measurements panel
    QLCDNumber *tabCHxvoltage[8];
    KLed *tabCHxLed[8];
    QPushButton *tabCHxKill[8];
    QRadioButton *tabCHxOn[8];
    QRadioButton *tabCHxOff[8];
    QLabel *tabCHxVset[8];
    QPushButton *tabCHxSetV[8];
    QLabel *tabCHxImon[8];


    bool isRemote;
  //  bool initialUpdate;
    bool connectionState;
    bool enabled[8]={false,false,false,false,false,false,false,false};
    bool ON[8]={false,false,false,false,false,false,false,false};
    void loadConfig();
    void saveConfig();
    void setChannelName(int channelno);
    void setChannelsNames();
    void createAllChannelsTab();
    void createChannelTabs();
    void drawLine(QLayout *layout);
    void connectSignals();
    std::string status_translate(DT1415ETchannelStatus status);
};

extern std::map<DT1415ETchannelStatus,std::string> enum_names;

#endif // HVPSUWIDGET_H
