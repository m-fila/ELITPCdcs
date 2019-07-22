#ifndef LV_CONTROLLER_H
#define LV_CONTROLLER_H
#include "opc_controller.h"
#include "../common/HMPDataType.h"
class lv_controller : public opc_controller
{
    Q_OBJECT
public:
    lv_controller(std::string OName,QObject *parent=0);
    void callSetChannel(int nr, bool state);
    void callSetOutput(bool state);
private:
    UA_NodeId setChannelNodeId;
    UA_NodeId setOutputNodeId;
    void browseIds();
};

#endif // LV_CONTROLLER_H
