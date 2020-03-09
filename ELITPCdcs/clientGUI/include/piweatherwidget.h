#ifndef PIWEATHER_WIDGET_H
#define PIWEATHER_WIDGET_H
#include "abstractwidget.h"
#include "piweathercontroller.h"
#include "open62541/types_dcsnodeset_generated.h"


namespace Ui {
class PiWeatherWidget;
}

class PiWeatherWidget : public AbstractWidget
{
    Q_OBJECT

public:
    explicit PiWeatherWidget(std::string name,QWidget *parent = 0);
    explicit PiWeatherWidget(std::string name, std::string address, std::string port, QWidget* parent=0);
    ~PiWeatherWidget();
    piweather_controller *controller;
private:
    Ui::PiWeatherWidget *ui;
    void connectSignals();
    bool connectionState;
    QString customName[4];

    void loadConfig();
    void saveConfig();
    void setChannelsNames();
    void setChannelName(int channelno);

public slots:

    void deviceConnect();
    void deviceDisconnect();
    void updateStatus(void* data) override;
    void updateMeasurements(void* data);
    void updateConfiguration(void* data);
    void controllerInit(UA_Client* client,UA_ClientConfig* config ,UA_CreateSubscriptionResponse resp) override;
    void changeNamePressed();
};


#endif // PIWEATHER_WIDGET_H
