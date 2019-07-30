#include "../../include/opc/dtcontroller.h"
#include <sstream>
DTController::DTController(std::string name): opc_template_controller<DTMeasurements,DTConfiguration,DT1415ET>(name){
    VariableTypeM=customTypeM.Type;
    VariableTypeC=customTypeC.Type;    
}

//DTController::~DTController(){
//    UA_NodeId_deleteMembers(&ObjectNodeId);
//}


void DTController::init(UA_Server* server){
    addObject(server);
    customTypeM.addCustomVariableTypeNode(server);
    customTypeC.addCustomVariableTypeNode(server);
    addMeasurementsVariable(server);
    addConfigurationVariable(server);
    addStatusVariable(server);
    addValueCallback(server,MeasurementsId ,MeasurementsReadCallback);
    addValueCallback(server,ConfigurationId,ConfigurationReadCallback);
    addValueCallback(server,StatusId ,StatusReadCallback);
    addDisconnectDeviceMethod(server);
    addConnectDeviceMethod(server);
    addSetChannelMethod(server);
    addSetVoltageMethod(server);
    addSetVoltageMaxMethod(server);
    addSetRampUpMethod(server);
    addSetRampDownMethod(server);
}

DTMeasurements DTController::getMeasurements(){
    DTMeasurements dtm;
    //voltages
    std::string response = device.getVoltage(DT1415ET::CHANNEL::ALL);
    std::istringstream iss(response);
    std::string val;
    int i;
    float total = 0;
    for(i=0; i<8; i++)
    {
        std::getline(iss, val, ';');
        dtm.voltage[i] = std::stof(val.c_str());
        total += dtm.voltage[i];
        //std::cout << voltageMeasurements.voltage[i] << std::endl;
    }
    dtm.totalVoltage = total;
    //currents
    response = device.getCurrent(DT1415ET::CHANNEL::ALL);
    std::istringstream iss2(response);
    for(i=0; i<8; i++)
    {
        std::getline(iss2, val, ';');
        dtm.current[i] = std::stof(val.c_str());
        //std::cout << measurements.current[i] << std::endl;
    }

    return dtm;
}

DTConfiguration DTController::getSettings(){
    DTConfiguration dtc;
    dtc.isRemote = device.isRemote();
    //device status
    std::string response = device.getStatus(DT1415ET::CHANNEL::ALL);
    std::istringstream iss(response);
    std::string val;
    int i;
    for(i=0; i<8; i++)
    {
        std::getline(iss, val, ';');
        dtc.status[i] = std::stoi(val.c_str());
        //std::cout << val << std::endl;
    }
    //voltage set
    float total = 0;
    response = device.getVoltageSet(DT1415ET::CHANNEL::ALL);
    std::istringstream iss2(response);
    for(i=0; i<8; i++)
    {
        std::getline(iss2, val, ';');
        //std::cout << val << std::endl;
        dtc.voltageSet[i] = std::stof(val.c_str());
        total += dtc.voltageSet[i];
    }
    dtc.totalVoltageSet= total;
    return dtc;
}

UA_StatusCode DTController::SetChannelCallback(UA_Server *server,
                                         const UA_NodeId *sessionId, void *sessionHandle,
                                         const UA_NodeId *methodId, void *methodContext,
                                         const UA_NodeId *objectId, void *objectContext,
                                         size_t inputSize, const UA_Variant *input,
                                         size_t outputSize, UA_Variant *output){
    DTController* Monitor=static_cast<DTController*>(methodContext);
    if(Monitor->isConnected()){
        UA_Int16 channel = *(UA_Int16*)input[0].data;
        UA_Boolean state = *(UA_Boolean*)input[1].data;
        DT1415ET::CHANNEL CH=static_cast <DT1415ET::CHANNEL>(channel);
        if(state){
           // Monitor->device.setON(CH);
            DeviceCommand<DT1415ET> command=std::bind(&DT1415ET::setON, _1,CH);
            Monitor->buffer.push(command);
        }
        else{
            //Monitor->device.setOFF(CH);
            DeviceCommand<DT1415ET> command=std::bind(&DT1415ET::setON, _1,CH);
            Monitor->buffer.push(command);
        }

    }
    else {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "device disconnected, SetChannel not send");
    }
    return UA_STATUSCODE_GOOD;

}

