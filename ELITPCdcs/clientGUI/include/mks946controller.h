#ifndef MKS946_CONTROLLER_H
#define MKS946_CONTROLLER_H
#include "opc_controller.h"
class MKS946_controller : public opc_controller {
    Q_OBJECT
  public:
    MKS946_controller(std::string OName, QObject *parent = 0);
    void callSetUnits(int units);
    void callSetRelay(int nr, int enabled, double setpoint, double hysteresis);
    void opcInit(UA_Client *client, UA_ClientConfig *config,
                 UA_CreateSubscriptionResponse response) override {
        opc_controller::opcInit(client, config, response);
        addMonitoredItem(browsedIds["relay"], response, RelayChangedCallback);
    }

  signals:
    void relayChanged(void *);

  private:
    const std::string setUnitsBrowseName;

    static void RelayChangedCallback(UA_Client *client, UA_UInt32 subId, void *subContext,
                                     UA_UInt32 monId, void *monContext,
                                     UA_DataValue *value);
};

#endif  // HV_CONTROLLER_H
