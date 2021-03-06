#include "DT1415ET.h"
#include <sstream>
#include <string>

#include <iomanip>

DT1415ET::DT1415ET()
    : DCSGenericDevice(ConnectionType::TCP, ConnectionType::TCP),
      firmwareVersion("1.10") {}

DT1415ET::~DT1415ET() {}

void DT1415ET::setConnectionStream(ConnectionStream *stream) {
    DCSGenericDevice::setConnectionStream(stream);
    if(connected) {
        setFirmwareVersion();
    }
}

std::string DT1415ET::sendDT1415ETcommand(CMD command, CHANNEL channel,
                                          std::string function, std::string value) {
    std::stringstream cb;
    // CMD value SET OR mon
    cb << "$CMD:" << CommandsMap.at(command);

    // add CH: if command has channel specified
    if(channel != CHANNEL::NONE)
        cb << ",CH:" << (int)channel;

    // command parameter (function)
    cb << ",PAR:" << function;

    // add value if std::string "value" not empty
    if(!value.empty())
        cb << ",VAL:" << value;

    // add \cr\lf at the end
    cb << "\n";

    // send command using opened connection and return response
    return sendWithResponse(cb.str());
}

std::string DT1415ET::extractCommandStatus(std::string command) { return "OK"; }

std::string DT1415ET::extractCommandValue(std::string command) {
    // TODO: extract command status, if error throw exception, if no, extract
    // value
    command.erase(command.length() - 1);
    std::size_t pos = command.find("VAL:");
    std::string ret = command;
    if(pos != std::string::npos)
        ret = command.substr(pos + 4);

    return ret;
}

std::string DT1415ET::getModel() {
    std::string resp = sendDT1415ETcommand(CMD::MON, CHANNEL::NONE, "BDNAME", "");
    return extractCommandValue(resp);
}

std::string DT1415ET::getFirmwareVersion() {
    std::string resp = sendDT1415ETcommand(CMD::MON, CHANNEL::NONE, "BDFREL", "");
    std::string fwv = extractCommandValue(resp);
    firmwareVersion = Version(firmwareVersion);
    return fwv;
}

void DT1415ET::setFirmwareVersion() {
    std::string resp = sendDT1415ETcommand(CMD::MON, CHANNEL::NONE, "BDFREL", "");
    firmwareVersion = Version(extractCommandValue(resp));
}

std::string DT1415ET::getSerialNumber() {
    std::string resp = sendDT1415ETcommand(CMD::MON, CHANNEL::NONE, "BDSNUM", "");
    return extractCommandValue(resp);
}

std::string DT1415ET::getControlMode() {
    std::string resp = sendDT1415ETcommand(CMD::MON, CHANNEL::NONE, "BDCTR", "");
    return extractCommandValue(resp);
}

bool DT1415ET::isRemote() {
    std::string mode = getControlMode();
    if(!mode.compare("REMOTE"))
        return true;
    else
        return false;
}

std::string DT1415ET::getVoltageSet(CHANNEL channel) {
    std::string resp = sendDT1415ETcommand(CMD::MON, channel, "VSET", "");
    return extractCommandValue(resp);
}

std::string DT1415ET::getCurrentSet(CHANNEL channel) {
    std::string resp = sendDT1415ETcommand(CMD::MON, channel, "ISET", "");
    return extractCommandValue(resp);
}

std::string DT1415ET::getVoltageMax(CHANNEL channel) {
    std::string resp;
    if(firmwareVersion > "1.03")
        resp = sendDT1415ETcommand(CMD::MON, channel, "SWVMAX", "");
    else
        resp = sendDT1415ETcommand(CMD::MON, channel, "SVMAX", "");
    return extractCommandValue(resp);
}

std::string DT1415ET::getRampUp(CHANNEL channel) {
    std::string resp = sendDT1415ETcommand(CMD::MON, channel, "RUP", "");
    return extractCommandValue(resp);
}
std::string DT1415ET::getRampDown(CHANNEL channel) {
    std::string resp;
    if(firmwareVersion > "1.03")
        resp = sendDT1415ETcommand(CMD::MON, channel, "RDWN", "");
    else
        resp = sendDT1415ETcommand(CMD::MON, channel, "RDW", "");

    return extractCommandValue(resp);
}

std::string DT1415ET::getVoltage(CHANNEL channel) {
    std::string resp = sendDT1415ETcommand(CMD::MON, channel, "VMON", "");
    return extractCommandValue(resp);
}

std::string DT1415ET::getStatus(CHANNEL channel) {
    std::string resp;
    if(firmwareVersion > "1.03")
        resp = sendDT1415ETcommand(CMD::MON, channel, "STATUS", "");
    else
        resp = sendDT1415ETcommand(CMD::MON, channel, "STAT", "");
    return extractCommandValue(resp);
}

std::string DT1415ET::getCurrent(CHANNEL channel) {
    std::string resp = sendDT1415ETcommand(CMD::MON, channel, "IMON", "");
    return extractCommandValue(resp);
}

// set functions
void DT1415ET::setON(CHANNEL channel) {
    if(isRemote())
        sendDT1415ETcommand(CMD::SET, channel, "ON", "");
}

void DT1415ET::setOFF(CHANNEL channel) {
    if(isRemote())
        sendDT1415ETcommand(CMD::SET, channel, "OFF", "");
}

void DT1415ET::setVoltageSet(CHANNEL channel, double value) {
    if(isRemote()) {
        std::ostringstream sb;
        sb << std::setprecision(1) << std::fixed << std::setw(6) << value;
        sendDT1415ETcommand(CMD::SET, channel, "VSET", sb.str());
    }
}

void DT1415ET::setCurrentSet(CHANNEL channel, double value) {
    if(isRemote()) {
        std::ostringstream sb;
        sb << std::setprecision(1) << std::fixed << std::setw(6) << value;
        sendDT1415ETcommand(CMD::SET, channel, "ISET", sb.str());
    }
}

void DT1415ET::setVoltageMax(CHANNEL channel, double value) {
    if(isRemote()) {
        std::ostringstream sb;
        sb << std::setprecision(1) << std::fixed << std::setw(6) << value;
        if(firmwareVersion > "1.03")
            sendDT1415ETcommand(CMD::SET, channel, "SWVMAX", sb.str());
        else
            sendDT1415ETcommand(CMD::SET, channel, "SVMAX", sb.str());
    }
}

void DT1415ET::setRampUp(CHANNEL channel, double value) {
    if(isRemote()) {
        std::ostringstream sb;
        sb << std::setprecision(1) << std::fixed << std::setw(6) << value;
        sendDT1415ETcommand(CMD::SET, channel, "RUP", sb.str());
    }
}
void DT1415ET::setRampDown(CHANNEL channel, double value) {
    if(isRemote()) {
        std::ostringstream sb;
        sb << std::setprecision(1) << std::fixed << std::setw(6) << value;
        if(firmwareVersion > "1.03")
            sendDT1415ETcommand(CMD::SET, channel, "RDWN", sb.str());
        else
            sendDT1415ETcommand(CMD::SET, channel, "RDW", sb.str());
    }
}

void DT1415ET::clearAlarmSignal() {
    if(isRemote())
        sendDT1415ETcommand(CMD::SET, CHANNEL::NONE, "BDCLR", "");
}