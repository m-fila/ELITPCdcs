#include "src/utl/TCPConnector.h"
#include "src/hw/HMP2020.h"
#include "src/DeviceController.h"
#include "src/ConnectionParameters.h"
#include <sstream>
#include <iostream>
#include <time.h>
#include "opc/opcserver.h"
#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>
#include "opc/HMPDataType.h"
#include "opc/hmpcontroller.h"
//#include <string>


int main(int argc, char *argv[])
{  //UA_Server *server = UA_Server_new();
   // UA_ServerConfig_setDefault(UA_Server_getConfig(server));

    opcserver server;

 //   server.run();


string ipaddress="192.168.168.20";
int port=5025;

TCPConnectionParameters* parameters=new TCPConnectionParameters;
parameters->IPaddress=ipaddress;
parameters->port=port;

HMPController controller("HMP2");



controller.addVariable(server.server);
controller.connect(parameters);

//controller.updateVariable(server.server);
//controller.addMonitoredItem(server.server);
controller.addValueCallback(server.server);
controller.addDisconnectDeviceMethod(server.server);
controller.addConnectDeviceMethod(server.server);
controller.addSetOutputMethod(server.server);
controller.addSetChannelMethod(server.server);

//HMPMeasurements mea=controller.getMeasurements();
//controller.addMonitoredItem(server.server);
//controller.addValueCallbackToCurrentTimeVariable(server.server);
server.run();
//AbstractDeviceController<HMP2020> controller;
/*
controller.connect(parameters);
while(true){
string str=controller.device->getVoltageSet();

 std::cout<<str<<std::endl;
sleep(10);}
*/
//controller.disconnect();

return 0;
}
