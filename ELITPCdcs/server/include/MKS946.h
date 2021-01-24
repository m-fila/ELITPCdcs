#ifndef mks946_H
#define mks946_H

#include "MKS.h"
#include "mks946codes.h"

class MKS946 : public MKS {
  public:
    MKS946();
    // channel enum 1-6
    enum class CH { NONE = 0, CH1, CH2, CH3, CH4, CH5, CH6 };
    enum class PID { R1 = 1, R2, R3, R4, R5, R6, R7, R8 };
    enum class RATIO { R1 = 1, R2, R3, R4 };
    enum class MODULE { A = 1, B, C };
    enum class RelayNo { R1 = 1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12 };
    // device specific commands

    // pressure commands
    std::string getPressure(CH ch);
    std::string getPressureAll();

    // CM commands
    std::string getCMNominalRange(CH ch);
    std::string getCMType(CH ch);
    std::string getCMVoltageRange(CH ch);

    void setCMNominalRange(CH ch, double value);
    void setCMType(CH ch, MKS946codes::CMType type);
    void setCMVoltageRange(CH ch, MKS946codes::CMVoltageRange range);

    // MFC commands
    std::string getFlow(CH ch);
    std::string getFlowScaleFactor(CH ch);
    std::string getFlowNominalRange(CH ch);
    std::string getFlowSetPoint(CH ch);
    std::string getFlowMode(CH ch);

    void zeroMFC(CH ch);
    void setFlowScaleFactor(CH ch, double factor);
    void setFlowNominalRange(CH ch, double range);
    void setFlowSetPoint(CH ch, double value);
    void setFlowMode(CH ch, MKS946codes::FlowMode mode);

    // relay commands
    std::string getRelayStatus(RelayNo i);
    std::string getRelayDirection(RelayNo i);
    std::string getRelayEnabled(RelayNo i);
    std::string getRelaySetPoint(RelayNo i);
    std::string getRelayHysteresis(RelayNo i);

    void setRelayDirection(RelayNo i, MKS946codes::RelayDirection dir);
    void setRelayEnabled(RelayNo i, MKS946codes::RelayEnabled);
    void setRelaySetpoint(RelayNo i, double v);
    void setRelayHysteresis(RelayNo i, double v);

    // pid/ratio commands
    std::string getActiveRatioRecipe();

    std::string getPIDControl();
    std::string getManualControl();

    std::string getActivePIDRecipe();

    std::string getPIDMFCChannel() { return queryPID("RDCH"); }
    std::string getPIDPressureChannel() { return queryPID("RPCH"); }
    std::string getPIDPressureSetPoint() { return queryPID("RPSP"); }
    std::string getPIDKp() { return queryPID("RKP"); }
    std::string getPIDTimeConstant() { return queryPID("RTI"); }
    std::string getPIDDerivativeTimeConstant() { return queryPID("RTD"); }
    std::string getPIDCeiling() { return queryPID("RCEI"); }
    std::string getPIDBase() { return queryPID("RBAS"); }
    std::string getPIDPreset() { return queryPID("RPST"); }
    std::string getPIDStart() { return queryPID("RSTR"); }
    std::string getPIDEnd() { return queryPID("REND"); }
    std::string getPIDCtrlStart() { return queryPID("RCST"); }
    std::string getPIDDirection() { return queryPID("RDIR"); }
    std::string getPIDBand() { return queryPID("RGSB"); }
    std::string getPIDGain() { return queryPID("RGSG"); }

    void setPIDMFCChannel(MKS946codes::PIDFlowChannel ch);
    void setPIDPressureChannel(MKS946codes::PIDPressureChannel ch);
    void setPIDPressureSetPoint(double value);
    void setPIDKp(double value);
    void setPIDTimeConstant(double value);
    void setPIDDerivativeTimeConstant(double value);
    void setPIDCeiling(double value);
    void setPIDBase(double value);
    void setPIDPreset(double value);
    void setPIDStart(double value);
    void setPIDEnd(double value);
    void setPIDCtrlStart(double value);
    void setPIDDirection(MKS946codes::PIDDirection dir);
    void setPIDBand(uint value);
    void setPIDGain(uint value);

    void setActivePIDRecipe(PID recipe);
    void setActiveRatioRecipe(RATIO recipe);
    void setPIDControl(bool isOn);
    void setManualControl(bool isOn);

    // modules commands
    std::string getModuleType();
    std::string getSensorType(MODULE m);
    std::string getFirmware(CH ch);
    std::string getSerialNumber(CH ch);

    // unit commands
    std::string getVendor() override { return "MKS"; }
    std::string getFirmwareVersion() override;
    std::string getName();
    std::string getModel() override;
    std::string getSerialNumber() override;
    std::string getUptime();

    std::string getUnits() { return sendWithDelayedResponse("U?"); }
    void setUnits(MKS946codes::Units u);

  private:
    std::string query(std::string command, CH ch);
    std::string queryPID(std::string command);
    void sendCommand(std::string command, CH ch, std::string value);
    void sendPIDCommand(std::string command, double value) {
        return sendCommand(command, CH::NONE, format(value));
    }
    static std::string format(double v);
};

#endif  // mks946_H
