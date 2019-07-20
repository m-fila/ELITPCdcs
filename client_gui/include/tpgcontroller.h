#ifndef TPG_CONTROLLER_H
#define TPG_CONTROLLER_H
#include "opc_controller.h"
#include "../common/HMPDataType.h"
class tpg_controller : public opc_controller
{
    Q_OBJECT
public:
    tpg_controller(std::string OName,QObject *parent=0);
//    void callSetChannel(int nr, bool state);
//    void callSetOutput(bool state);
};

#endif // TPG_CONTROLLER_H
