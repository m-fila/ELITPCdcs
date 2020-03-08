#include "MKS910.h"
#include <sstream>
#include <iostream>
MKS910::MKS910() : MKS(){
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

void MKS910::setUnits(MKS910codes::Units u){
    sendWithDelayedResponse("U!"+MKS910codes::unitsToString.at(u));
}

