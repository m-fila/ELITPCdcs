#include "MKS946.h"
#include <sstream>

MKS946::MKS946() : MKS() {}

std::string MKS946::getPressure(CH ch) {
    std::string cmd = "PR" + std::to_string(static_cast<int>(ch)) + "?";
    return sendWithDelayedResponse(cmd);
}

std::string MKS946::getPressureAll() { return sendWithDelayedResponse("PRZ?"); }

std::string MKS946::getFlow(CH ch) {
    std::string cmd = "FR" + std::to_string(static_cast<int>(ch)) + "?";
    return sendWithDelayedResponse(cmd);
}

std::string MKS946::getFlowFactor(CH ch) {
    std::string cmd = "QSF" + std::to_string(static_cast<int>(ch)) + "?";
    return sendWithDelayedResponse(cmd);
}

std::string MKS946::getFlowNominalRange(CH ch) {
    std::string cmd = "RNG" + std::to_string(static_cast<int>(ch)) + "?";
    return sendWithDelayedResponse(cmd);
}

void MKS946::zeroMFC(CH ch) {
    std::string cmd = "QZ" + std::to_string(static_cast<int>(ch)) + "?";
    sendWithDelayedResponse(cmd);
}

std::string MKS946::getFlowSetPoint(CH ch) {
    std::string cmd = "QSP" + std::to_string(static_cast<int>(ch)) + "?";
    return sendWithDelayedResponse(cmd);
}

std::string MKS946::getFlowMode(CH ch) {
    std::string cmd = "QMD" + std::to_string(static_cast<int>(ch)) + "?";
    return sendWithDelayedResponse(cmd);
}

std::string MKS946::getActiveRatioRecipe() {
    std::string cmd = "RCP?";
    return sendWithDelayedResponse(cmd);
}

std::string MKS946::getActivePIDRecipe() {
    std::string cmd = "RRCP?";
    return sendWithDelayedResponse(cmd);
}

std::string MKS946::getModuleType() {
    std::string cmd = "MT?";
    return sendWithDelayedResponse(cmd);
}

std::string MKS946::getSensorType(MODULE m) {
    std::string n;
    switch(m) {
    case MODULE::A:
        n = "A";
        break;
    case MODULE::B:
        n = "B";
        break;
    case MODULE::C:
        n = "C";
        break;
    }
    std::string cmd = "ST" + n;
    return sendWithDelayedResponse(cmd);
}

std::string MKS946::getFirmware(CH ch) {
    return sendWithDelayedResponse("FV" + std::to_string(static_cast<int>(ch)) + "?");
}

std::string MKS946::getSerialNumber(CH ch) {
    return sendWithDelayedResponse("SN" + std::to_string(static_cast<int>(ch)) + "?");
}

std::string MKS946::getFirmwareVersion() { return sendWithDelayedResponse("FV6?"); }
std::string MKS946::getName() { return sendWithDelayedResponse("DT?"); }
std::string MKS946::getModel() { return sendWithDelayedResponse("MD?"); }
std::string MKS946::getSerialNumber() { return sendWithDelayedResponse("SN?"); }
std::string MKS946::getUptime() { return sendWithDelayedResponse("TIM?"); }

void MKS946::setUnits(MKS946codes::Units u) {
    sendWithDelayedResponse("U!" + MKS946codes::unitsToString.at(u));
}

std::string MKS946::getPIDControl() { return sendWithDelayedResponse("PID?"); }
std::string MKS946::getManualControl() { return sendWithDelayedResponse("RM?"); }

void MKS946::setPIDControl(bool isOn) {
    std::string status = isOn ? "ON" : "OFF";
    sendWithDelayedResponse("PID" + status);
}

void MKS946::setManualControl(bool isOn) {
    std::string status = isOn ? "ON" : "OFF";
    sendWithDelayedResponse("RM" + status);
}

std::string MKS946::getRelayStatus(MKS946::RelayNo i) {
    return sendWithDelayedResponse("SS" + std::to_string(static_cast<int>(i)) + "?");
}

std::string MKS946::getRelayDirection(MKS946::RelayNo i) {
    return sendWithDelayedResponse("SD" + std::to_string(static_cast<int>(i)) + "?");
}

std::string MKS946::getRelayEnabled(MKS946::RelayNo i) {
    return sendWithDelayedResponse("EN" + std::to_string(static_cast<int>(i)) + "?");
}

std::string MKS946::getRelaySetPoint(MKS946::RelayNo i) {
    return sendWithDelayedResponse("SP" + std::to_string(static_cast<int>(i)) + "?");
}

std::string MKS946::getRelayHysteresis(MKS946::RelayNo i) {
    return sendWithDelayedResponse("SH" + std::to_string(static_cast<int>(i)) + "?");
}

void MKS946::setRelayDirection(MKS946::RelayNo i, MKS946codes::RelayDirection dir) {
    sendWithDelayedResponse("SD" + std::to_string(static_cast<int>(i)) + "!" +
                            MKS946codes::relayDirectionToString.at(dir));
}

void MKS946::setRelayEnabled(MKS946::RelayNo i, MKS946codes::RelayEnabled enabled) {
    sendWithDelayedResponse("EN" + std::to_string(static_cast<int>(i)) + "!" +
                            MKS946codes::relayEnabledToString.at(enabled));
}

void MKS946::setRelaySetpoint(MKS946::RelayNo i, double v) {
    sendWithDelayedResponse("SP" + std::to_string(static_cast<int>(i)) + "!" +
                            std::to_string(v));
}

void MKS946::setRelayHysteresis(MKS946::RelayNo i, double v) {
    sendWithDelayedResponse("SH" + std::to_string(static_cast<int>(i)) + "!" +
                            std::to_string(v));
}
