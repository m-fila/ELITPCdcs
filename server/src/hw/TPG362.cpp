#include "../../include/hw/TPG362.h"

TPG362::TPG362(): GenericDevice(ConnectionType::TCP, ConnectionType::TCP){
}

std::string TPG362::sendWithEqnuiry(std::string command){
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
    return sendWithEqnuiry("AYT");
}

std::string TPG362::getGaugesData(CH ch){
    if(ch==CH::ALL){
        return sendWithEqnuiry("PRX");
    }
    else{
        return(sendWithEqnuiry("PR"+std::to_string(static_cast<int>(ch))));
    }
}