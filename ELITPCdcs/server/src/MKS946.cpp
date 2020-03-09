#include "MKS946.h"
#include <sstream>
#include <iostream>
MKS946::MKS946() : MKS(){
}

std::string MKS946::getPressure(CH ch){
    std::string cmd="PR"+std::to_string(static_cast<int>(ch))+"?";
    return sendWithDelayedResponse(cmd);
}
std::string MKS946::getPressureAll(){
    return sendWithDelayedResponse("PRZ?");
}
std::string MKS946::getFlow(CH ch){
    std::string cmd="FR"+std::to_string(static_cast<int>(ch))+"?";
    return sendWithDelayedResponse(cmd);
}

std::string MKS946::getFlowFactor(CH ch){
    std::string cmd="QSF"+std::to_string(static_cast<int>(ch))+"?";
    return sendWithDelayedResponse(cmd);
}

std::string MKS946::getFlowNominalRange(CH ch){
    std::string cmd="RNG"+std::to_string(static_cast<int>(ch))+"?";
    return sendWithDelayedResponse(cmd);
}

void MKS946::zeroMFC(CH ch){
    std::string cmd="QZ"+std::to_string(static_cast<int>(ch))+"?";
    sendWithDelayedResponse(cmd);
}

std::string MKS946::getFlowPoint(CH ch){
    std::string cmd="QSP"+std::to_string(static_cast<int>(ch))+"?";
    return sendWithDelayedResponse(cmd);
}

std::string MKS946::getFlowMode(CH ch){
    std::string cmd="QMD"+std::to_string(static_cast<int>(ch))+"?";
    return sendWithDelayedResponse(cmd);
}


std::string MKS946::getActiveRatioRecipe(){
    std::string cmd="RCP?";
    return sendWithDelayedResponse(cmd);
}
std::string MKS946::getActivePIDRecipe(){
    std::string cmd="RRCP?";
    return sendWithDelayedResponse(cmd);
}



std::string MKS946::getModuleType(){
    std::string cmd="MT?";
    return sendWithDelayedResponse(cmd);
}
std::string MKS946::getSensorType(MODULE m){
    std::string n;
    switch(m){
        case MODULE::A : n="A"; break;
        case MODULE::B : n="B"; break;
        case MODULE::C : n="C"; break;
    }
    std::string cmd="ST"+n;
    return sendWithDelayedResponse(cmd);
}



std::string MKS946::getFirmware(){
    return sendWithDelayedResponse("FV?");
}
std::string MKS946::getHardware(){
    return sendWithDelayedResponse("HV?");
}
std::string MKS946::getName(){
    return sendWithDelayedResponse("DT?");
}
std::string MKS946::getModel(){
    return sendWithDelayedResponse("MD?");
}
std::string MKS946::getSerialNumber(){
    return sendWithDelayedResponse("SN?");
}
std::string MKS946::getPartNumer(){
    return sendWithDelayedResponse("PN?");
}
std::string MKS946::getUptime(){
    return sendWithDelayedResponse("TIM?");
}

void MKS946::setUnits(MKS946codes::Units u){
    sendWithDelayedResponse("U!"+MKS946codes::unitsToString.at(u));
}

