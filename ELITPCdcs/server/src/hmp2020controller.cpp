#include "hmp2020controller.h"
#include <cstring>

HMPController::HMPController(std::string name, int size): OpcTemplateController<UA_HMPm,UA_HMPc,HMP2020>(name), size(size){
   variableTypeM=UA_TYPES_DCSNODESET[UA_TYPES_DCSNODESET_HMPM];
   variableTypeC=UA_TYPES_DCSNODESET[UA_TYPES_DCSNODESET_HMPC];
   UA_HMPm_init(&measurements);
   UA_HMPc_init(&configuration);
}

//HMPController~HMPController(){
//    UA_NodeId_deleteMembers(&ObjectNodeId);
//}

void HMPController::init(UA_Server *server){
    addObject(server);
    //customType.addCustomVariableTypeNode(server);
    //addMeasurementsVariable(server);
    //addConfigurationVariable(server);
    addMeasurementsVariable(server);
    addConfigurationVariable(server);
    addStatusVariable(server);
    addDisconnectDeviceMethod(server);
    addConnectDeviceMethod(server);
    addSetOutputMethod(server);
    addSetChannelMethod(server);
    addSetVoltageMethod(server);
    addSetCurrentMethod(server);
    spawn_thread();
}

UA_HMPm HMPController::getMeasurements(){
    std::string response;
//    int size=2;
    UA_HMPm hmp;
    UA_HMPm_init(&hmp);
    response = device.getOutputGen();
    hmp.output = (response=="1") ? true : false;
    hmp.chSize=size;
    hmp.currentSize=size;
    hmp.voltageSize=size;
    hmp.ch=static_cast<UA_Boolean*>(UA_Array_new(size, &UA_TYPES[UA_TYPES_BOOLEAN]));
    hmp.current=static_cast<UA_Double*>(UA_Array_new(size, &UA_TYPES[UA_TYPES_DOUBLE]));
    hmp.voltage=static_cast<UA_Double*>(UA_Array_new(size, &UA_TYPES[UA_TYPES_DOUBLE]));
    for(int i=0; i!=size;++i){
        device.setActiveChannel(i+1);
        response = device.getOutputSel();
        hmp.ch[i]=(response=="1") ? true : false;
        hmp.current[i]=std::stod(device.getCurrent());
        hmp.voltage[i]=std::stod(device.getVoltage());
    }
    return hmp;
}




UA_HMPc HMPController::getSettings(){
//    int size=2;
    UA_HMPc hmp;
    UA_HMPc_init(&hmp);
    hmp.currentSetSize=size;
    hmp.voltageSetSize=size;
    hmp.currentSet=static_cast<UA_Double*>(UA_Array_new(size, &UA_TYPES[UA_TYPES_DOUBLE]));
    hmp.voltageSet=static_cast<UA_Double*>(UA_Array_new(size, &UA_TYPES[UA_TYPES_DOUBLE]));
    for(int i=0; i!=size;i++){
        device.setActiveChannel(i+1);
        hmp.currentSet[i]=std::stod(device.getCurrentSet());
        hmp.voltageSet[i]=std::stod(device.getVoltageSet());
    }
    return hmp;
}

UA_StatusCode HMPController::setOutputCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output) {
//    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SetOutput was called");
    HMPController* monitor=static_cast<HMPController*>(methodContext);
    if(monitor->isConnected()){
        UA_Boolean state = *(UA_Boolean*)input->data;
    //    std::lock_guard<std::mutex> lock(Monitor->deviceMutex);
        DeviceCommand<HMP2020> command=std::bind(&HMP2020::setOutputGen, _1,state);
        monitor->buffer.push(command);
       // monitor->device.setOutputGen(state);
    }
    else {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "device disconnected, SetOutput not send");
    }
    return UA_STATUSCODE_GOOD;
}

