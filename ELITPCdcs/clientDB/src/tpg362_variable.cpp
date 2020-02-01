#include "tpg362_variable.h"
//#include "sstream"
TPG362Variable::TPG362Variable(std::string OName,std::string VName): template_variable<UA_TPG362m>(OName,VName)
{
}



std::string TPG362Variable::translateKeys(){
//    std::string str="UA_timestamp INTEGER PRIMARY KEY"
 //           ", S1 REAL, S2 REAL, C1 REAL, C2 REAL";
//    return str;

       std::ostringstream oss;
      oss<<"UA_timestamp INTEGER PRIMARY KEY";
    for(int i=1; i!=3;i++){
        oss<<", S"<<i<<" REAL";
    }
    for(int i=1; i!=3;i++){
        oss<<", P"<<i<<" REAL";
    }
    return oss.str();
}


std::ostream& operator<<(std::ostream& os, const UA_TPG362m& data){
    int size=data.statusSize;
    for (int i=0;i<size;++i){
        if(i){
            os<<", ";
        }
        os<<data.status[i];
    }
    for (int i=0;i<size;++i){
        os<<", "<<data.vacuum[i];
    }
    return os;
}
