#ifndef HV_CONTROLLER_H
#define HV_CONTROLLER_H
#include "opc_controller.h"
class hv_controller : public opc_controller
{
    Q_OBJECT
public:
    hv_controller(std::string OName,QObject *parent=0);
    void callSetChannel(int nr, bool state);
    void callSetVoltage(int nr, double voltage);
    void callSetVoltageMax(int nr, double voltage);
    void callSetRampUp(int nr, double rup);
    void callSetRampDown(int nr, double rdwn);
private:
    const std::string setChannelBrowseName;
    const std::string setVoltageBrowseName;
    const std::string setVoltageMaxBrowseName;
    const std::string setRampUpBrowseName;
    const std::string setRampDownBrowseName;
//    UA_NodeId setChannelNodeId;
//    UA_NodeId setVoltageNodeId;
//    UA_NodeId setVoltageMaxNodeId;
//    UA_NodeId setRampUpNodeId;
//    UA_NodeId setRampDownNodeId;
//    void browseIds();
};

#endif // HV_CONTROLLER_H
