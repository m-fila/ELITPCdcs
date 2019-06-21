#ifndef OPC_TEMPLATE_CONTROLLER_H
#define OPC_TEMPLATE_CONTROLLER_H
#include <open62541/server.h>
#include "opc_monitor.h"
#include "../DeviceController.h"

template <class T,class D>
class opc_template_controller :public opc_monitor, public AbstractDeviceController<D>
{
public:
    opc_template_controller(){}
    opc_template_controller(std::string name) : opc_monitor(name){}
    ~opc_template_controller(){}

    virtual void addCustomDataType(UA_Server *server, UA_ServerConfig *config)=0;
    void updateMeasurements(UA_Server *server);
    void updateConfiguration(UA_Server *server);
    void updateStatus(UA_Server *server);

    // adding abstract device support
    void disconnectDevice(){AbstractDeviceController<D>::disconnect();}
    void connectDevice(TCPConnectionParameters *parameters){AbstractDeviceController<D>::connect(parameters);}
    bool isConnected();

private:
    virtual T getMeasurements()=0;
    virtual T getSettings()=0;
};
#include "opc_controller.hxx"
#endif // OPC_TEMPLATE_CONTROLLER_H