void DTController::addSetChannelMethod(UA_Server* server){
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


UA_StatusCode DTController::SetVoltageCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output) {
 //   UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SetOutput was called");
    DTController* Monitor=static_cast<DTController*>(methodContext);
    if(Monitor->isConnected()){
        UA_Int16 channel = *(UA_Int16*)input[0].data;
        UA_Double voltage = *(UA_Double*)input[1].data;
        DT1415ET::CHANNEL CH=static_cast <DT1415ET::CHANNEL>(channel);
        DeviceCommand<DT1415ET> command=std::bind(&DT1415ET::setVoltageSet, _1,CH,voltage);
        Monitor->buffer.push(command);
       // Monitor->device.setVoltageSet(CH,voltage);
    }
    else {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "device disconnected, SetVoltage not send");
    }
    return UA_STATUSCODE_GOOD;
}
void DTController::addSetVoltageMethod(UA_Server *server) {
    UA_Argument inputArguments[2];
    UA_Argument_init(&inputArguments[0]);
    inputArguments[0].description = UA_LOCALIZEDTEXT_ALLOC("en-US", "Channel number");
    inputArguments[0].name = UA_String_fromChars("Channel");
    inputArguments[0].dataType = UA_TYPES[UA_TYPES_INT16].typeId;
    inputArguments[0].valueRank = UA_VALUERANK_SCALAR;

    UA_Argument_init(&inputArguments[1]);
    inputArguments[1].description = UA_LOCALIZEDTEXT_ALLOC("en-US", "Voltage");
    inputArguments[1].name = UA_String_fromChars("Voltage");
    inputArguments[1].dataType = UA_TYPES[UA_TYPES_DOUBLE].typeId;
    inputArguments[1].valueRank = UA_VALUERANK_SCALAR;


    UA_MethodAttributes methodAttr = UA_MethodAttributes_default;
    methodAttr.description = UA_LOCALIZEDTEXT_ALLOC("en-US","Sets channel's voltage");
    methodAttr.displayName = UA_LOCALIZEDTEXT_ALLOC("en-US","setvoltage");
    methodAttr.executable = true;
    methodAttr.userExecutable = true;
 //   UA_NodeId MethodNodeId=UA_NODEID_STRING_ALLOC(1,"SetVoltage");
    UA_QualifiedName MethodQName= UA_QUALIFIEDNAME_ALLOC(1, "setvoltage");
    UA_Server_addMethodNode(server, UA_NODEID_NULL,
                            ObjectNodeId,
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASORDEREDCOMPONENT),
                            MethodQName,
                            methodAttr, &SetVoltageCallback,
                            2,inputArguments, 0, nullptr,this, nullptr);
    UA_MethodAttributes_deleteMembers(&methodAttr);
    UA_Argument_deleteMembers(&inputArguments[0]);
    UA_Argument_deleteMembers(&inputArguments[1]);
  //  UA_NodeId_deleteMembers(&MethodNodeId);
    UA_QualifiedName_deleteMembers(&MethodQName);
}

