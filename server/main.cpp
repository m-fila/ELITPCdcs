//#include "include/utl/TCPConnector.h"
//#include "include/hw/HMP2020.h"
//#include "include/DeviceController.h"
//#include "include/ConnectionParameters.h"
//#include <sstream>
//#include <iostream>
//#include <time.h>
//#include <open62541/plugin/log_stdout.h>
//#include <open62541/server.h>
//#include <open62541/server_config_default.h>
#include "include/opc/opc_server.h"
#include "include/opc/hmpcontroller.h"
#include "include/opc/dtcontroller.h"
#include "include/opc/opc_state.h"


int main(int argc, char *argv[])
{
    opc_server server;

/*
    string ipaddress="192.168.168.20";
    int port=5025;

    TCPConnectionParameters parameters;
    parameters.IPaddress=ipaddress;
    parameters.port=port;
*/
    HMPController controller("HMP2");
    opc_state state;
    state.init(server.server);
    UA_DataTypeArray hmpcustom=controller.customType.DataTypeArray(nullptr); //{nullptr,1,types};
    server.addCustomTypes(&hmpcustom);
    controller.customType.addCustomVariableTypeNode(server.server);
    controller.addObject(server.server);
    controller.addMeasurementsVariable(server.server);
    controller.addConfigurationVariable(server.server);
    controller.addStatusVariable(server.server);
 //   controller.addMonitoredItem(server.server, controller.MeasurementsVariableName);
    controller.addValueCallback(server.server,controller.MeasurementsId ,controller.MeasurementsReadCallback);
    controller.addValueCallback(server.server,controller.ConfigurationId ,controller.ConfigurationReadCallback);
    controller.addValueCallback(server.server,controller.StatusId ,controller.StatusReadCallback);
    controller.addDisconnectDeviceMethod(server.server);
    controller.addConnectDeviceMethod(server.server);
    controller.addSetOutputMethod(server.server);
    controller.addSetChannelMethod(server.server);


    DTController dtcontroller("HV1");
    dtcontroller.addObject(server.server);
    dtcontroller.customTypeM.addCustomVariableTypeNode(server.server);
    dtcontroller.customTypeC.addCustomVariableTypeNode(server.server);
    dtcontroller.addMeasurementsVariable(server.server);
    dtcontroller.addConfigurationVariable(server.server);
    dtcontroller.addStatusVariable(server.server);
    dtcontroller.addValueCallback(server.server,dtcontroller.MeasurementsId ,dtcontroller.MeasurementsReadCallback);
    dtcontroller.addValueCallback(server.server,dtcontroller.ConfigurationId,dtcontroller.ConfigurationReadCallback);
    dtcontroller.addValueCallback(server.server,dtcontroller.StatusId ,dtcontroller.StatusReadCallback);
    dtcontroller.addDisconnectDeviceMethod(server.server);
    dtcontroller.addConnectDeviceMethod(server.server);
    server.run();
return 0;
}
