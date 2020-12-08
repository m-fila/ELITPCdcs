#ifndef ABSTRACTWIDGET_H
#define ABSTRACTWIDGET_H

#include "TCPWidget.h"
#include "basewidget.h"
#include "opc_controller.h"
#include "open62541/types_dcsnodeset_generated.h"
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
    virtual void saveConfig() {}
    virtual void loadConfig() {}
    virtual void connectSignals();
  public slots:
    virtual void updateStatus(void *data);
    virtual void updateMeasurements(void *data) {}
    virtual void updateConfiguration(void *data) {}
    virtual void updateConnectionParameters(void *data);
    virtual void controllerInit(UA_Client *client, UA_ClientConfig *config,
                                UA_CreateSubscriptionResponse resp) override {
        controller->opcInit(client, config, resp);
    }
};

#endif  // ABSTRACTWIDGET_H