void HMPController::addSetOutputMethod(UA_Server *server) {

    UA_Argument inputArgument;
    UA_Argument_init(&inputArgument);
    inputArgument.description = UA_LOCALIZEDTEXT_ALLOC("en-US", "ON/OFF");
    inputArgument.name = UA_String_fromChars("State");
    inputArgument.dataType = UA_TYPES[UA_TYPES_BOOLEAN].typeId;
    inputArgument.valueRank = UA_VALUERANK_SCALAR;


    UA_MethodAttributes methodAttr = UA_MethodAttributes_default;
    methodAttr.description = UA_LOCALIZEDTEXT_ALLOC("en-US","Sets output ON/OFF");
    methodAttr.displayName = UA_LOCALIZEDTEXT_ALLOC("en-US","setoutput");
    methodAttr.executable = true;
    methodAttr.userExecutable = true;
    UA_QualifiedName methodQName= UA_QUALIFIEDNAME_ALLOC(1, "setoutput");
    UA_Server_addMethodNode(server, UA_NODEID_NULL,
                            objectNodeId,
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASORDEREDCOMPONENT),
                            methodQName,
                            methodAttr, &setOutputCallback,
                            1,&inputArgument, 0, nullptr,this, nullptr);
    UA_MethodAttributes_deleteMembers(&methodAttr);
    UA_Argument_deleteMembers(&inputArgument);
    UA_QualifiedName_deleteMembers(&methodQName);
}

UA_StatusCode HMPController::setChannelCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output) {
 //   UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SetOutput was called");
    HMPController* monitor=static_cast<HMPController*>(methodContext);
    if(monitor->isConnected()){
        UA_Int16 channel = *(UA_Int16*)input[0].data;
        UA_Boolean state = *(UA_Boolean*)input[1].data;
      //  std::lock_guard<std::mutex> lock(monitor->deviceMutex);
        DeviceCommand<HMP2020> command=std::bind(&HMP2020::setOutputSel, _1,channel,state);
        monitor->buffer.push(command);
        //monitor->device.setOutputSel(channel,state);
    }
    else {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "device disconnected, SetChannel not send");
    }
    return UA_STATUSCODE_GOOD;
}

void HMPController::addSetChannelMethod(UA_Server *server) {
    UA_Argument inputArguments[2];
    UA_Argument_init(&inputArguments[0]);
    inputArguments[0].description = UA_LOCALIZEDTEXT_ALLOC("en-US", "Channel number");
    inputArguments[0].name = UA_String_fromChars("Channel");
    inputArguments[0].dataType = UA_TYPES[UA_TYPES_INT16].typeId;
    inputArguments[0].valueRank = UA_VALUERANK_SCALAR;

    UA_Argument_init(&inputArguments[1]);
    inputArguments[1].description = UA_LOCALIZEDTEXT_ALLOC("en-US", "ON/OFF");
    inputArguments[1].name = UA_String_fromChars("State");
    inputArguments[1].dataType = UA_TYPES[UA_TYPES_BOOLEAN].typeId;
    inputArguments[1].valueRank = UA_VALUERANK_SCALAR;


    UA_MethodAttributes methodAttr = UA_MethodAttributes_default;
    methodAttr.description = UA_LOCALIZEDTEXT_ALLOC("en-US","Sets channel ON/OFF");
    methodAttr.displayName = UA_LOCALIZEDTEXT_ALLOC("en-US","setchannel");
    methodAttr.executable = true;
    methodAttr.userExecutable = true;
    UA_QualifiedName methodQName= UA_QUALIFIEDNAME_ALLOC(1, "setchannel");
    UA_Server_addMethodNode(server, UA_NODEID_NULL,
                            objectNodeId,
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASORDEREDCOMPONENT),
                            methodQName,
                            methodAttr, &setChannelCallback,
                            2,inputArguments, 0, nullptr,this, nullptr);
    UA_MethodAttributes_deleteMembers(&methodAttr);
    UA_Argument_deleteMembers(&inputArguments[0]);
    UA_Argument_deleteMembers(&inputArguments[1]);
    UA_QualifiedName_deleteMembers(&methodQName);
}

UA_StatusCode HMPController::setVoltageCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output) {
 //   UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SetOutput was called");
    HMPController* monitor=static_cast<HMPController*>(methodContext);
    if(monitor->isConnected()){
        UA_Int16 channel = *(UA_Int16*)input[0].data;
        UA_Double voltage = *(UA_Double*)input[1].data;
      //  std::lock_guard<std::mutex> lock(monitor->deviceMutex);
        DeviceCommand<HMP2020> command=std::bind(&HMP2020::setVoltage, _1,channel,voltage);
        monitor->buffer.push(command);
        //monitor->device.setVoltage(channel,state);
    }
    else {
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "device disconnected, SetVoltage not send");
    }
    return UA_STATUSCODE_GOOD;
}

