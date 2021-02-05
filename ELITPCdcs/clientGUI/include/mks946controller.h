#ifndef MKS946_CONTROLLER_H
#define MKS946_CONTROLLER_H
#include "opc_controller.h"
#include "open62541/types_elitpcnodeset_generated.h"
#include "open62541/types_elitpcnodeset_generated_handling.h"
class MKS946_controller : public opc_controller {
    Q_OBJECT
  public:
    MKS946_controller(std::string OName, QObject *parent = 0);
    void callSetRelay(int nr, int enabled, double setpoint, double hysteresis);
    void callConfigureFlow(std::string mode, double setpoint, double nominalRange,
                           double scaleFactor);
    void callConfigurePressure(std::string type, double nominalRange,
                               std::string voltageRange);
    void callConfigurePID(std::string flowChannel, std::string pressureChannel,
                          double pressureSetpoint, double kp, double timeConstant,
                          double derivatibeTomeConstant, double ceiling, double base,
                          double preset, double start, double end, double ctrlStart,
                          std::string direction, int band, int gain);
    void callSetPIDState(bool state);
    void callZeroMFC();
};
#endif  // MKS946_CONTROLLER_H
