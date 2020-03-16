#include "N1471.h"
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

N1471::N1471() : GenericDevice(ConnectionType::TCP, ConnectionType::TCP), moduleAddress(0)
{

}

void N1471::setModuleAddress(int nr){
    if(nr>=0 && nr<=31){
        moduleAddress=nr;
    }  
}

std::string N1471::sendN1471command(CMD command, CHANNEL channel, std::string function,std::string value)
{
    std::stringstream cb;
    //module number 0-31
    cb<<"$BD:" << moduleAddress<<",";
    //CMD value SET OR mon
    cb << "$CMD:" << CommandsMap.at(command);

    //add CH: if command has channel specified
    if(channel!=CHANNEL::NONE)
        cb << ",CH:" << (int)channel;

    // command parameter (function)
    cb << ",PAR:" << function;

    //add value if std::string "value" not empty
    if(!value.empty())
        cb << ",VAL:" << value;

    //add \cr\lf at the end
    cb << "\n";

    //send command using opened connection and return response
    return sendWithResponse(cb.str());
}

std::string N1471::extractCommandStatus(std::string command)
{
    return "OK";
}

std::string N1471::extractCommandValue(std::string command)
{
    //TODO: extract command status, if error throw exception, if no, extract value
    command.erase(command.length()-1);
    std::size_t pos = command.find("VAL:");
    std::string ret = command;
    if(pos!=std::string::npos)
        ret = command.substr(pos+4);

    return ret;
}

std::string N1471::getModuleName()
{
    std::string resp = sendN1471command(CMD::MON, CHANNEL::NONE, "BDNAME", "");
    return extractCommandValue(resp);
}

std::string N1471::getFirmwareVersion()
{
    std::string resp = sendN1471command(CMD::MON, CHANNEL::NONE, "BDFREL", "");
    std::string fwv=extractCommandValue(resp);
    return fwv;
}

std::string N1471::getSerialNumber()
{
    std::string resp = sendN1471command(CMD::MON, CHANNEL::NONE, "BDSNUM", "");
    return extractCommandValue(resp);
}

std::string N1471::getControlMode()
{
    std::string resp = sendN1471command(CMD::MON, CHANNEL::NONE, "BDCTR", "");
    return extractCommandValue(resp);
}

bool N1471::isRemote()
{
    std::string mode = getControlMode();
    if(!mode.compare("REMOTE"))
        return true;
    else
        return false;
}

std::string N1471::getIdentifier()
{
    std::stringstream cb;
    cb << "Module name: " << getModuleName() << " | ";
    cb << "Firmware version: " << getFirmwareVersion() << " | ";
    cb << "S/N: " << getSerialNumber();

    return cb.str();
}

std::string N1471::getVoltageSet(CHANNEL channel)
{
    std::string resp = sendN1471command(CMD::MON, channel, "VSET", "");
    return extractCommandValue(resp);
}

std::string N1471::getCurrentSet(CHANNEL channel)
{
    std::string resp = sendN1471command(CMD::MON, channel, "ISET", "");
    return extractCommandValue(resp);
}

std::string N1471::getVoltageMax(CHANNEL channel)
{
    std::string resp = sendN1471command(CMD::MON, channel, "MAXV", "");
    return extractCommandValue(resp);
}

std::string N1471::getRampUp(CHANNEL channel)
{
    std::string resp = sendN1471command(CMD::MON, channel, "RUP", "");
    return extractCommandValue(resp);
}

std::string N1471::getRampDown(CHANNEL channel) 
{   
    std::string resp = sendN1471command(CMD::MON, channel, "RDW", "");
    return extractCommandValue(resp);
}

std::string N1471::getVoltage(CHANNEL channel)
{
    std::string resp = sendN1471command(CMD::MON, channel, "VMON", "");
    return extractCommandValue(resp);
}

std::string N1471::getStatus(CHANNEL channel)
{
    std::string resp = sendN1471command(CMD::MON, channel, "STAT", "");
    return extractCommandValue(resp);
}

std::string N1471::getCurrent(CHANNEL channel)
{
    std::string resp = sendN1471command(CMD::MON, channel, "IMON", "");
    return extractCommandValue(resp);
}

std::string N1471::getPolarity(CHANNEL channel)
{
    std::string resp = sendN1471command(CMD::MON, channel, "POL", "");
    return extractCommandValue(resp);
}

// set functions
void N1471::setON(CHANNEL channel)
{
    if(isRemote())
        sendN1471command(CMD::SET, channel, "ON", "");
}

void N1471::setOFF(CHANNEL channel)
{
    if(isRemote())
        sendN1471command(CMD::SET, channel, "OFF", "");
}

void N1471::setVoltageSet(CHANNEL channel, double value)
{
    if(isRemote())
    {
        std::ostringstream sb;
        sb << std::setprecision(1) << std::fixed << std::setw(6) << value;
        sendN1471command(CMD::SET, channel, "VSET", sb.str());
    }
}

void N1471::setCurrentSet(CHANNEL channel, double value)
{
    if(isRemote())
    {
        std::ostringstream sb;
        sb << std::setprecision(1) << std::fixed << std::setw(6) << value;
        sendN1471command(CMD::SET, channel, "ISET", sb.str());
    }
}

void N1471::setVoltageMax(CHANNEL channel, double value)
{
    if(isRemote())
    {
        std::ostringstream sb;
        sb << std::setprecision(1) << std::fixed << std::setw(6) << value;
        sendN1471command(CMD::SET, channel, "MAXV", sb.str());
    }
}

void N1471::setRampUp(CHANNEL channel, double value)
{
    if(isRemote())
    {
        std::ostringstream sb;
        sb << std::setprecision(1) << std::fixed << std::setw(6) << value;
        sendN1471command(CMD::SET, channel, "RUP", sb.str());
    }
}
void N1471::setRampDown(CHANNEL channel, double value)
{
    if(isRemote())
    {
        std::ostringstream sb;
        sb << std::setprecision(1) << std::fixed << std::setw(6) << value;
        sendN1471command(CMD::SET, channel, "RDW", sb.str());
    }
}
