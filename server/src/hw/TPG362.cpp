#include "../../include/hw/TPG362.h"

TPG362::TPG362(): GenericDevice(ConnectionType::TCP, ConnectionType::TCP){
}

std::string TPG362::sendWithEnquiry(std::string command){
    std::string r=sendWithResponse(command+"\r");
    if (static_cast<int>(r.at(0)==6)){
       //6->ACK
       return sendWithResponse(enq); 
    }
    else{
        //21->NACK
    }
}

std::string TPG362::getIdentifier(){
    return sendWithEnquiry("AYT");
}

std::string TPG362::getGaugesData(CH ch){
    if(ch==CH::ALL){
        return sendWithEnquiry("PRX");
    }
    else{
        return(sendWithEnquiry("PR"+std::to_string(static_cast<int>(ch))));
    }
}
std::string TPG362::getGaugesIdentification(){
    return sendWithEnquiry("TID");
}
std::string TPG362::getUnits(){
    return sendWithEnquiry("UNI");
}

std::string TPG362::getSwitchingFunctions(){
    return sendWithEnquiry("SPS");
}
std::string TPG362::getSwitchingFunction(SWITCHING_FUNCTION f){
    int F=static_cast<int>(f);
    return sendWithEnquiry("SP"+std::to_string(F));
}

std::string TPG362::getFilter(){
    return sendWithEnquiry("FIL");
}

std::string TPG362::setFormat(FORMAT format){
    int f=static_cast<int>(format);
    return sendWithEnquiry("FMT,"+std::to_string(f));
}

std::string TPG362::setGaugesStatus(STATUS s1,STATUS s2){
    int S1=static_cast<int>(s1);
    int S2=static_cast<int>(s2);
    return sendWithEnquiry("SEN,"+std::to_string(S1)+","+std::to_string(S2));
}

std::string TPG362::setUnits(UNIT unit){
    int u=static_cast<int>(unit);
    return sendWithEnquiry("UNI,"+std::to_string(u));
}

std::string TPG362::setDisplayResolution(int r1,int r2){
    return sendWithEnquiry("DCD,"+std::to_string(r1)+","+std::to_string(r2));
}
std::string TPG362::setFilter(FILTER f1,FILTER f2){
    int F1 =static_cast<int>(f1);
    int F2 =static_cast<int>(f2);
    return sendWithEnquiry("FIL,"+std::to_string(F1)+","+std::to_string(F2));
}
