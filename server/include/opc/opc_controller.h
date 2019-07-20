#ifndef OPC_TEMPLATE_CONTROLLER_H
#define OPC_TEMPLATE_CONTROLLER_H
#include <open62541/server.h>
#include "opc_monitor.h"
#include "../DeviceController.h"

template <class M,class C,class D>
class opc_template_controller :public opc_monitor, public AbstractDeviceController<D>
{
public:
    opc_template_controller(){}
    opc_template_controller(std::string name) : opc_monitor(name){}
    ~opc_template_controller(){}

    // adding abstract device support
    void updateMeasurements(UA_Server *server);
    void updateConfiguration(UA_Server *server);
    void updateStatus(UA_Server *server);
    bool isConnected(){return AbstractDeviceController<D>::device->isConnected();}

private:
    virtual M getMeasurements()=0;
    virtual C getSettings()=0;
    void disconnectDevice(){AbstractDeviceController<D>::disconnect();}
    void connectDevice(TCPConnectionParameters *parameters){AbstractDeviceController<D>::connect(parameters);}

};
#include "opc_controller.hxx"
#endif // OPC_TEMPLATE_CONTROLLER_H
