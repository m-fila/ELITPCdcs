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

    void updateStatus(UA_Variant data) override;
    void updateMeasurements(UA_Variant data);
    void updateConfiguration(UA_Variant data);

    void changeNamePressed();
};

#endif  // PIWEATHER_WIDGET_H
