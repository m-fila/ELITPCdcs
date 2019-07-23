#include "../include/hmp_variable.h"

hmp_variable::hmp_variable(std::string OName,std::string VName): template_variable<HMPMeasurements>(OName,VName)
{
}

hmp_variable::~hmp_variable(){
 //   UA_NodeId_deleteMembers(&VariableNodeId);
}

std::string hmp_variable::translateKeys(){
    int custom_size=customType.Type.membersSize;
    std::ostringstream oss;
    oss<<"UA_timestamp INTEGER PRIMARY KEY";
    for(int i=0; i!=custom_size;i++){
        oss<<", "<<customType.members[i].memberName<<" REAL";
    }
    return oss.str();

}
//std::string hmp_variable::translateName(){
    //int custom_size=customType.Type.membersSize;
//std::ostringstream oss;
//for(int i=0; i!=custom_size;i++){
//        oss<<VariableName+"."+customType.members[i].memberName<<" ";
//    }
//return oss.str();
//}

