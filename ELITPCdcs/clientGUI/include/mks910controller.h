#ifndef MKS910_CONTROLLER_H
#define MKS910_CONTROLLER_H
#include "opc_controller.h"
class MKS910_controller : public opc_controller {
    Q_OBJECT
  public:
    MKS910_controller(std::string OName, QObject *parent = 0);
    void callSetUnits(int units);
    void callSetRelay(int nr, int enabled, double setpoint, double hysteresis);

  private:
    const std::string setUnitsBrowseName = "setunits";
};

#endif  // MKS910_CONTROLLER_H
