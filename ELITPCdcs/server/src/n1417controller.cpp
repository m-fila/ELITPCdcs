#include "n1417controller.h"
#include <sstream>

N1417Controller::N1417Controller(std::string name): 
    OpcTemplateController<UA_DT1415m,UA_DT1415c,N1417>(name,
        UA_TYPES_DCSNODESET[UA_TYPES_DCSNODESET_DT1415M],
        UA_TYPES_DCSNODESET[UA_TYPES_DCSNODESET_DT1415C]){
}

void N1417Controller::init(UA_Server* server){
    addObject(server);
    addMeasurementsVariable(server);
    addConfigurationVariable(server);
    addStatusVariable(server);
    addDisconnectDeviceMethod(server);
    addConnectDeviceMethod(server);
    addSetChannelMethod(server);
    addSetVoltageMethod(server);
    addSetCurrentMethod(server);
    addSetVoltageMaxMethod(server);
    addSetRampUpMethod(server);
    addSetRampDownMethod(server);
    spawn_thread();
}

UA_DT1415m N1417Controller::getMeasurements(){
    UA_DT1415m dtm;
    UA_DT1415m_init(&dtm);
    int size=4;
    dtm.voltageSize=size;
    dtm.currentSize=size;
    dtm.voltage=static_cast<UA_Double*>(UA_Array_new(size, &UA_TYPES[UA_TYPES_DOUBLE]));
    dtm.current=static_cast<UA_Double*>(UA_Array_new(size, &UA_TYPES[UA_TYPES_DOUBLE]));
    //voltages
    std::string response = device.getVoltage(N1417::CHANNEL::ALL);
    std::istringstream iss(response);
    std::string val;
    int i;
    float total = 0;
    for(i=0; i<size; ++i)
    {
        std::getline(iss, val, ';');
        dtm.voltage[i] = std::stod(val.c_str());
        total += dtm.voltage[i];
    }
    dtm.totalVoltage = total;
    //currents
    response = device.getCurrent(N1417::CHANNEL::ALL);
    std::istringstream iss2(response);
    for(i=0; i<size; i++)
    {
        std::getline(iss2, val, ';');
        dtm.current[i] = std::stod(val.c_str());
    }

    return dtm;
}

UA_DT1415c N1417Controller::getSettings(){
    UA_DT1415c dtc;
    UA_DT1415c_init(&dtc);
    int size=4;
    dtc.voltageSetSize=size;
    dtc.currentSetSize=size;
    dtc.statusSize=size;
    dtc.voltageMaxSize=size;
    dtc.rupSize=size;
    dtc.rdownSize=size;
    dtc.voltageSet=static_cast<UA_Double*>(UA_Array_new(size, &UA_TYPES[UA_TYPES_DOUBLE]));
    dtc.currentSet=static_cast<UA_Double*>(UA_Array_new(size, &UA_TYPES[UA_TYPES_DOUBLE]));
    dtc.status=static_cast<UA_UInt32*>(UA_Array_new(size, &UA_TYPES[UA_TYPES_UINT32]));
    dtc.voltageMax=static_cast<UA_Double*>(UA_Array_new(size, &UA_TYPES[UA_TYPES_DOUBLE]));
    dtc.rdown=static_cast<UA_Double*>(UA_Array_new(size, &UA_TYPES[UA_TYPES_DOUBLE]));
    dtc.rup=static_cast<UA_Double*>(UA_Array_new(size, &UA_TYPES[UA_TYPES_DOUBLE]));
    dtc.isRemote = device.isRemote();
    //device status
    std::string response = device.getStatus(N1417::CHANNEL::ALL);
    std::istringstream iss(response);
    std::string val;
    int i;
    for(i=0; i<size; ++i)
    {
        std::getline(iss, val, ';');
        dtc.status[i] = std::stoi(val.c_str());
    }
    //voltage set
    float total = 0;
    response = device.getVoltageSet(N1417::CHANNEL::ALL);
    std::istringstream iss2(response);
    for(i=0; i<size; i++)
    {
        std::getline(iss2, val, ';');
        dtc.voltageSet[i] = std::stod(val.c_str());
        total += dtc.voltageSet[i];
    }

    response = device.getCurrentSet(N1417::CHANNEL::ALL);
    std::istringstream iss6(response);
    for(i=0; i<size; i++)
    {
        std::getline(iss6, val, ';');
        dtc.currentSet[i] = std::stod(val.c_str());
    }

    dtc.totalVoltageSet= total;

    response = device.getRampUp(N1417::CHANNEL::ALL);
    std::istringstream iss3(response);
    for(i=0; i<size; ++i)
    {
        std::getline(iss3, val, ';');
        dtc.rup[i] = std::stoi(val.c_str());
    }

    response = device.getRampDown(N1417::CHANNEL::ALL);
    std::istringstream iss4(response);
    for(i=0; i<size; ++i)
    {
        std::getline(iss4, val, ';');
        dtc.rdown[i] = std::stoi(val.c_str());
    }
    response = device.getVoltageMax(N1417::CHANNEL::ALL);
    std::istringstream iss5(response);
    for(i=0; i<size; ++i)
    {
        std::getline(iss5, val, ';');
        dtc.voltageMax[i] = std::stoi(val.c_str());
    }

    return dtc;
}