UA_StatusCode DTController::SetVoltageMaxCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output) {
 //   UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SetOutput was called");
    DTController* Monitor=static_cast<DTController*>(methodContext);
    if(Monitor->isConnected()){
        UA_Int16 channel = *(UA_Int16*)input[0].data;
        UA_Double voltage = *(UA_Double*)input[1].data;
        DT1415ET::CHANNEL CH=static_cast <DT1415ET::CHANNEL>(channel);
        //Monitor->device.setVoltageMax(CH,voltage);
        DeviceCommand<DT1415ET> command=std::bind(&DT1415ET::setVoltageMax, _1,CH,voltage);
        Monitor->buffer.push(command);
    }
    else {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "device disconnected, SetVoltageMax not send");
    }
    return UA_STATUSCODE_GOOD;
}
void DTController::addSetVoltageMaxMethod(UA_Server *server) {
    UA_Argument inputArguments[2];
    UA_Argument_init(&inputArguments[0]);
    inputArguments[0].description = UA_LOCALIZEDTEXT_ALLOC("en-US", "Channel number");
    inputArguments[0].name = UA_String_fromChars("Channel");
    inputArguments[0].dataType = UA_TYPES[UA_TYPES_INT16].typeId;
    inputArguments[0].valueRank = UA_VALUERANK_SCALAR;

    UA_Argument_init(&inputArguments[1]);
    inputArguments[1].description = UA_LOCALIZEDTEXT_ALLOC("en-US", "Voltage");
    inputArguments[1].name = UA_String_fromChars("Voltage");
    inputArguments[1].dataType = UA_TYPES[UA_TYPES_DOUBLE].typeId;
    inputArguments[1].valueRank = UA_VALUERANK_SCALAR;


    UA_MethodAttributes methodAttr = UA_MethodAttributes_default;
    methodAttr.description = UA_LOCALIZEDTEXT_ALLOC("en-US","Sets channel's voltage max");
    methodAttr.displayName = UA_LOCALIZEDTEXT_ALLOC("en-US","setvoltagemax");
    methodAttr.executable = true;
    methodAttr.userExecutable = true;
 //   UA_NodeId MethodNodeId=UA_NODEID_STRING_ALLOC(1,"SetVoltage");
    UA_QualifiedName MethodQName= UA_QUALIFIEDNAME_ALLOC(1, "setvoltagemax");
    UA_Server_addMethodNode(server, UA_NODEID_NULL,
                            ObjectNodeId,
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASORDEREDCOMPONENT),
                            MethodQName,
                            methodAttr, &SetVoltageMaxCallback,
                            2,inputArguments, 0, nullptr,this, nullptr);
    UA_MethodAttributes_deleteMembers(&methodAttr);
    UA_Argument_deleteMembers(&inputArguments[0]);
    UA_Argument_deleteMembers(&inputArguments[1]);
  //  UA_NodeId_deleteMembers(&MethodNodeId);
    UA_QualifiedName_deleteMembers(&MethodQName);
}

