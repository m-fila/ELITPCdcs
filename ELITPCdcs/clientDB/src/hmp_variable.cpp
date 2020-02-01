#include "hmp_variable.h"
//#include "sstream"
HMPVariable::HMPVariable(std::string OName,std::string VName, int size): template_variable<UA_HMPm>(OName,VName), size(size)
{
}



std::string HMPVariable::translateKeys(){
//    std::string str="UA_timestamp INTEGER PRIMARY KEY"
//            ", V1 REAL, V2 REAL, C1 REAL, C2 REAL";
//    return str;

       std::ostringstream oss;
      oss<<"UA_timestamp INTEGER PRIMARY KEY";
    for(int i=0; i!=size;i++){
        oss<<", V"<<i+1<<" REAL";
    }
    for(int i=0; i!=size;i++){
        oss<<", C"<<i+1<<" REAL";
    }
    return oss.str();
}


std::ostream& operator<<(std::ostream& os, const UA_HMPm& data){
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
