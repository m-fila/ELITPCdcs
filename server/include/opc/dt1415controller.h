#ifndef DTCONTROLLER_H
#define DTCONTROLLER_H

//#include <open62541/plugin/log_stdout.h>
//#include <open62541/server.h>
#include "../DeviceController.h"
#include "../hw/DT1415ET.h"
#include "opc_controller.h"
#include "../../../common/DTmDataType.h"
#include "../../../common/DTcDataType.h"
#include "open62541/types_dcsnodeset_generated.h"
#include "open62541/types_dcsnodeset_generated_handling.h"
class DT1415Controller :public OpcTemplateController<UA_DT1415m,UA_DT1415c,DT1415ET>
{
public:
    DT1415Controller(std::string name);
//    ~DT1415Controller();
    void init(UA_Server *server);
    void addSetChannelMethod(UA_Server *server);
    void addSetVoltageMethod(UA_Server *server);
    void addSetVoltageMaxMethod(UA_Server *server);
    void addSetRampUpMethod(UA_Server *server);
    void addSetRampDownMethod(UA_Server *server);

private:
    UA_DT1415m getMeasurements() final;
    UA_DT1415c getSettings() final;

    static UA_StatusCode setRampDownCallback(UA_Server *server,
                             const UA_NodeId *sessionId, void *sessionHandle,
                             const UA_NodeId *methodId, void *methodContext,
                             const UA_NodeId *objectId, void *objectContext,
                             size_t inputSize, const UA_Variant *input,
                             size_t outputSize, UA_Variant *output);
    static UA_StatusCode setRampUpCallback(UA_Server *server,
                             const UA_NodeId *sessionId, void *sessionHandle,
                             const UA_NodeId *methodId, void *methodContext,
                             const UA_NodeId *objectId, void *objectContext,
                             size_t inputSize, const UA_Variant *input,
                             size_t outputSize, UA_Variant *output);
    static UA_StatusCode setVoltageMaxCallback(UA_Server *server,
                             const UA_NodeId *sessionId, void *sessionHandle,
                             const UA_NodeId *methodId, void *methodContext,
                             const UA_NodeId *objectId, void *objectContext,
                             size_t inputSize, const UA_Variant *input,
                             size_t outputSize, UA_Variant *output);
    static UA_StatusCode setVoltageCallback(UA_Server *server,
                             const UA_NodeId *sessionId, void *sessionHandle,
                             const UA_NodeId *methodId, void *methodContext,
                             const UA_NodeId *objectId, void *objectContext,
                             size_t inputSize, const UA_Variant *input,
                             size_t outputSize, UA_Variant *output);
    static UA_StatusCode setChannelCallback(UA_Server *server,
                             const UA_NodeId *sessionId, void *sessionHandle,
                             const UA_NodeId *methodId, void *methodContext,
                             const UA_NodeId *objectId, void *objectContext,
                             size_t inputSize, const UA_Variant *input,
                             size_t outputSize, UA_Variant *output);
};



#endif // DTCONTROLLER_H
