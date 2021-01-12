#ifndef TPG_CONTROLLER_H
#define TPG_CONTROLLER_H
#include "opc_controller.h"
class tpg_controller : public opc_controller {
    Q_OBJECT
  public:
    tpg_controller(std::string OName, QObject *parent = 0);
    void callSetRelay(int nr, int enabled, double setpoint, double hysteresis);
};

#endif  // TPG_CONTROLLER_H
