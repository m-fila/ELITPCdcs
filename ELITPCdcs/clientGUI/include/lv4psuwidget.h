#ifndef LV4PSUWIDGET_H
#define LV4PSUWIDGET_H

#include "abstractwidget.h"
#include "lvcontroller.h"
#include "open62541/types_dcsnodeset_generated.h"

struct device4Outputs{
    bool CH1;
    bool CH2;
    bool CH3;
    bool CH4;
    bool Output;
};

namespace Ui {
class LV4psuWidget;
}

class LV4psuWidget : public AbstractWidget
{
    Q_OBJECT

public:
    explicit LV4psuWidget(std::string name,QWidget *parent = 0);
    explicit LV4psuWidget(std::string name, std::string address, std::string port, QWidget* parent=0);
//    LV4psuWidget(std::string name);
    ~LV4psuWidget();
    lv_controller *LVController;
private:
    Ui::LV4psuWidget *ui;
    void connectSignals();
    device4Outputs deviceSettings;
    bool connectionState;
    QString customName[4];

    void loadConfig();
    void saveConfig();
    void setChannelsNames();
    void setChannelName(int channelno);
protected:
    void closeEvent(QCloseEvent* e);
public slots:

    void deviceConnect();
    void deviceDisconnect();
    void updateStatus(void* data);
    void updateMeasurements(void* data);
    void updateConfiguration(void* data);
    void controllerInit(UA_Client* client,UA_ClientConfig* config ,UA_CreateSubscriptionResponse resp);

    void setCH1ON();
    void setCH1OFF();
    void setCH2ON();
    void setCH2OFF();
    void setCH3ON();
    void setCH3OFF();
    void setCH4ON();
    void setCH4OFF();
    void setOutputON();
    void setOutputOFF();
    
    void changeNamePressed();
    void setVPressed();
    void setIPressed();
};

#endif // LV4PSUWIDGET_H
