#include "dt1415_variable.h"
#include "sstream"
dt1415_variable::dt1415_variable(std::string OName,std::string VName): template_variable<UA_DT1415m>(OName,VName)
{
}

dt1415_variable::~dt1415_variable(){
 //   UA_NodeId_deleteMembers(&VariableNodeId);
}

std::string dt1415_variable::translateKeys(){
//    std::string str="UA_timestamp INTEGER PRIMARY KEY"
            ", V1 REAL, V2 REAL, C1 REAL, C2 REAL";
//    return str;

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



static std::ostream& operator<<(std::ostream& os, const UA_DT1415m& data){
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