void HMPController::addSetVoltageMethod(UA_Server *server) {
    UA_Argument inputArguments[2];
    UA_Argument_init(&inputArguments[0]);
    inputArguments[0].description = UA_LOCALIZEDTEXT_ALLOC("en-US", "Channel number");
    inputArguments[0].name = UA_String_fromChars("Channel");
    inputArguments[0].dataType = UA_TYPES[UA_TYPES_INT16].typeId;
    inputArguments[0].valueRank = UA_VALUERANK_SCALAR;

    UA_Argument_init(&inputArguments[1]);
    inputArguments[1].description = UA_LOCALIZEDTEXT_ALLOC("en-US", "Voltage in V");
    inputArguments[1].name = UA_String_fromChars("Voltage");
    inputArguments[1].dataType = UA_TYPES[UA_TYPES_DOUBLE].typeId;
    inputArguments[1].valueRank = UA_VALUERANK_SCALAR;


    UA_MethodAttributes methodAttr = UA_MethodAttributes_default;
    methodAttr.description = UA_LOCALIZEDTEXT_ALLOC("en-US","Sets voltage");
    methodAttr.displayName = UA_LOCALIZEDTEXT_ALLOC("en-US","setvoltage");
    methodAttr.executable = true;
    methodAttr.userExecutable = true;
    UA_QualifiedName methodQName= UA_QUALIFIEDNAME_ALLOC(1, "setvoltage");
    UA_Server_addMethodNode(server, UA_NODEID_NULL,
                            objectNodeId,
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASORDEREDCOMPONENT),
                            methodQName,
                            methodAttr, &setVoltageCallback,
                            2,inputArguments, 0, nullptr,this, nullptr);
    UA_MethodAttributes_deleteMembers(&methodAttr);
    UA_Argument_deleteMembers(&inputArguments[0]);
    UA_Argument_deleteMembers(&inputArguments[1]);
    UA_QualifiedName_deleteMembers(&methodQName);
}


UA_StatusCode HMPController::setCurrentCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output) {
 //   UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SetOutput was called");
    HMPController* monitor=static_cast<HMPController*>(methodContext);
    if(monitor->isConnected()){
        UA_Int16 channel = *(UA_Int16*)input[0].data;
        UA_Double current = *(UA_Double*)input[1].data;
      //  std::lock_guard<std::mutex> lock(monitor->deviceMutex);
        DeviceCommand<HMP2020> command=std::bind(&HMP2020::setCurrent, _1,channel,current);
        monitor->buffer.push(command);
        //monitor->device.setVoltage(current,state);
    }
    else {
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "device disconnected, SetCurrent not send");
    }
    return UA_STATUSCODE_GOOD;
}

void HMPController::addSetCurrentMethod(UA_Server *server) {
    UA_Argument inputArguments[2];
    UA_Argument_init(&inputArguments[0]);
    inputArguments[0].description = UA_LOCALIZEDTEXT_ALLOC("en-US", "Channel number");
    inputArguments[0].name = UA_String_fromChars("Channel");
    inputArguments[0].dataType = UA_TYPES[UA_TYPES_INT16].typeId;
    inputArguments[0].valueRank = UA_VALUERANK_SCALAR;

    UA_Argument_init(&inputArguments[1]);
    inputArguments[1].description = UA_LOCALIZEDTEXT_ALLOC("en-US", "Current in A");
    inputArguments[1].name = UA_String_fromChars("Current");
    inputArguments[1].dataType = UA_TYPES[UA_TYPES_DOUBLE].typeId;
    inputArguments[1].valueRank = UA_VALUERANK_SCALAR;


    UA_MethodAttributes methodAttr = UA_MethodAttributes_default;
    methodAttr.description = UA_LOCALIZEDTEXT_ALLOC("en-US","Sets current");
    methodAttr.displayName = UA_LOCALIZEDTEXT_ALLOC("en-US","setcurrent");
    methodAttr.executable = true;
    methodAttr.userExecutable = true;
    UA_QualifiedName methodQName= UA_QUALIFIEDNAME_ALLOC(1, "setcurrent");
    UA_Server_addMethodNode(server, UA_NODEID_NULL,
                            objectNodeId,
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASORDEREDCOMPONENT),
                            methodQName,
                            methodAttr, &setCurrentCallback,
                            2,inputArguments, 0, nullptr,this, nullptr);
    UA_MethodAttributes_deleteMembers(&methodAttr);
    UA_Argument_deleteMembers(&inputArguments[0]);
    UA_Argument_deleteMembers(&inputArguments[1]);
    UA_QualifiedName_deleteMembers(&methodQName);
}
