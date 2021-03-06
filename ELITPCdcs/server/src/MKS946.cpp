#include "MKS946.h"
#include <sstream>
MKS946::MKS946() : MKS() {}
std::string MKS946::query(std::string command, CH ch) {
    std::stringstream cmd;
    cmd << command;
    if(ch != CH::NONE) {
        cmd << static_cast<int>(ch);
    }
    cmd << "?";
    return sendWithDelayedResponse(cmd.str());
}
void MKS946::sendCommand(std::string command, CH ch, std::string value) {
    std::stringstream cmd;
    cmd << command;
    if(ch != CH::NONE) {
        cmd << static_cast<int>(ch);
    }
    cmd << "!";
    if(!value.empty()) {
        cmd << value;
    }
    sendWithDelayedResponse(cmd.str());
}

std::string MKS946::queryPID(std::string command) {
    auto response = query(command, CH::NONE);
    auto found = response.find(':');
    if(found == std::string::npos) {
        throw std::runtime_error("received incorrect response to PID query '" + command +
                                 "'");
    }
    return response.substr(++found);
}

std::string MKS946::getPressure(CH ch) { return query("PR", ch); }

std::string MKS946::getPressureAll() { return query("PRZ", CH::NONE); }

std::string MKS946::getFlow(CH ch) { return query("FR", ch); }

std::string MKS946::getFlowScaleFactor(CH ch) { return query("QSF", ch); }

std::string MKS946::getFlowNominalRange(CH ch) { return query("RNG", ch); }

void MKS946::setFlowScaleFactor(CH ch, double factor) {
    sendCommand("QSF", ch, std::to_string(factor));
}

void MKS946::setFlowNominalRange(CH ch, double value) {
    sendCommand("RNG", ch, std::to_string(value));
}

void MKS946::zeroMFC(CH ch) { sendCommand("QZ", ch, ""); }

std::string MKS946::getFlowSetPoint(CH ch) { return query("QSP", ch); }

std::string MKS946::getFlowMode(CH ch) { return query("QMD", ch); }

void MKS946::setFlowSetPoint(CH ch, double value) {
    sendCommand("QSP", ch, std::to_string(value));
}

void MKS946::setFlowMode(CH ch, MKS946codes::FlowMode mode) {
    sendCommand("QMD", ch, MKS946codes::flowModeToString.at(mode));
}

std::string MKS946::getActiveRatioRecipe() { return query("RRCP", CH::NONE); }

std::string MKS946::getActivePIDRecipe() { return query("RCP", CH::NONE); }

std::string MKS946::getModuleType() { return query("MT", CH::NONE); }

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
    return query("ST" + n, CH::NONE);
}

std::string MKS946::getFirmware(CH ch) { return query("FV", ch); }

std::string MKS946::getSerialNumber(CH ch) { return query("SN", ch); }

std::string MKS946::getFirmwareVersion() { return query("FV6", CH::NONE); }
std::string MKS946::getName() { return query("DT", CH::NONE); }
std::string MKS946::getModel() { return query("MD", CH::NONE); }
std::string MKS946::getSerialNumber() { return query("SN", CH::NONE); }
std::string MKS946::getUptime() { return query("TIM", CH::NONE); }

void MKS946::setUnits(MKS946codes::Units u) {
    sendCommand("U", CH::NONE, MKS946codes::unitsToString.at(u));
}

std::string MKS946::getPIDControl() { return query("PID", CH::NONE); }
std::string MKS946::getManualControl() { return query("RM", CH::NONE); }

void MKS946::setPIDControl(bool isOn) {
    std::string status = isOn ? "ON" : "OFF";
    sendCommand("PID", CH::NONE, status);
}

void MKS946::setManualControl(bool isOn) {
    std::string status = isOn ? "ON" : "OFF";
    sendCommand("RM", CH::NONE, status);
}

std::string MKS946::getRelayStatus(MKS946::RelayNo i) {
    return query("SS" + std::to_string(static_cast<int>(i)), CH::NONE);
}

std::string MKS946::getRelayDirection(MKS946::RelayNo i) {
    return query("SD" + std::to_string(static_cast<int>(i)), CH::NONE);
}

std::string MKS946::getRelayEnabled(MKS946::RelayNo i) {
    return query("EN" + std::to_string(static_cast<int>(i)), CH::NONE);
}

std::string MKS946::getRelaySetPoint(MKS946::RelayNo i) {
    return query("SP" + std::to_string(static_cast<int>(i)), CH::NONE);
}

