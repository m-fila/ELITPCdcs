#include "abstract_variable.h"

abstract_variable::abstract_variable(std::string name) : VariableName(name)
{
    VariableId=UA_NODEID_STRING_ALLOC(1,VariableName.c_str());
}
abstract_variable::~abstract_variable(){
    UA_NodeId_deleteMembers(&VariableId);
}

void abstract_variable::handler_ValueChanged(UA_Client *client, UA_UInt32 subId, void *subContext,
                                             UA_UInt32 monId, void *monContext, UA_DataValue *value){
    abstract_variable *variable=static_cast<abstract_variable*>(monContext);
    variable->updateData(value);
}