UA_StatusCode N1417Controller::setChannelCallback(UA_Server *server,
                                         const UA_NodeId *sessionId, void *sessionHandle,
                                         const UA_NodeId *methodId, void *methodContext,
                                         const UA_NodeId *objectId, void *objectContext,
                                         size_t inputSize, const UA_Variant *input,
                                         size_t outputSize, UA_Variant *output){
    N1417Controller* Monitor=static_cast<N1417Controller*>(methodContext);
    if(Monitor->isConnected()){
        UA_Int16 channel = *(UA_Int16*)input[0].data;
        UA_Boolean state = *(UA_Boolean*)input[1].data;
        N1417::CHANNEL CH=static_cast <N1417::CHANNEL>(channel);
        if(state){
           // Monitor->device.setON(CH);
            DeviceCommand<N1417> command=std::bind(&N1417::setON, _1,CH);
            Monitor->buffer.push(command);
        }
        else{
            //Monitor->device.setOFF(CH);
            DeviceCommand<N1417> command=std::bind(&N1417::setOFF, _1,CH);
            Monitor->buffer.push(command);
        }

    }
    else {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "device disconnected, SetChannel not send");
    }
    return UA_STATUSCODE_GOOD;

}
void N1417Controller::addSetChannelMethod(UA_Server* server){
    addMethod(server,"setchannel",setChannelCallback,"Sets channel ON/OFF",{{UA_TYPES[UA_TYPES_INT16],"Channel","Channel number"},{UA_TYPES[UA_TYPES_BOOLEAN],"State","ON/OFF"}},{});
}    

UA_StatusCode N1417Controller::setVoltageCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output) {
 //   UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SetOutput was called");
    N1417Controller* Monitor=static_cast<N1417Controller*>(methodContext);
    if(Monitor->isConnected()){
        UA_Int16 channel = *(UA_Int16*)input[0].data;
        UA_Double voltage = *(UA_Double*)input[1].data;
        N1417::CHANNEL CH=static_cast <N1417::CHANNEL>(channel);
        DeviceCommand<N1417> command=std::bind(&N1417::setVoltageSet, _1,CH,voltage);
        Monitor->buffer.push(command);
       // Monitor->device.setVoltageSet(CH,voltage);
    }
    else {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "device disconnected, SetVoltage not send");
    }
    return UA_STATUSCODE_GOOD;
}

void N1417Controller::addSetVoltageMethod(UA_Server *server) {
    addMethod(server,"setvoltage",setVoltageCallback,"Sets channel's voltage",{{UA_TYPES[UA_TYPES_INT16],"Channel","Channel number"},{UA_TYPES[UA_TYPES_DOUBLE],"Voltage","Voltage in V"}},{});
} 

