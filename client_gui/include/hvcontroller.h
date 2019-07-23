#ifndef HV_CONTROLLER_H
#define HV_CONTROLLER_H
#include "opc_controller.h"
#include "../common/DTmDataType.h"
#include "../common/DTcDataType.h"
class hv_controller : public opc_controller
{
    Q_OBJECT
public:
    hv_controller(std::string OName,QObject *parent=0);
    void callSetChannel(int nr, bool state);
    void callSetVoltage(int nr, double voltage);
private:
    UA_NodeId setChannelNodeId;
    UA_NodeId setVoltageNodeId;
    void browseIds();
};

#endif // HV_CONTROLLER_H
