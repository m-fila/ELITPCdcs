#include "src/utl/TCPConnector.h"
#include "src/hw/HMP2020.h"
#include "src/DeviceController.h"
#include "src/ConnectionParameters.h"
#include <sstream>
#include <iostream>
#include <time.h>
#include "opc/opc_server.h"
#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>
#include "opc/HMPDataType.h"
#include "opc/hmpcontroller.h"
#include "opc/opc_state.h"
//#include <string>

int main(int argc, char *argv[])
{
    opc_server server;


    string ipaddress="192.168.168.20";
    int port=5025;

    TCPConnectionParameters parameters;
    parameters.IPaddress=ipaddress;
    parameters.port=port;

    HMPController controller("HMP2");
    opc_state state;
    state.init(server.server);
//    state.addObject(server.server);
//    state.addStateVariable(server.server);
    controller.addCustomDataType(server.server,server.config);
    controller.addObject(server.server);
    controller.addMeasurementsVariable(server.server);
    controller.addConfigurationVariable(server.server);
    controller.addStatusVariable(server.server);

  //  controller.connect(&parameters);
//    controller.updateStatus(server.server);

    controller.addMonitoredItem(server.server, controller.MeasurementsVariableName);
    controller.addValueCallback(server.server,controller.MeasurementsVariableName ,controller.MeasurementsReadCallback);
    controller.addValueCallback(server.server,controller.ConfigurationVariableName ,controller.ConfigurationReadCallback);
    controller.addValueCallback(server.server,controller.StatusVariableName ,controller.StatusReadCallback);
    controller.addDisconnectDeviceMethod(server.server);
    controller.addConnectDeviceMethod(server.server);
    controller.addSetOutputMethod(server.server);
    controller.addSetChannelMethod(server.server);



    server.run();


return 0;
}
