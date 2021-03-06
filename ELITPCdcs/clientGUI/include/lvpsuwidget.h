#ifndef LVPSUWIDGET_H
#define LVPSUWIDGET_H

#include "DCSProfileWidget.h"
#include "abstractwidget.h"
#include "lvcontroller.h"
#include "open62541/types_elitpcnodeset_generated.h"

struct deviceOutputs {
    bool CH1;
    bool CH2;
    bool Output;
};

namespace Ui {
class LVpsuWidget;
}

class LVpsuWidget : public AbstractWidget {
    Q_OBJECT

  public:
    explicit LVpsuWidget(std::string name, QWidget *parent = 0);
    ~LVpsuWidget();
    lv_controller *LVController;

  private:
    Ui::LVpsuWidget *ui;
    void connectSignals();
    deviceOutputs deviceSettings;
    bool connectionState;
    QString customName[2];
    DCSProfileWidget *profile;

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
    void setOutputON();
    void setOutputOFF();

    void changeNamePressed();
    void setVPressed();
    void setIPressed();

  private:
    std::string getDeviceType() const final { return "HMP2020"; }
};

#endif  // LVPSUWIDGET_H
