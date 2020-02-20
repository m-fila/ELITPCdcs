#ifndef MKS910CONTROLLER_H
#define MKS910CONTROLLER_H
#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include "MKS910.h"
#include "opc_controller.h"

#include "open62541/types_dcsnodeset_generated.h"
#include "open62541/types_dcsnodeset_generated_handling.h"
class MKS910Controller :public OpcTemplateController<UA_MKS910m,UA_MKS910c,MKS910>
{
public:
    MKS910Controller(std::string name);
    
    void init(UA_Server *server);
    void addSetUnitsMethod(UA_Server *server);
private:
    UA_MKS910m getMeasurements() final;
    UA_MKS910c getSettings() final;

    static UA_StatusCode setUnitsCallback(UA_Server *server,
                             const UA_NodeId *sessionId, void *sessionHandle,
                             const UA_NodeId *methodId, void *methodContext,
                             const UA_NodeId *objectId, void *objectContext,
                             size_t inputSize, const UA_Variant *input,
                             size_t outputSize, UA_Variant *output);    
};
#endif // TPG362CONTROLLER_H
