#include "hmpcontroller.h"
//#include "nodemap.h"
#include <cstring>

HMPController::HMPController(std::string name): opc_template_controller<HMPMeasurements,HMP2020>(name){
    VariableType=HMPType;
}

HMPController::~HMPController(){
    UA_NodeId_deleteMembers(&ObjectNodeId);
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

HMPMeasurements HMPController::getSettings(){
        string response,ch1_v,ch2_v,ch1_c,ch2_c;
        bool ch1,ch2,output;
        device->setActiveChannel(1);
        ch1_v = device->getVoltageSet();
        ch1_c = device->getCurrentSet();
        response = device->getOutputSel();
        ch1 = (response=="1") ? true : false;
        device->setActiveChannel(2);
        ch2_v = device->getVoltageSet();
        ch2_c = device->getCurrentSet();
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
    HMPController* Monitor=static_cast<HMPController*>(methodContext);
    if(Monitor->isConnected()){
        UA_Boolean state = *(UA_Boolean*)input->data;
        Monitor->device->setOutputGen(state);
    }
    else {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "device disconnected, comand not send");
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
    UA_NodeId MethodNodeId=UA_NODEID_STRING_ALLOC(1,"SetOutput");
    UA_QualifiedName MethodQName= UA_QUALIFIEDNAME_ALLOC(1, "setoutput");
    UA_Server_addMethodNode(server, MethodNodeId,
                            ObjectNodeId,
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASORDEREDCOMPONENT),
                            MethodQName,
                            methodAttr, &SetOutputCallback,
                            1,&inputArgument, 0, nullptr,this, nullptr);
    UA_MethodAttributes_deleteMembers(&methodAttr);
    UA_Argument_deleteMembers(&inputArgument);
    UA_NodeId_deleteMembers(&MethodNodeId);
    UA_QualifiedName_deleteMembers(&MethodQName);
}

UA_StatusCode HMPController::SetChannelCallback(UA_Server *server,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SetOutput was called");
    HMPController* Monitor=static_cast<HMPController*>(methodContext);
    if(Monitor->isConnected()){
        UA_Int16 channel = *(UA_Int16*)input[0].data;
        UA_Boolean state = *(UA_Boolean*)input[1].data;
        Monitor->device->setOutputSel(channel,state);
    }
    else {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "device disconnected, comand not send");
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
    UA_NodeId MethodNodeId=UA_NODEID_STRING_ALLOC(1,"SetChannel");
    UA_QualifiedName MethodQName= UA_QUALIFIEDNAME_ALLOC(1, "setchannel");
    UA_Server_addMethodNode(server, MethodNodeId,
                            ObjectNodeId,
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASORDEREDCOMPONENT),
                            MethodQName,
                            methodAttr, &SetChannelCallback,
                            2,inputArguments, 0, nullptr,this, nullptr);
    UA_MethodAttributes_deleteMembers(&methodAttr);
    UA_Argument_deleteMembers(&inputArguments[0]);
    UA_Argument_deleteMembers(&inputArguments[1]);
    UA_NodeId_deleteMembers(&MethodNodeId);
    UA_QualifiedName_deleteMembers(&MethodQName);
}

void HMPController::addCustomDataType(UA_Server *server, UA_ServerConfig *config) {
        UA_DataType *types = static_cast<UA_DataType*> (UA_malloc(sizeof(UA_DataType)));
        UA_DataTypeMember *members = static_cast<UA_DataTypeMember*>(UA_malloc(sizeof(UA_DataTypeMember) * 7));
        for(int i=0; i!=7;i++){
            members[i] = HMP_members[i];
        }
        types[0] = HMPType;
        types[0].members = members;
        UA_DataTypeArray customDataTypes = {config->customDataTypes, 1, types};
        config->customDataTypes = &customDataTypes;


        char* language=const_cast<char*>("en-Us");
        char* hmpname=const_cast<char*>("HMPMeasurements");
        UA_VariableTypeAttributes dattr = UA_VariableTypeAttributes_default;
        dattr.description = UA_LOCALIZEDTEXT(language, hmpname);
        dattr.displayName = UA_LOCALIZEDTEXT(language, hmpname);
        dattr.dataType = HMPType.typeId;
        dattr.valueRank = UA_VALUERANK_SCALAR;
        HMPMeasurements hmp=new_HMPMeasurements();
        UA_Variant_setScalar(&dattr.value, &hmp, &HMPType);
        UA_QualifiedName TypeQName=UA_QUALIFIEDNAME_ALLOC(1, "HMPMeasurements");
        UA_Server_addVariableTypeNode(server, HMPType.typeId,
                                      UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
                                      UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),
                                      TypeQName,
                                      UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
                                      dattr, nullptr, nullptr);
        UA_QualifiedName_deleteMembers(&TypeQName);
        UA_free(types);
        UA_free(members);
    }



