#ifndef HV_CONTROLLER_H
#define HV_CONTROLLER_H
#include "opc_controller.h"
class hv_controller : public opc_controller {
    Q_OBJECT
  public:
    hv_controller(std::string OName, QObject *parent = 0);
    void callSetChannel(int nr, bool state);
    void callSetVoltage(int nr, double voltage);
    void callSetVoltageMax(int nr, double voltage);
    void callSetRampUp(int nr, double rup);
    void callSetRampDown(int nr, double rdwn);
    void callSetCurrent(int nr, double current);

  private:
    const std::string setChannelBrowseName;
    const std::string setVoltageBrowseName;
    const std::string setVoltageMaxBrowseName;
    const std::string setRampUpBrowseName;
    const std::string setRampDownBrowseName;
    const std::string setCurrentBrowseName;
};

#endif  // HV_CONTROLLER_H
