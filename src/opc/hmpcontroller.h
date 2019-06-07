#ifndef HMPCONTROLLER_H
#define HMPCONTROLLER_H
#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include "../DeviceController.h"
#include "../hw/HMP2020.h"
#include "opc_controller.h"
#include "HMPDataType.h"

class HMPController :public opc_template_controller<HMPMeasurements,HMP2020>
{
public:
    HMPController(std::string name);
    ~HMPController();
    void addVariable(UA_Server *server){opc_template_controller::addVariable(server,new_HMPMeasurements());}
    HMPMeasurements getMeasurements();
    //HMP specific methods:
    static UA_StatusCode SetOutputCallback(UA_Server *server,
                             const UA_NodeId *sessionId, void *sessionHandle,
                             const UA_NodeId *methodId, void *methodContext,
                             const UA_NodeId *objectId, void *objectContext,
                             size_t inputSize, const UA_Variant *input,
                             size_t outputSize, UA_Variant *output);
    void addSetOutputMethod(UA_Server *server);
    static UA_StatusCode SetChannelCallback(UA_Server *server,
                             const UA_NodeId *sessionId, void *sessionHandle,
                             const UA_NodeId *methodId, void *methodContext,
                             const UA_NodeId *objectId, void *objectContext,
                             size_t inputSize, const UA_Variant *input,
                             size_t outputSize, UA_Variant *output);
    void addSetChannelMethod(UA_Server *server);

};

#endif // HMPCONTROLLER_H
