#ifndef LVPSUWIDGET_H
#define LVPSUWIDGET_H

#include "abstractwidget.h"
#include "lvcontroller.h"

namespace Ui {
class LVpsuWidget;
}

class LVpsuWidget : public AbstractWidget
{
    Q_OBJECT

public:
    explicit LVpsuWidget(std::string name,QWidget *parent = 0);
    explicit LVpsuWidget(std::string name, std::string address, std::string port, QWidget* parent=0);
//    LVpsuWidget(std::string name);
    ~LVpsuWidget();
    lv_controller *LVController;
private:
    Ui::LVpsuWidget *ui;
    void connectSignals();
    HMPMeasurements deviceSettings;
    bool connectionState;
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
    void setOutputON();
    void setOutputOFF();

};

#endif // LVPSUWIDGET_H
