#include "HMP2020.h"
#include <sstream>

HMP2020::HMP2020()
    : DCSGenericDevice(ConnectionType::TCP | ConnectionType::Serial,
                       ConnectionType::TCP) {}

HMP2020::~HMP2020() {}

void HMP2020::setActiveChannel(int channel) {
    std::stringstream cb;
    cb << "INST OUT" << channel << "\n";
    sendCommand(cb.str());
}

std::string HMP2020::getVoltageSet() { return sendWithResponse("VOLT?\n"); }

std::string HMP2020::getCurrentSet() { return sendWithResponse("CURR?\n"); }

std::string HMP2020::getOutputSel() { return sendWithResponse("OUTP:SEL?\n"); }

std::string HMP2020::getOutputGen() { return sendWithResponse("OUTP:GEN?\n"); }

std::string HMP2020::getVoltage() { return sendWithResponse("MEAS:VOLT?\n"); }

std::string HMP2020::getCurrent() { return sendWithResponse("MEAS:CURR?\n"); }

void HMP2020::setOutputSel(int channel, bool state) {
    setActiveChannel(channel);
    std::stringstream cb;
    cb << "OUTP:SEL " << state << "\n";
    sendCommand(cb.str());
}

void HMP2020::setOutputGen(bool state) {
    std::stringstream cb;
    cb << "OUTP:GEN " << state << "\n";
    sendCommand(cb.str());
}

void HMP2020::setVoltage(int channel, double v) {
    setActiveChannel(channel);
    std::stringstream cb;
    cb << "SOUR:VOLT " << v << "\n";
    sendCommand(cb.str());
}

void HMP2020::setCurrent(int channel, double i) {
    setActiveChannel(channel);
    std::stringstream cb;
    cb << "SOUR:CURR " << i << "\n";
    sendCommand(cb.str());
}

std::string HMP2020::parseIdentifier(size_t n) {
    std::stringstream ss(sendWithResponse("*IDN?\n"));
    std::string segment;
    for(size_t i = 0; i <= n; ++i) {
        std::getline(ss, segment, ',');
    }
    return segment;
}

std::string HMP2020::getVendor() { return parseIdentifier(0); }
std::string HMP2020::getModel() { return parseIdentifier(1); }
std::string HMP2020::getSerialNumber() {
    auto r = parseIdentifier(2);
    auto slash = r.find("/");
    if(slash == std::string::npos) {
        return r;
    }
    return r.substr(slash + 1, r.length());
}

std::string HMP2020::getPartNumber() {
    auto r = parseIdentifier(2);
    auto slash = r.find("/");
    if(slash == std::string::npos) {
        return r;
    }
    return r.substr(0, slash);
}
std::string HMP2020::getFirmwareVersion() { return parseIdentifier(3); }