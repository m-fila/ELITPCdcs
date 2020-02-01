#include "configWrapper.h"
// default constructor
ConfigWrapper::ConfigWrapper(){
      builder["collectComments"] = false;
}

Json::Value& ConfigWrapper::parse(std::stringstream& ss){
    Json::parseFromStream(builder,ss,&obj,&errs);
    return obj;
}

Json::Value& ConfigWrapper::parse(std::string& str){
    std::stringstream ss(str);
    return parse(ss);    
}


Json::Value& ConfigWrapper::parse(std::ifstream& ifs){
    std::stringstream buffer;
    buffer << ifs.rdbuf();
    return parse(buffer);
}