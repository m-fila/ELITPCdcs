#ifndef MKS946_CONTROLLER_H
#define MKS946_CONTROLLER_H
#include "opc_controller.h"
class MKS946_controller : public opc_controller {
    Q_OBJECT
  public:
    MKS946_controller(std::string OName, QObject *parent = 0);
    void opcInit(UA_Client *client, UA_ClientConfig *config,
                 UA_CreateSubscriptionResponse response) override {
        opc_controller::opcInit(client, config, response);
    }
};
#endif  // MKS946_CONTROLLER_H