UA_StatusCode N1417Controller::setVoltageMaxCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output) {
 //   UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SetOutput was called");
    N1417Controller* Monitor=static_cast<N1417Controller*>(methodContext);
    if(Monitor->isConnected()){
        UA_Int16 channel = *(UA_Int16*)input[0].data;
        UA_Double voltage = *(UA_Double*)input[1].data;
        N1417::CHANNEL CH=static_cast <N1417::CHANNEL>(channel);
        //Monitor->device.setVoltageMax(CH,voltage);
        DeviceCommand<N1417> command=std::bind(&N1417::setVoltageMax, _1,CH,voltage);
        Monitor->buffer.push(command);
    }
    else {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "device disconnected, SetVoltageMax not send");
    }
    return UA_STATUSCODE_GOOD;
}

void N1417Controller::addSetVoltageMaxMethod(UA_Server *server) {
    addMethod(server,"setvoltagemax",setVoltageMaxCallback,"Sets channel's voltage max",{{UA_TYPES[UA_TYPES_INT16],"Channel","Channel number"},{UA_TYPES[UA_TYPES_DOUBLE],"Maximum voltage","Maximum voltage in V"}},{});
}

UA_StatusCode N1417Controller::setRampUpCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output) {
 //   UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SetOutput was called");
    N1417Controller* Monitor=static_cast<N1417Controller*>(methodContext);
    if(Monitor->isConnected()){
        UA_Int16 channel = *(UA_Int16*)input[0].data;
        UA_Double voltage = *(UA_Double*)input[1].data;
        N1417::CHANNEL CH=static_cast <N1417::CHANNEL>(channel);
       // Monitor->device.setRampUp(CH,voltage);
        DeviceCommand<N1417> command=std::bind(&N1417::setRampUp, _1,CH,voltage);
        Monitor->buffer.push(command);
    }
    else {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "device disconnected, SetRampUp not send");
    }
    return UA_STATUSCODE_GOOD;
}

void N1417Controller::addSetRampUpMethod(UA_Server *server) {
    addMethod(server,"setrampup",setRampUpCallback,"Sets channel's ramp up",{{UA_TYPES[UA_TYPES_INT16],"Channel","Channel number"},{UA_TYPES[UA_TYPES_DOUBLE],"Ramp up","Ramp up in V/s"}},{});
}

UA_StatusCode N1417Controller::setRampDownCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output) {
 //   UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SetOutput was called");
    N1417Controller* Monitor=static_cast<N1417Controller*>(methodContext);
    if(Monitor->isConnected()){
        UA_Int16 channel = *(UA_Int16*)input[0].data;
        UA_Double voltage = *(UA_Double*)input[1].data;
        N1417::CHANNEL CH=static_cast <N1417::CHANNEL>(channel);
        //Monitor->device.setRampDown(CH,voltage);
        DeviceCommand<N1417> command=std::bind(&N1417::setRampDown, _1,CH,voltage);
        Monitor->buffer.push(command);
    }
    else {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "device disconnected, SetRampDown not send");
    }
    return UA_STATUSCODE_GOOD;
}

void N1417Controller::addSetRampDownMethod(UA_Server *server) {
    addMethod(server,"setrampdown",setRampDownCallback,"Sets channel's ramp down",{{UA_TYPES[UA_TYPES_INT16],"Channel","Channel number"},{UA_TYPES[UA_TYPES_DOUBLE],"Ramp down","Ramp down in V/s"}},{});
}

void N1417Controller::addSetCurrentMethod(UA_Server *server) {
    addMethod(server,"setcurrent",setCurrentCallback,"Sets channel's current",{{UA_TYPES[UA_TYPES_INT16],"Channel","Channel number"},{UA_TYPES[UA_TYPES_DOUBLE],"Current","Current in A"}},{});
}

UA_StatusCode N1417Controller::setCurrentCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output) {
 //   UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SetOutput was called");
    N1417Controller* Monitor=static_cast<N1417Controller*>(methodContext);
    if(Monitor->isConnected()){
        UA_Int16 channel = *(UA_Int16*)input[0].data;
        UA_Double current = *(UA_Double*)input[1].data;
        N1417::CHANNEL CH=static_cast <N1417::CHANNEL>(channel);
        DeviceCommand<N1417> command=std::bind(&N1417::setCurrentSet, _1,CH,current);
        Monitor->buffer.push(command);
    }
    else {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "device disconnected, SetCurrentSet not send");
    }
    return UA_STATUSCODE_GOOD;
}