#ifndef PIWEATHER_WIDGET_H
#define PIWEATHER_WIDGET_H
#include "abstractwidget.h"
#include "open62541/types_elitpcnodeset_generated.h"
#include "piweathercontroller.h"

namespace Ui {
class PiWeatherWidget;
}

class PiWeatherWidget : public AbstractWidget {
    Q_OBJECT

  public:
    explicit PiWeatherWidget(std::string name, QWidget *parent = 0);
    ~PiWeatherWidget();

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

    void updateStatus(void *data) override;
    void updateMeasurements(void *data);
    void updateConfiguration(void *data);
    //   void controllerInit(UA_Client *client, UA_ClientConfig *config,
    //                       UA_CreateSubscriptionResponse resp) override;
    void changeNamePressed();
};

#endif  // PIWEATHER_WIDGET_H
