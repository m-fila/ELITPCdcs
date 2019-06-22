#include "abstract_variable.h"

abstract_variable::abstract_variable(std::string name) : VariableName(name)
{
    VariableId=UA_NODEID_STRING_ALLOC(1,VariableName.c_str());
}
abstract_variable::~abstract_variable(){
    UA_NodeId_deleteMembers(&VariableId);
}

