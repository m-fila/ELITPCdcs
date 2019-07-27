#ifndef HMPCONTROLLER_H
#define HMPCONTROLLER_H
#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include "../DeviceController.h"
#include "../hw/HMP2020.h"
#include "opc_controller.h"
#include "../../../common/HMPDataType.h"

class HMPController :public opc_template_controller<HMPMeasurements,HMPMeasurements,HMP2020>
{
public:
    HMPController(std::string name);
//    ~HMPController();
    hmp_customType customType;
    void init(UA_Server *server);

    void addSetChannelMethod(UA_Server *server);
    void addSetOutputMethod(UA_Server *server);

private:
    HMPMeasurements getMeasurements();
    HMPMeasurements getSettings();

    static UA_StatusCode SetChannelCallback(UA_Server *server,
                             const UA_NodeId *sessionId, void *sessionHandle,
                             const UA_NodeId *methodId, void *methodContext,
                             const UA_NodeId *objectId, void *objectContext,
                             size_t inputSize, const UA_Variant *input,
                             size_t outputSize, UA_Variant *output);
    static UA_StatusCode SetOutputCallback(UA_Server *server,
                             const UA_NodeId *sessionId, void *sessionHandle,
                             const UA_NodeId *methodId, void *methodContext,
                             const UA_NodeId *objectId, void *objectContext,
                             size_t inputSize, const UA_Variant *input,
                             size_t outputSize, UA_Variant *output);

};

#endif // HMPCONTROLLER_H
