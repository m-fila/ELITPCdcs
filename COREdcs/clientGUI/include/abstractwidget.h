#ifndef ABSTRACTWIDGET_H
#define ABSTRACTWIDGET_H

#include "TCPWidget.h"
#include "basewidget.h"
#include "opc_controller.h"
#include "open62541/types_dcsnodeset_generated.h"
#include <QLabel>
#include <QSettings>
#include <QWidget>
#include <string>
class KLed;
class AbstractWidget : public BaseWidget {
    Q_OBJECT
  public:
    explicit AbstractWidget(opc_controller *controller, std::string,
                            bool horizontalTpcPanel = false, QWidget *parent = 0);

  protected:
    opc_controller *controller;
    TCPWidget *tcp;
    QLabel deviceInfoLabel{"..."};
    virtual void saveConfig() {}
    virtual void loadConfig() {}
    virtual void connectSignals();
  public slots:
    virtual void updateStatus(UA_Variant data);
    virtual void updateMeasurements(UA_Variant data) {}
    virtual void updateConfiguration(UA_Variant data) {}
    virtual void updateConnectionParameters(UA_Variant data);
    virtual void updateDeviceInfo(UA_Variant data);

    virtual void controllerInit(UA_Client *client, UA_ClientConfig *config,
                                UA_CreateSubscriptionResponse resp) override {
        controller->opcInit(client, config, resp);
    }
};

#endif  // ABSTRACTWIDGET_H
