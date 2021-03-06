#ifndef LV4PSUWIDGET_H
#define LV4PSUWIDGET_H

#include "abstractwidget.h"
#include "lvcontroller.h"
#include "open62541/types_elitpcnodeset_generated.h"

struct device4Outputs {
    bool CH1;
    bool CH2;
    bool CH3;
    bool CH4;
    bool Output;
};

namespace Ui {
class LV4psuWidget;
}

class LV4psuWidget : public AbstractWidget {
    Q_OBJECT

  public:
    explicit LV4psuWidget(std::string name, QWidget *parent = 0);
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

  public slots:

    void updateStatus(UA_DataValue *data) override;
    void updateMeasurements(UA_DataValue *data) override;
    void updateConfiguration(UA_DataValue *data) override;

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

  private:
    std::string getDeviceType() const final { return "HMP4040"; }
};

#endif  // LV4PSUWIDGET_H