std::string MKS946::getRelayHysteresis(MKS946::RelayNo i) {
    return query("SH" + std::to_string(static_cast<int>(i)), CH::NONE);
}

void MKS946::setRelayDirection(MKS946::RelayNo i, MKS946codes::RelayDirection dir) {
    sendCommand("SD" + std::to_string(static_cast<int>(i)), CH::NONE,
                MKS946codes::relayDirectionToString.at(dir));
}

void MKS946::setRelayEnabled(MKS946::RelayNo i, MKS946codes::RelayEnabled enabled) {
    sendCommand("EN" + std::to_string(static_cast<int>(i)), CH::NONE,
                MKS946codes::relayEnabledToString.at(enabled));
}

void MKS946::setRelaySetpoint(MKS946::RelayNo i, double v) {
    sendCommand("SP" + std::to_string(static_cast<int>(i)), CH::NONE, std::to_string(v));
}

void MKS946::setRelayHysteresis(MKS946::RelayNo i, double v) {
    sendCommand("SH" + std::to_string(static_cast<int>(i)), CH::NONE, std::to_string(v));
}

std::string MKS946::getCMNominalRange(CH ch) { return query("RNG", ch); }

void MKS946::setCMNominalRange(CH ch, double value) {
    sendCommand("RNG", ch, std::to_string(value));
}

std::string MKS946::getCMType(CH ch) { return query("CMT", ch); }

void MKS946::setCMType(CH ch, MKS946codes::CMType type) {
    sendCommand("CMT", ch, MKS946codes::CMTypeToString.at(type));
}

std::string MKS946::getCMVoltageRange(CH ch) { return query("BVR", ch); }

void MKS946::setCMVoltageRange(CH ch, MKS946codes::CMVoltageRange range) {
    sendCommand("BVR", ch, MKS946codes::CMVoltageRangeToString.at(range));
}

void MKS946::setPIDMFCChannel(MKS946codes::PIDFlowChannel ch) {
    return sendCommand("RDCH", CH::NONE, MKS946codes::PIDFlowChannelToString.at(ch));
}
void MKS946::setPIDPressureChannel(MKS946codes::PIDPressureChannel ch) {
    return sendCommand("RPCH", CH::NONE, MKS946codes::PIDPressureChannelToString.at(ch));
}
void MKS946::setPIDPressureSetPoint(double value) {
    return sendCommand("RPSP", CH::NONE, format(value));
}
void MKS946::setPIDKp(double value) { return sendPIDCommand("RKP", value); }
void MKS946::setPIDTimeConstant(double value) { return sendPIDCommand("RTI", value); }
void MKS946::setPIDDerivativeTimeConstant(double value) {
    return sendPIDCommand("RTD", value);
}
void MKS946::setPIDCeiling(double value) { return sendPIDCommand("RCEI", value); }
void MKS946::setPIDBase(double value) { return sendPIDCommand("RBAS", value); }
void MKS946::setPIDPreset(double value) { return sendPIDCommand("RPST", value); }
void MKS946::setPIDStart(double value) { return sendPIDCommand("RSTR", value); }
void MKS946::setPIDEnd(double value) { return sendPIDCommand("REND", value); }
void MKS946::setPIDCtrlStart(double value) { return sendPIDCommand("RCST", value); }
void MKS946::setPIDDirection(MKS946codes::PIDDirection dir) {
    return sendCommand("RDIR", CH::NONE, MKS946codes::PIDDirectionToString.at(dir));
}
void MKS946::setPIDBand(uint value) {
    if(value > 30) {
        throw std::runtime_error("Incorrect PID band value");
    }
    return sendCommand("RGSB", CH::NONE, std::to_string(value));
}
void MKS946::setPIDGain(uint value) {
    if(value == 0 || value > 200) {
        throw std::runtime_error("Incorrect PID gain value");
    }
    return sendCommand("RGSG", CH::NONE, std::to_string(value));
}

std::string MKS946::format(double v) {
    std::stringstream ss;
    ss.precision(2);
    ss << std::scientific << v;
    auto str = ss.str();
    str.replace(4, 1, "E");
    return str;
}

std::string MKS946::getUserCalibrationEnabled() { return query("CAL", CH::NONE); }
void MKS946::setUserCalibrationEnabled(bool enabled) {
    return sendCommand("CAL", CH::NONE, enabled ? "Enable" : "Disable");
}