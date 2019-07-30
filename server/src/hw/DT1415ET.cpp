#include "../../include/hw/DT1415ET.h"
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

DT1415ET::DT1415ET() : GenericDevice(ConnectionType::TCP, ConnectionType::TCP)
{

}

DT1415ET::~DT1415ET()
{

}

std::string DT1415ET::sendDT1415ETcommand(CMD command, CHANNEL channel, std::string function,std::string value)
{
    std::stringstream cb;
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

std::string DT1415ET::extractCommandStatus(std::string command)
{
    return "OK";
}

std::string DT1415ET::extractCommandValue(std::string command)
{
    //TODO: extract command status, if error throw exception, if no, extract value
    command.erase(command.length()-1);
    std::size_t pos = command.find("VAL:");
    std::string ret = command;
    if(pos!=std::string::npos)
        ret = command.substr(pos+4);

    return ret;
}

std::string DT1415ET::getModuleName()
{
    std::string resp = sendDT1415ETcommand(CMD::MON, CHANNEL::NONE, "BDNAME", "");
    return extractCommandValue(resp);
}

std::string DT1415ET::getFirmwareVersion()
{
    std::string resp = sendDT1415ETcommand(CMD::MON, CHANNEL::NONE, "BDFREL", "");
    return extractCommandValue(resp);
}

std::string DT1415ET::getSerialNumber()
{
    std::string resp = sendDT1415ETcommand(CMD::MON, CHANNEL::NONE, "BDSNUM", "");
    return extractCommandValue(resp);
}

std::string DT1415ET::getControlMode()
{
    std::string resp = sendDT1415ETcommand(CMD::MON, CHANNEL::NONE, "BDCTR", "");
    return extractCommandValue(resp);
}

bool DT1415ET::isRemote()
{
    std::string mode = getControlMode();
    if(!mode.compare("REMOTE"))
        return true;
    else
        return false;
}

std::string DT1415ET::getIdentifier()
{
    std::stringstream cb;
    cb << "Module name: " << getModuleName() << " | ";
    cb << "Firmware version: " << getFirmwareVersion() << " | ";
    cb << "S/N: " << getSerialNumber();

    return cb.str();
}

std::string DT1415ET::getVoltageSet(CHANNEL channel)
{
    std::string resp = sendDT1415ETcommand(CMD::MON, channel, "VSET", "");
    return extractCommandValue(resp);
}

std::string DT1415ET::getVoltageMax(CHANNEL channel)
{
    std::string resp = sendDT1415ETcommand(CMD::MON, channel, "VMAX", "");
    return extractCommandValue(resp);
}

std::string DT1415ET::getRampUp(CHANNEL channel)
{
    std::string resp = sendDT1415ETcommand(CMD::MON, channel, "RUP", "");
    return extractCommandValue(resp);
}
std::string DT1415ET::getRampDown(CHANNEL channel)
{
    std::string resp = sendDT1415ETcommand(CMD::MON, channel, "RDW", "");
    return extractCommandValue(resp);
}

std::string DT1415ET::getVoltage(CHANNEL channel)
{
    std::string resp = sendDT1415ETcommand(CMD::MON, channel, "VMON", "");
    return extractCommandValue(resp);
}

std::string DT1415ET::getStatus(CHANNEL channel)
{
    std::string resp = sendDT1415ETcommand(CMD::MON, channel, "STATUS", "");
    return extractCommandValue(resp);
}

std::string DT1415ET::getCurrent(CHANNEL channel)
{
    std::string resp = sendDT1415ETcommand(CMD::MON, channel, "IMON", "");
    return extractCommandValue(resp);
}

std::string DT1415ET::getCurrentSet(CHANNEL channel)
{
    std::string resp = sendDT1415ETcommand(CMD::MON, channel, "ISET", "");
    return extractCommandValue(resp);
}

// set functions
void DT1415ET::setON(CHANNEL channel)
{
    if(isRemote())
        sendDT1415ETcommand(CMD::SET, channel, "ON", "");
}

void DT1415ET::setOFF(CHANNEL channel)
{
    if(isRemote())
        sendDT1415ETcommand(CMD::SET, channel, "OFF", "");
}

void DT1415ET::setVoltageSet(CHANNEL channel, double value)
{
    if(isRemote())
    {
        std::ostringstream sb;
        sb << std::setprecision(1) << std::fixed << std::setw(6) << value;
        //std::cout << sb.str() << std::endl;
        sendDT1415ETcommand(CMD::SET, channel, "VSET", sb.str());
    }
}

void DT1415ET::setVoltageMax(CHANNEL channel, double value)
{
    if(isRemote())
    {
        std::ostringstream sb;
        sb << std::setprecision(1) << std::fixed << std::setw(6) << value;
        //std::cout << sb.str() << std::endl;
        sendDT1415ETcommand(CMD::SET, channel, "SVMAX", sb.str());
    }
}

void DT1415ET::setRampUp(CHANNEL channel, double value)
{
    if(isRemote())
    {
        std::ostringstream sb;
        sb << std::setprecision(1) << std::fixed << std::setw(6) << value;
        //std::cout << sb.str() << std::endl;
        sendDT1415ETcommand(CMD::SET, channel, "RUP", sb.str());
    }
}
void DT1415ET::setRampDown(CHANNEL channel, double value)
{
    if(isRemote())
    {
        std::ostringstream sb;
        sb << std::setprecision(1) << std::fixed << std::setw(6) << value;
        //std::cout << sb.str() << std::endl;
        sendDT1415ETcommand(CMD::SET, channel, "RDW", sb.str());
    }
}
