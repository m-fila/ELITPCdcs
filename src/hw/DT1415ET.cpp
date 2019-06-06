#include "DT1415ET.h"
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

string DT1415ET::sendDT1415ETcommand(CMD command, CHANNEL channel, string function, string value)
{
    std::stringstream cb;
    //CMD value SET OR mon
    cb << "$CMD:" << CommandsMap.at(command);

    //add CH: if command has channel specified
    if(channel!=CHANNEL::NONE)
        cb << ",CH:" << (int)channel;

    // command parameter (function)
    cb << ",PAR:" << function;

    //add value if string "value" not empty
    if(!value.empty())
        cb << ",VAL:" << value;

    //add \cr\lf at the end
    cb << "\n";

    //send command using opened connection and return response
    return sendWithResponse(cb.str());
}

string DT1415ET::extractCommandStatus(string command)
{
    return "OK";
}

string DT1415ET::extractCommandValue(string command)
{
    //TODO: extract command status, if error throw exception, if no, extract value
    command.erase(command.length()-1);
    std::size_t pos = command.find("VAL:");
    string ret = command;
    if(pos!=string::npos)
        ret = command.substr(pos+4);

    return ret;
}

string DT1415ET::getModuleName()
{
    string resp = sendDT1415ETcommand(CMD::MON, CHANNEL::NONE, "BDNAME", "");
    return extractCommandValue(resp);
}

string DT1415ET::getFirmwareVersion()
{
    string resp = sendDT1415ETcommand(CMD::MON, CHANNEL::NONE, "BDFREL", "");
    return extractCommandValue(resp);
}

string DT1415ET::getSerialNumber()
{
    string resp = sendDT1415ETcommand(CMD::MON, CHANNEL::NONE, "BDSNUM", "");
    return extractCommandValue(resp);
}

string DT1415ET::getControlMode()
{
    string resp = sendDT1415ETcommand(CMD::MON, CHANNEL::NONE, "BDCTR", "");
    return extractCommandValue(resp);
}

bool DT1415ET::isRemote()
{
    string mode = getControlMode();
    if(!mode.compare("REMOTE"))
        return true;
    else
        return false;
}

string DT1415ET::getIdentifier()
{
    std::stringstream cb;
    cb << "Module name: " << getModuleName() << " | ";
    cb << "Firmware version: " << getFirmwareVersion() << " | ";
    cb << "S/N: " << getSerialNumber();

    return cb.str();
}

string DT1415ET::getVoltageSet(CHANNEL channel)
{
    string resp = sendDT1415ETcommand(CMD::MON, channel, "VSET", "");
    return extractCommandValue(resp);
}

string DT1415ET::getVoltage(CHANNEL channel)
{
    string resp = sendDT1415ETcommand(CMD::MON, channel, "VMON", "");
    return extractCommandValue(resp);
}

string DT1415ET::getStatus(CHANNEL channel)
{
    string resp = sendDT1415ETcommand(CMD::MON, channel, "STAT", "");
    return extractCommandValue(resp);
}

string DT1415ET::getCurrent(CHANNEL channel)
{
    string resp = sendDT1415ETcommand(CMD::MON, channel, "IMON", "");
    return extractCommandValue(resp);
}

string DT1415ET::getCurrentSet(CHANNEL channel)
{
    string resp = sendDT1415ETcommand(CMD::MON, channel, "ISET", "");
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

void DT1415ET::setVoltageMax(CHANNEL channel, string value)
{

}
