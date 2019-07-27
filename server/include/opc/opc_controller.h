#ifndef OPC_TEMPLATE_CONTROLLER_H
#define OPC_TEMPLATE_CONTROLLER_H
#include <open62541/server.h>
#include "opc_monitor.h"
#include "../DeviceController.h"

template <class M,class C,class D>
class opc_template_controller :public opc_monitor, public AbstractDeviceController<D>
{
public:
    opc_template_controller(std::string name) : opc_monitor(name){}
//    virtual ~opc_template_controller(){}

private:
    virtual M getMeasurements()=0;
    virtual C getSettings()=0;
    void disconnectDevice(){AbstractDeviceController<D>::disconnect();}
    void connectDevice(TCPConnectionParameters *parameters){AbstractDeviceController<D>::connect(parameters);}

protected:
    bool isConnected(){return AbstractDeviceController<D>::device->isConnected();}
    void updateMeasurements(UA_Server *server);
    void updateConfiguration(UA_Server *server);
    void updateStatus(UA_Server *server);
    M measurements;
    C config;
};
#include "opc_controller.hxx"
#endif // OPC_TEMPLATE_CONTROLLER_H
