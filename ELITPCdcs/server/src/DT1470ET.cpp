#include "DT1470ET.h"
#include <sstream>
#include <string>

#include <iomanip>

DT1470ET::DT1470ET() : GenericDevice(ConnectionType::TCP, ConnectionType::TCP) {}

DT1470ET::~DT1470ET() {}

void DT1470ET::setConnectionStream(ConnectionStream *stream) {
    GenericDevice::setConnectionStream(stream);
    if(connected) {
        setFirmwareVersion();
    }
}

std::string DT1470ET::sendDT1470ETcommand(CMD command, CHANNEL channel,
                                          std::string function, std::string value) {
    std::stringstream cb;
    // CMD value SET OR mon
    cb << "$BD:" << bd;
    cb << ",CMD:" << CommandsMap.at(command);

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

std::string DT1470ET::extractCommandStatus(std::string command) { return "OK"; }

std::string DT1470ET::extractCommandValue(std::string command) {
    // TODO: extract command status, if error throw exception, if no, extract
    // value
    command.erase(command.length() - 1);
    std::size_t pos = command.find("VAL:");
    std::string ret = command;
    if(pos != std::string::npos)
        ret = command.substr(pos + 4);

    return ret;
}

std::string DT1470ET::getModuleName() {
    std::string resp = sendDT1470ETcommand(CMD::MON, CHANNEL::NONE, "BDNAME", "");
    return extractCommandValue(resp);
}

std::string DT1470ET::getFirmwareVersion() {
    std::string resp = sendDT1470ETcommand(CMD::MON, CHANNEL::NONE, "BDFREL", "");
    std::string fwv = extractCommandValue(resp);
    firmwareVersion = Version(firmwareVersion);
    return fwv;
}

void DT1470ET::setFirmwareVersion() {
    std::string resp = sendDT1470ETcommand(CMD::MON, CHANNEL::NONE, "BDFREL", "");
    firmwareVersion = Version(extractCommandValue(resp));
}

std::string DT1470ET::getSerialNumber() {
    std::string resp = sendDT1470ETcommand(CMD::MON, CHANNEL::NONE, "BDSNUM", "");
    return extractCommandValue(resp);
}

std::string DT1470ET::getControlMode() {
    std::string resp = sendDT1470ETcommand(CMD::MON, CHANNEL::NONE, "BDCTR", "");
    return extractCommandValue(resp);
}

bool DT1470ET::isRemote() {
    std::string mode = getControlMode();
    if(!mode.compare("REMOTE"))
        return true;
    else
        return false;
}

std::string DT1470ET::getIdentifier() {
    std::stringstream cb;
    cb << "Module name: " << getModuleName() << " | ";
    cb << "Firmware version: " << getFirmwareVersion() << " | ";
    cb << "S/N: " << getSerialNumber();

    return cb.str();
}

std::string DT1470ET::getVoltageSet(CHANNEL channel) {
    std::string resp = sendDT1470ETcommand(CMD::MON, channel, "VSET", "");
    return extractCommandValue(resp);
}

std::string DT1470ET::getCurrentSet(CHANNEL channel) {
    std::string resp = sendDT1470ETcommand(CMD::MON, channel, "ISET", "");
    return extractCommandValue(resp);
}

std::string DT1470ET::getVoltageMax(CHANNEL channel) {
    std::string resp = sendDT1470ETcommand(CMD::MON, channel, "MAXV", "");
    return extractCommandValue(resp);
}

std::string DT1470ET::getRampUp(CHANNEL channel) {
    std::string resp = sendDT1470ETcommand(CMD::MON, channel, "RUP", "");
    return extractCommandValue(resp);
}
std::string DT1470ET::getRampDown(CHANNEL channel) {
    std::string resp = sendDT1470ETcommand(CMD::MON, channel, "RDW", "");
    return extractCommandValue(resp);
}

std::string DT1470ET::getVoltage(CHANNEL channel) {
    std::string resp = sendDT1470ETcommand(CMD::MON, channel, "VMON", "");
    return extractCommandValue(resp);
}

std::string DT1470ET::getStatus(CHANNEL channel) {
    std::string resp = sendDT1470ETcommand(CMD::MON, channel, "STAT", "");
    return extractCommandValue(resp);
}

std::string DT1470ET::getCurrent(CHANNEL channel) {
    std::string resp = sendDT1470ETcommand(CMD::MON, channel, "IMON", "");
    return extractCommandValue(resp);
}

// set functions
void DT1470ET::setON(CHANNEL channel) {
    if(isRemote())
        sendDT1470ETcommand(CMD::SET, channel, "ON", "");
}

std::string DT1470ET::getPolarity(CHANNEL channel) {
    std::string resp = sendDT1470ETcommand(CMD::MON, channel, "POL", "");
    return extractCommandValue(resp);
}

void DT1470ET::setOFF(CHANNEL channel) {
    if(isRemote())
        sendDT1470ETcommand(CMD::SET, channel, "OFF", "");
}

void DT1470ET::setVoltageSet(CHANNEL channel, double value) {
    if(isRemote()) {
        std::ostringstream sb;
        sb << std::setprecision(1) << std::fixed << std::setw(6) << value;
        sendDT1470ETcommand(CMD::SET, channel, "VSET", sb.str());
    }
}

void DT1470ET::setCurrentSet(CHANNEL channel, double value) {
    if(isRemote()) {
        std::ostringstream sb;
        sb << std::setprecision(1) << std::fixed << std::setw(6) << value;
        sendDT1470ETcommand(CMD::SET, channel, "ISET", sb.str());
    }
}

void DT1470ET::setVoltageMax(CHANNEL channel, double value) {
    if(isRemote()) {
        std::ostringstream sb;
        sb << std::setprecision(1) << std::fixed << std::setw(6) << value;
        sendDT1470ETcommand(CMD::SET, channel, "MAXV", sb.str());
    }
}

void DT1470ET::setRampUp(CHANNEL channel, double value) {
    if(isRemote()) {
        std::ostringstream sb;
        sb << std::setprecision(1) << std::fixed << std::setw(6) << value;
        sendDT1470ETcommand(CMD::SET, channel, "RUP", sb.str());
    }
}
void DT1470ET::setRampDown(CHANNEL channel, double value) {
    if(isRemote()) {
        std::ostringstream sb;
        sb << std::setprecision(1) << std::fixed << std::setw(6) << value;
        sendDT1470ETcommand(CMD::SET, channel, "RDW", sb.str());
    }
}

void DT1470ET::clearAlarmSignal() {
    if(isRemote())
        sendDT1470ETcommand(CMD::SET, CHANNEL::NONE, "BDCLR", "");
}

void DT1470ET::setInterlockMode(INTERLOCK mode) {
    if(isRemote())
        sendDT1470ETcommand(CMD::SET, CHANNEL::NONE, "BDILKM",
                            mode == INTERLOCK::OPEN ? "OPEN" : "CLOSED");
}

std::string DT1470ET::getInterlockStatus() {
    std::string resp = sendDT1470ETcommand(CMD::MON, CHANNEL::NONE, "BDILK", "");
    return extractCommandValue(resp);
}

std::string DT1470ET::getInterlockMode() {
    std::string resp = sendDT1470ETcommand(CMD::MON, CHANNEL::NONE, "BDILKM", "");
    return extractCommandValue(resp);
}

std::string DT1470ET::getAlarmSignal() {
    std::string resp = sendDT1470ETcommand(CMD::MON, CHANNEL::NONE, "BDALARM", "");
    return extractCommandValue(resp);
}

std::string DT1470ET::getBUSTerminationStatus() {
    std::string resp = sendDT1470ETcommand(CMD::MON, CHANNEL::NONE, "BDTERM", "");
    return extractCommandValue(resp);
}
