#include "hmpcontroller.h"
//#include "nodemap.h"
#include <cstring>

HMPController::HMPController(std::string name)
{
    VariableType=HMPType;

    ObjectName=new char[name.length() + 1];
    std::strcpy(ObjectName,name.c_str());

    std::string statusname=name+".Status";
    StatusVariableName=new char[name.length() + 1];
    std::strcpy(StatusVariableName,statusname.c_str());

    std::string measurementsname=name+".Measurements";
    MeasurementsVariableName=new char[measurementsname.length() + 1];
    std::strcpy(MeasurementsVariableName,measurementsname.c_str());

    //NodeMap::Nodes.insert(std::pair<string,opc_monitor*>(measurementsname,this));
  //  NodeMap::Nodes.insert(std::pair<string,opc_monitor*>(name,this));

}
HMPController::~HMPController(){
    delete(ObjectName);
    delete(MeasurementsVariableName);
    delete(StatusVariableName);
}


HMPMeasurements HMPController::getMeasurements(){
        string response,ch1_v,ch2_v,ch1_c,ch2_c;
        bool ch1,ch2,output;
        device->setActiveChannel(1);
        ch1_v = device->getVoltage();
        ch1_c = device->getCurrent();
        response = device->getOutputSel();
        ch1 = (response=="1") ? true : false;
        device->setActiveChannel(2);
        ch2_v = device->getVoltage();
        ch2_c = device->getCurrent();
        response = device->getOutputSel();
        ch2 = (response=="1") ? true : false;
        response = device->getOutputGen();
        output = (response=="1") ? true : false;
        return new_HMPMeasurements(ch1,ch2,output,ch1_v,ch2_v,ch1_c,ch2_c);
}


UA_StatusCode HMPController::SetOutputCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SetOutput was called");
    //std::string NodeName=reinterpret_cast<char*>(objectId->identifier.string.data);
    //HMPController* Monitor=dynamic_cast<HMPController*>(NodeMap::Nodes[NodeName]);
    HMPController* Monitor=static_cast<HMPController*>(methodContext);
    UA_Boolean state = *(UA_Boolean*)input->data;
    Monitor->device->setOutputGen(state);
    return UA_STATUSCODE_GOOD;
}
void HMPController::addSetOutputMethod(UA_Server *server) {

    UA_Argument inputArgument;
    UA_Argument_init(&inputArgument);
    inputArgument.description = UA_LOCALIZEDTEXT("en-US", "ON/OFF");
    inputArgument.name = UA_STRING("State");
    inputArgument.dataType = UA_TYPES[UA_TYPES_BOOLEAN].typeId;
    inputArgument.valueRank = UA_VALUERANK_SCALAR;


    UA_MethodAttributes helloAttr = UA_MethodAttributes_default;
    helloAttr.description = UA_LOCALIZEDTEXT("en-US","Sets output ON/OFF");
    helloAttr.displayName = UA_LOCALIZEDTEXT("en-US","setoutput");
    helloAttr.executable = true;
    helloAttr.userExecutable = true;
    UA_Server_addMethodNode(server, UA_NODEID_STRING(1,"SetOutput"),
                            UA_NODEID_STRING(1,ObjectName),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASORDEREDCOMPONENT),
                            UA_QUALIFIEDNAME(1, "setoutput"),
                            helloAttr, &SetOutputCallback,
                            1,&inputArgument, 0, nullptr,this, nullptr);
}

UA_StatusCode HMPController::SetChannelCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SetOutput was called");
   // std::string NodeName=reinterpret_cast<char*>(objectId->identifier.string.data);
   // HMPController* Monitor=dynamic_cast<HMPController*>(NodeMap::Nodes[NodeName]);
    HMPController* Monitor=static_cast<HMPController*>(methodContext);
    UA_Int16 channel = *(UA_Int16*)input[0].data;
    UA_Boolean state = *(UA_Boolean*)input[1].data;
    Monitor->device->setOutputSel(channel,state);
    return UA_STATUSCODE_GOOD;
}
void HMPController::addSetChannelMethod(UA_Server *server) {
    UA_Argument inputArguments[2];
    UA_Argument_init(&inputArguments[0]);
    inputArguments[0].description = UA_LOCALIZEDTEXT("en-US", "Channel number");
    inputArguments[0].name = UA_STRING("Channel");
    inputArguments[0].dataType = UA_TYPES[UA_TYPES_INT16].typeId;
    inputArguments[0].valueRank = UA_VALUERANK_SCALAR;

    UA_Argument_init(&inputArguments[1]);
    inputArguments[1].description = UA_LOCALIZEDTEXT("en-US", "ON/OFF");
    inputArguments[1].name = UA_STRING("State");
    inputArguments[1].dataType = UA_TYPES[UA_TYPES_BOOLEAN].typeId;
    inputArguments[1].valueRank = UA_VALUERANK_SCALAR;


    UA_MethodAttributes helloAttr = UA_MethodAttributes_default;
    helloAttr.description = UA_LOCALIZEDTEXT("en-US","Sets channel ON/OFF");
    helloAttr.displayName = UA_LOCALIZEDTEXT("en-US","setchannel");
    helloAttr.executable = true;
    helloAttr.userExecutable = true;
    UA_Server_addMethodNode(server, UA_NODEID_STRING(1,"SetChannel"),
                            UA_NODEID_STRING(1,ObjectName),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASORDEREDCOMPONENT),
                            UA_QUALIFIEDNAME(1, "setchannel"),
                            helloAttr, &SetChannelCallback,
                            2,inputArguments, 0, nullptr,this, nullptr);
}




