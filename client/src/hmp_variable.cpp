#include "../include/hmp_variable.h"

hmp_variable::hmp_variable(std::string name): template_variable<HMPMeasurements>(name)
{
}

hmp_variable::~hmp_variable(){
    UA_NodeId_deleteMembers(&VariableId);
}

std::string hmp_variable::translateName(){
int custom_size=HMPType.membersSize;
std::ostringstream oss;
for(int i=0; i!=custom_size;i++){
        oss<<VariableName+"."+HMP_members[i].memberName<<" ";
    }
return oss.str();
}
