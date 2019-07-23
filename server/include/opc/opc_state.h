#ifndef OPC_STATE_H
#define OPC_STATE_H

#include <open62541/server.h>
#include <string>
#include "opc_object.h"
class opc_state:  opc_object
{
public:

    enum MachineState : UA_Int32 {
        Idle,
        Pumping,
        Redy,
    };

    opc_state();
    ~opc_state();
    MachineState state;
    void init(UA_Server *server);

private:

    static UA_StatusCode SetStateCallback(UA_Server *server,
                             const UA_NodeId *sessionId, void *sessionHandle,
                             const UA_NodeId *methodId, void *methodContext,
                             const UA_NodeId *objectId, void *objectContext,
                             size_t inputSize, const UA_Variant *input,
                             size_t outputSize, UA_Variant *output);
    void setState(UA_Server *server, MachineState State);
    void addSetStateMethod(UA_Server *server);
    std::string VariableName;
    UA_NodeId VariableNodeId;
};

#endif // OPC_STATE_H