UA_StatusCode DTController::SetRampUpCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output) {
 //   UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SetOutput was called");
    DTController* Monitor=static_cast<DTController*>(methodContext);
    if(Monitor->isConnected()){
        UA_Int16 channel = *(UA_Int16*)input[0].data;
        UA_Double voltage = *(UA_Double*)input[1].data;
        DT1415ET::CHANNEL CH=static_cast <DT1415ET::CHANNEL>(channel);
       // Monitor->device.setRampUp(CH,voltage);
        DeviceCommand<DT1415ET> command=std::bind(&DT1415ET::setRampDown, _1,CH,voltage);
        Monitor->buffer.push(command);
    }
    else {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "device disconnected, SetRampUp not send");
    }
    return UA_STATUSCODE_GOOD;
}
void DTController::addSetRampUpMethod(UA_Server *server) {
    UA_Argument inputArguments[2];
    UA_Argument_init(&inputArguments[0]);
    inputArguments[0].description = UA_LOCALIZEDTEXT_ALLOC("en-US", "Channel number");
    inputArguments[0].name = UA_String_fromChars("Channel");
    inputArguments[0].dataType = UA_TYPES[UA_TYPES_INT16].typeId;
    inputArguments[0].valueRank = UA_VALUERANK_SCALAR;

    UA_Argument_init(&inputArguments[1]);
    inputArguments[1].description = UA_LOCALIZEDTEXT_ALLOC("en-US", "V/s");
    inputArguments[1].name = UA_String_fromChars("Ramp up");
    inputArguments[1].dataType = UA_TYPES[UA_TYPES_DOUBLE].typeId;
    inputArguments[1].valueRank = UA_VALUERANK_SCALAR;


    UA_MethodAttributes methodAttr = UA_MethodAttributes_default;
    methodAttr.description = UA_LOCALIZEDTEXT_ALLOC("en-US","Sets channel's ramp up");
    methodAttr.displayName = UA_LOCALIZEDTEXT_ALLOC("en-US","setrampup");
    methodAttr.executable = true;
    methodAttr.userExecutable = true;
 //   UA_NodeId MethodNodeId=UA_NODEID_STRING_ALLOC(1,"SetVoltage");
    UA_QualifiedName MethodQName= UA_QUALIFIEDNAME_ALLOC(1, "setrampup");
    UA_Server_addMethodNode(server, UA_NODEID_NULL,
                            ObjectNodeId,
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASORDEREDCOMPONENT),
                            MethodQName,
                            methodAttr, &SetRampUpCallback,
                            2,inputArguments, 0, nullptr,this, nullptr);
    UA_MethodAttributes_deleteMembers(&methodAttr);
    UA_Argument_deleteMembers(&inputArguments[0]);
    UA_Argument_deleteMembers(&inputArguments[1]);
  //  UA_NodeId_deleteMembers(&MethodNodeId);
    UA_QualifiedName_deleteMembers(&MethodQName);
}
UA_StatusCode DTController::SetRampDownCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output) {
 //   UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SetOutput was called");
    DTController* Monitor=static_cast<DTController*>(methodContext);
    if(Monitor->isConnected()){
        UA_Int16 channel = *(UA_Int16*)input[0].data;
        UA_Double voltage = *(UA_Double*)input[1].data;
        DT1415ET::CHANNEL CH=static_cast <DT1415ET::CHANNEL>(channel);
        //Monitor->device.setRampDown(CH,voltage);
        DeviceCommand<DT1415ET> command=std::bind(&DT1415ET::setRampDown, _1,CH,voltage);
        Monitor->buffer.push(command);
    }
    else {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "device disconnected, SetRampDown not send");
    }
    return UA_STATUSCODE_GOOD;
}
void DTController::addSetRampDownMethod(UA_Server *server) {
    UA_Argument inputArguments[2];
    UA_Argument_init(&inputArguments[0]);
    inputArguments[0].description = UA_LOCALIZEDTEXT_ALLOC("en-US", "Channel number");
    inputArguments[0].name = UA_String_fromChars("Channel");
    inputArguments[0].dataType = UA_TYPES[UA_TYPES_INT16].typeId;
    inputArguments[0].valueRank = UA_VALUERANK_SCALAR;

    UA_Argument_init(&inputArguments[1]);
    inputArguments[1].description = UA_LOCALIZEDTEXT_ALLOC("en-US", "V/s");
    inputArguments[1].name = UA_String_fromChars("Ramp down");
    inputArguments[1].dataType = UA_TYPES[UA_TYPES_DOUBLE].typeId;
    inputArguments[1].valueRank = UA_VALUERANK_SCALAR;


    UA_MethodAttributes methodAttr = UA_MethodAttributes_default;
    methodAttr.description = UA_LOCALIZEDTEXT_ALLOC("en-US","Sets channel's ramp down");
    methodAttr.displayName = UA_LOCALIZEDTEXT_ALLOC("en-US","setrampdown");
    methodAttr.executable = true;
    methodAttr.userExecutable = true;
 //   UA_NodeId MethodNodeId=UA_NODEID_STRING_ALLOC(1,"SetVoltage");
    UA_QualifiedName MethodQName= UA_QUALIFIEDNAME_ALLOC(1, "setrampdown");
    UA_Server_addMethodNode(server, UA_NODEID_NULL,
                            ObjectNodeId,
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASORDEREDCOMPONENT),
                            MethodQName,
                            methodAttr, &SetRampDownCallback,
                            2,inputArguments, 0, nullptr,this, nullptr);
    UA_MethodAttributes_deleteMembers(&methodAttr);
    UA_Argument_deleteMembers(&inputArguments[0]);
    UA_Argument_deleteMembers(&inputArguments[1]);
  //  UA_NodeId_deleteMembers(&MethodNodeId);
    UA_QualifiedName_deleteMembers(&MethodQName);
}
