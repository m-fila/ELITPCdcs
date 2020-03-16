#include "dt1415_variable.h"
#include "sstream"
caen_variable::caen_variable(std::string OName,std::string VName, int size): template_variable<UA_DT1415m>(OName,VName), size(size)
{
}

caen_variable::~caen_variable(){
 //   UA_NodeId_deleteMembers(&VariableNodeId);
}

std::string caen_variable::translateKeys(){
    std::ostringstream oss;
    oss<<"UA_timestamp INTEGER PRIMARY KEY";
    for(int i=0; i!=size;i++){
        oss<<", V"<<i<<" REAL";
    }
    for(int i=0; i!=size;i++){
        oss<<", C"<<i<<" REAL";
    }
    return oss.str();
}


std::ostream& operator<<(std::ostream& os, const UA_DT1415m& data){
    int size=data.voltageSize;
    for (int i=0;i<size;++i){
        if(i){
            os<<", ";
        }
        os<<data.voltage[i];
    }
    for (int i=0;i<size;++i){
        os<<", "<<data.current[i];
    }
    return os;
}
