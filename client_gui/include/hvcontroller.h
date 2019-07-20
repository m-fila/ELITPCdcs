#ifndef HV_CONTROLLER_H
#define HV_CONTROLLER_H
#include "opc_controller.h"
#include "../common/HMPDataType.h"
class hv_controller : public opc_controller
{
    Q_OBJECT
public:
    hv_controller(std::string OName,QObject *parent=0);
//    void callSetChannel(int nr, bool state);
//    void callSetOutput(bool state);
};

#endif // HV_CONTROLLER_H
