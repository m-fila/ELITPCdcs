#include "MKS910.h"

#include <sstream>
MKS910::MKS910() : MKS() {}

std::string MKS910::getStatus() {
    std::string resp = sendWithDelayedResponse("T?");
    if(resp == "O" || resp == "M" || resp == "Z")
        return resp;
    else {
        throw std::runtime_error("Encountered unknown MKS910 status code");
    }
}

std::string MKS910::getTemp() { return sendWithDelayedResponse("TEM?"); }

std::string MKS910::getPPirani() { return sendWithDelayedResponse("PR1?"); }
std::string MKS910::getPPiezo() { return sendWithDelayedResponse("PR2?"); }
std::string MKS910::getPCombined() { return sendWithDelayedResponse("PR3?"); }
std::string MKS910::getPCombinedLong() { return sendWithDelayedResponse("PR4?"); }
std::string MKS910::getPDiff() { return sendWithDelayedResponse("PR5?"); }

std::string MKS910::getFirmwareVersion() { return sendWithDelayedResponse("FV?"); }
std::string MKS910::getHardwareVersion() { return sendWithDelayedResponse("HV?"); }
std::string MKS910::getName() { return sendWithDelayedResponse("DT?"); }
std::string MKS910::getModel() { return sendWithDelayedResponse("MD?"); }
std::string MKS910::getSerialNumber() { return sendWithDelayedResponse("SN?"); }
std::string MKS910::getPartNumber() { return sendWithDelayedResponse("PN?"); }
std::string MKS910::getUptime() { return sendWithDelayedResponse("TIM?"); }

void MKS910::setUnits(MKS910codes::Units u) {
    sendWithDelayedResponse("U!" + MKS910codes::unitsToString.at(u));
}

std::string MKS910::getRelayStatus(MKS910::RelayNo i) {
    return sendWithDelayedResponse("SS" + std::to_string(static_cast<int>(i)) + "?");
}
std::string MKS910::getRelayDirection(MKS910::RelayNo i) {
    return sendWithDelayedResponse("SD" + std::to_string(static_cast<int>(i)) + "?");
}
std::string MKS910::getRelayEnabled(MKS910::RelayNo i) {
    return sendWithDelayedResponse("EN" + std::to_string(static_cast<int>(i)) + "?");
}
std::string MKS910::getRelaySetPoint(MKS910::RelayNo i) {
    return sendWithDelayedResponse("SP" + std::to_string(static_cast<int>(i)) + "?");
}
std::string MKS910::getRelayHysteresis(MKS910::RelayNo i) {
    return sendWithDelayedResponse("SH" + std::to_string(static_cast<int>(i)) + "?");
}
std::string MKS910::getRelaySafetyDelay() { return sendWithDelayedResponse("SPD?"); }

void MKS910::setRelayDirection(MKS910::RelayNo i, MKS910codes::RelayDirection dir) {
    sendWithDelayedResponse("SD" + std::to_string(static_cast<int>(i)) + "!" +
                            MKS910codes::relayDirectionToString.at(dir));
}

void MKS910::setRelayEnabled(MKS910::RelayNo i, bool isEnabled) {
    std::string en = isEnabled ? "ON" : "OFF";
    sendWithDelayedResponse("EN" + std::to_string(static_cast<int>(i)) + "!" + en);
}
void MKS910::setRelaySetpoint(MKS910::RelayNo i, double v) {
    sendWithDelayedResponse("SP" + std::to_string(static_cast<int>(i)) + "!" +
                            std::to_string(v));
}
void MKS910::setRelayHysteresis(MKS910::RelayNo i, double v) {
    sendWithDelayedResponse("SH" + std::to_string(static_cast<int>(i)) + "!" +
                            std::to_string(v));
}
void MKS910::setRelaySafetyDelay(MKS910codes::RelayDelay delay) {
    sendWithDelayedResponse("SPD!" + MKS910codes::relayDelayToString.at(delay));
}
