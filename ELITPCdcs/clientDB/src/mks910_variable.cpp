#include "mks910_variable.h"
//#include "sstream"
MKS910Variable::MKS910Variable(std::string OName,std::string VName): template_variable<UA_MKS910m>(OName,VName)
{
}

std::string MKS910Variable::translateKeys(){
    std::ostringstream oss;
    oss<<"UA_timestamp INTEGER PRIMARY KEY"
    <<", vacuum"<<" REAL"
    <<", temperature"<<" REAL"
    <<", status"<<" REAL"
    <<", units"<<" REAL";
    return oss.str();
}

std::ostream& operator<<(std::ostream& os, const UA_MKS910m& data){
    os<<data.vacuum
    <<", "<<data.temperature
    <<", "<<data.status
    <<", "<<data.units;
    return os;
}
