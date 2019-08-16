#ifndef HMPCONTROLLER_H
#define HMPCONTROLLER_H
#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include "../DeviceController.h"
#include "../hw/HMP2020.h"
#include "opc_controller.h"

#include "open62541/types_dcsnodeset_generated.h"
#include "open62541/types_dcsnodeset_generated_handling.h"
class HMP2020Controller :public OpcTemplateController<UA_HMPm,UA_HMPc,HMP2020>
{
public:
    HMP2020Controller(std::string name);
//    ~HMPController();
    void init(UA_Server *server);

    void addSetChannelMethod(UA_Server *server);
    void addSetOutputMethod(UA_Server *server);

private:
    UA_HMPm getMeasurements() final;
    UA_HMPc getSettings() final;

    static UA_StatusCode setChannelCallback(UA_Server *server,
                             const UA_NodeId *sessionId, void *sessionHandle,
                             const UA_NodeId *methodId, void *methodContext,
                             const UA_NodeId *objectId, void *objectContext,
                             size_t inputSize, const UA_Variant *input,
                             size_t outputSize, UA_Variant *output);
    static UA_StatusCode setOutputCallback(UA_Server *server,
                             const UA_NodeId *sessionId, void *sessionHandle,
                             const UA_NodeId *methodId, void *methodContext,
                             const UA_NodeId *objectId, void *objectContext,
                             size_t inputSize, const UA_Variant *input,
                             size_t outputSize, UA_Variant *output);

};

#endif // HMPCONTROLLER_H
