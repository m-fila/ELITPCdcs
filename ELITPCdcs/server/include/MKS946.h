#ifndef mks946_H
#define mks946_H

#include "MKS.h"
#include "mks946codes.h"

class MKS946 : public MKS {
  public:
    MKS946();
    // channel enum 1-6
    enum class CH { CH1 = 1, CH2, CH3, CH4, CH5, CH6 };
    enum class PID { R1 = 1, R2, R3, R4, R5, R6, R7, R8 };
    enum class RATIO { R1 = 1, R2, R3, R4 };
    enum class MODULE { A = 1, B, C };
    enum class RelayNo { R1 = 1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12 };
    // device specific commands

    // pressure commands
    std::string getPressure(CH ch);
    std::string getPressureAll();

    // flow commands
    std::string getFlow(CH ch);
    std::string getFlowFactor(CH ch);
    std::string getFlowNominalRange(CH ch);
    std::string getFlowSetPoint(CH ch);
    std::string getFlowMode(CH ch);

    void zeroMFC(CH ch);
    void setFlowFactor(CH ch, double factor);
    void setFlowNomialRange(CH ch, double range);
    void setFlowPoint(CH ch, double value);
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
    std::string getActivePIDRecipe();
    std::string getActiveRatioRecipe();
    std::string getPIDControl();
    std::string getManualControl();

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

    // TORR, PASCAL, MBAR
    void setUnits(MKS946codes::Units u);
};

#endif  // mks946_H
