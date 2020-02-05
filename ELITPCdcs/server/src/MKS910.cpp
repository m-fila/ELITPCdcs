#include "MKS910.h"
#include <sstream>
#include <iostream>
MKS910::MKS910() : GenericDevice(ConnectionType::TCP | ConnectionType::Serial, ConnectionType::TCP),delay(200),channel(253){
}

std::string MKS910::encapsulate(std::string content ){
        std::stringstream ss;
        ss<<"@"<<channel<<content<<";FF";
        return ss.str();
}

std::string MKS910::extract(std::string content){
    const std::string beg("@"+std::to_string(channel));
    const std::string end(";FF");
    if (content.find(beg)!=std::string::npos && content.find(end)!=std::string::npos){
        std::string value=content.substr(beg.length()+3, content.length()-beg.length()-3-end.length());
        if(content.find("ACK")!=std::string::npos){
            return value;
        }
        else{
            throw std::runtime_error("Encountered MKS error response");
        }
    }
    else
    {
        throw std::runtime_error("Encountered MKS broken response");
    }
    
}

std::string MKS910::sendWithDelayedResponse(std::string command){
    std::string s=GenericDevice::sendWithDelayedResponse(encapsulate(command),delay);
    std::cout<<encapsulate(command)<<std::endl;
    std::cout<<s<<std::endl;
    return extract(s);
}

std::string MKS910::getStatus(){
    std::string resp=sendWithDelayedResponse("T?");
    if (resp=="O" || resp=="M" || resp=="Z")
        return resp;
    else
    {
        throw std::runtime_error("Encountered unknown MKS910 status code");
    }
    
}

std::string MKS910::getTemp(){
    return sendWithDelayedResponse("TEM?");
}

std::string MKS910::getUnits(){
    return sendWithDelayedResponse("U?");
}

std::string MKS910::getPPirani(){
    return sendWithDelayedResponse("PR1?");
}
std::string MKS910::getPPiezo(){
    return sendWithDelayedResponse("PR2?");
}
std::string MKS910::getPCombined(){
    return sendWithDelayedResponse("PR3?");
}
std::string MKS910::getPCombinedLong(){
    return sendWithDelayedResponse("PR4?");
}
std::string MKS910::getPDiff(){
    return sendWithDelayedResponse("PR5?");
}

std::string MKS910::getFirmware(){
    return sendWithDelayedResponse("FV?");
}
std::string MKS910::getHardware(){
    return sendWithDelayedResponse("HV?");
}
std::string MKS910::getName(){
    return sendWithDelayedResponse("DT?");
}
std::string MKS910::getModel(){
    return sendWithDelayedResponse("MD?");
}
std::string MKS910::getSerialNumber(){
    return sendWithDelayedResponse("SN?");
}
std::string MKS910::getPartNumer(){
    return sendWithDelayedResponse("PN?");
}
std::string MKS910::getUptime(){
    return sendWithDelayedResponse("TIM?");
}

void MKS910::setUnits(std::string u){
    sendWithDelayedResponse("PN!"+u);
}

