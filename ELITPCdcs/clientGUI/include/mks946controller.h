#ifndef MKS946_CONTROLLER_H
#define MKS946_CONTROLLER_H
#include "opc_controller.h"
class MKS946_controller : public opc_controller {
    Q_OBJECT
  public:
    MKS946_controller(std::string OName, QObject *parent = 0);
    void callSetRelay(int nr, int enabled, double setpoint, double hysteresis);
};
#endif  // MKS946_CONTROLLER_H
