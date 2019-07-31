#include "../../include/opc/hmpcontroller.h"
#include <cstring>

HMPController::HMPController(std::string name): opc_template_controller<UA_HMPm,UA_HMPc,HMP2020>(name){
   VariableTypeM=UA_TYPES_DCSNODESET[UA_TYPES_DCSNODESET_HMPM];
   VariableTypeC=UA_TYPES_DCSNODESET[UA_TYPES_DCSNODESET_HMPC];
}

//HMPController::~HMPController(){
//    UA_NodeId_deleteMembers(&ObjectNodeId);
//}

void HMPController::init(UA_Server *server){
    addObject(server);
    //customType.addCustomVariableTypeNode(server);
    //addMeasurementsVariable(server);
    //addConfigurationVariable(server);
    addVariable3(server,&MeasurementsId,"Measurements",UA_TYPES_DCSNODESET[UA_TYPES_DCSNODESET_HMPM]);
    addVariable3(server,&ConfigurationId,"Configuration",UA_TYPES_DCSNODESET[UA_TYPES_DCSNODESET_HMPC]);
    addStatusVariable(server);
    addValueCallback(server,MeasurementsId ,MeasurementsReadCallback);
    addValueCallback(server,ConfigurationId ,ConfigurationReadCallback);
    addValueCallback(server,StatusId ,StatusReadCallback);
    addDisconnectDeviceMethod(server);
    addConnectDeviceMethod(server);
    addSetOutputMethod(server);
    addSetChannelMethod(server);
}
/*
HMPMeasurements HMPController::getMeasurements(){
      //  std::lock_guard<std::mutex> lock(deviceMutex);
        string response,ch1_v,ch2_v,ch1_c,ch2_c;
        bool ch1,ch2,output;
        device.setActiveChannel(1);
        ch1_v = device.getVoltage();
        ch1_c = device.getCurrent();
        response = device.getOutputSel();
        ch1 = (response=="1") ? true : false;
        device.setActiveChannel(2);
        ch2_v = device.getVoltage();
        ch2_c = device.getCurrent();
        response = device.getOutputSel();
        ch2 = (response=="1") ? true : false;
        response = device.getOutputGen();
        output = (response=="1") ? true : false;
        return customType.get(ch1,ch2,output,ch1_v,ch2_v,ch1_c,ch2_c);
}*/
UA_HMPm HMPController::getMeasurements(){
    std::string response;
    int size=2;
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


/*
HMPMeasurements HMPController::getSettings(){
     //   std::lock_guard<std::mutex> lock(deviceMutex);
        string response,ch1_v,ch2_v,ch1_c,ch2_c;
        bool ch1,ch2,output;
        device.setActiveChannel(1);
        ch1_v = device.getVoltageSet();
        ch1_c = device.getCurrentSet();
        response = device.getOutputSel();
        ch1 = (response=="1") ? true : false;
        device.setActiveChannel(2);
        ch2_v = device.getVoltageSet();
        ch2_c = device.getCurrentSet();
        response = device.getOutputSel();
        ch2 = (response=="1") ? true : false;
        response = device.getOutputGen();
        output = (response=="1") ? true : false;
        return customType.get(ch1,ch2,output,ch1_v,ch2_v,ch1_c,ch2_c);
}*/

UA_HMPc HMPController::getSettings(){
    int size=2;
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

UA_StatusCode HMPController::SetOutputCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output) {
//    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SetOutput was called");
    HMPController* Monitor=static_cast<HMPController*>(methodContext);
    if(Monitor->isConnected()){
        UA_Boolean state = *(UA_Boolean*)input->data;
    //    std::lock_guard<std::mutex> lock(Monitor->deviceMutex);
        DeviceCommand<HMP2020> command=std::bind(&HMP2020::setOutputGen, _1,state);
        Monitor->buffer.push(command);
       // Monitor->device.setOutputGen(state);
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
  //  UA_NodeId MethodNodeId=UA_NODEID_STRING_ALLOC(1,"SetOutput");
    UA_QualifiedName MethodQName= UA_QUALIFIEDNAME_ALLOC(1, "setoutput");
    UA_Server_addMethodNode(server, UA_NODEID_NULL,
                            ObjectNodeId,
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASORDEREDCOMPONENT),
                            MethodQName,
                            methodAttr, &SetOutputCallback,
                            1,&inputArgument, 0, nullptr,this, nullptr);
    UA_MethodAttributes_deleteMembers(&methodAttr);
    UA_Argument_deleteMembers(&inputArgument);
 //   UA_NodeId_deleteMembers(&MethodNodeId);
    UA_QualifiedName_deleteMembers(&MethodQName);
}

UA_StatusCode HMPController::SetChannelCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output) {
 //   UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SetOutput was called");
    HMPController* Monitor=static_cast<HMPController*>(methodContext);
    if(Monitor->isConnected()){
        UA_Int16 channel = *(UA_Int16*)input[0].data;
        UA_Boolean state = *(UA_Boolean*)input[1].data;
      //  std::lock_guard<std::mutex> lock(Monitor->deviceMutex);
        DeviceCommand<HMP2020> command=std::bind(&HMP2020::setOutputSel, _1,channel,state);
        Monitor->buffer.push(command);
        //Monitor->device.setOutputSel(channel,state);
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
 //   UA_NodeId MethodNodeId=UA_NODEID_STRING_ALLOC(1,"SetChannel");
    UA_QualifiedName MethodQName= UA_QUALIFIEDNAME_ALLOC(1, "setchannel");
    UA_Server_addMethodNode(server, UA_NODEID_NULL,
                            ObjectNodeId,
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASORDEREDCOMPONENT),
                            MethodQName,
                            methodAttr, &SetChannelCallback,
                            2,inputArguments, 0, nullptr,this, nullptr);
    UA_MethodAttributes_deleteMembers(&methodAttr);
    UA_Argument_deleteMembers(&inputArguments[0]);
    UA_Argument_deleteMembers(&inputArguments[1]);
  //  UA_NodeId_deleteMembers(&MethodNodeId);
    UA_QualifiedName_deleteMembers(&MethodQName);
}


