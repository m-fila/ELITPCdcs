#ifndef OPC_TEMPLATE_CONTROLLER_H
#define OPC_TEMPLATE_CONTROLLER_H
#include <open62541/server.h>

#include <open62541/client_subscriptions.h>
#include "opc_monitor.h"
#include "../DeviceController.h"
//#include <open62541/plugin/log_stdout.h>
template <class T,class D>
class opc_template_controller :public opc_monitor, public AbstractDeviceController<D>
{
public:
    opc_template_controller(){}
    ~opc_template_controller(){}
    bool isConnected();
    virtual T getMeasurements()=0;
    virtual void addVariable(UA_Server *server)=0;
    void addVariable(UA_Server *server,T variable);


    void updateVariable(UA_Server *server);
    void disconnectDevice(){AbstractDeviceController<D>::disconnect();}
    void connectDevice(TCPConnectionParameters *parameters){AbstractDeviceController<D>::connect(parameters);}
     static void dataChangeNotificationCallback(UA_Server *server, UA_UInt32 monitoredItemId,
                                   void *monitoredItemContext, const UA_NodeId *nodeId,
                                   void *nodeContext, UA_UInt32 attributeId,
                                   const UA_DataValue *value);

    void addMonitoredItem(UA_Server *server);
};
#include "opc_controller.hxx"
#endif // OPC_TEMPLATE_CONTROLLER_H
