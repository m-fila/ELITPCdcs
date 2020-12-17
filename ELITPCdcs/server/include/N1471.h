#ifndef N1471_H
#define N1471_H

#include "DCSGenericDevice.h"
#include <map>
#include <string>

class N1471 : public DCSGenericDevice {
  public:
    enum class CMD { MON, SET };
    enum class CHANNEL { CH0, CH1, CH2, CH3, ALL, NONE };
    enum class STATUS { CMDOK, CMDERR, CHERR, PARERR, VALERR, LOCERR, UNKNOWN };

  public:
    N1471();
    std::string sendN1471command(CMD command, CHANNEL channel, std::string function,
                                 std::string value);

    std::string getVendor() override { return "Caen"; }
    std::string getModel() override;
    std::string getFirmwareVersion() override;
    std::string getSerialNumber() override;

    std::string getControlMode();

    bool isRemote();
    std::string getVoltageSet(CHANNEL channel);
    std::string getVoltage(CHANNEL channel);
    std::string getVoltageMax(CHANNEL channel);
    std::string getRampUp(CHANNEL channel);
    std::string getRampDown(CHANNEL channel);
    std::string getStatus(CHANNEL channel);
    std::string getCurrent(CHANNEL channel);
    std::string getCurrentSet(CHANNEL channel);
    std::string getPolarity(CHANNEL channel);

    void setON(CHANNEL channel);
    void setOFF(CHANNEL channel);
    void setVoltageSet(CHANNEL channel, double value);
    void setCurrentSet(CHANNEL channel, double value);
    void setVoltageMax(CHANNEL channel, double value);
    void setRampUp(CHANNEL channel, double value);
    void setRampDown(CHANNEL, double value);
    void setModuleAddress(int nr);

    void clearAlarmSignal();

  private:
    const std::map<CMD, std::string> CommandsMap = {{CMD::MON, "MON"}, {CMD::SET, "SET"}};
    const std::map<STATUS, std::string> StatusMap = {
        {STATUS::CMDOK, "#CMD:OK"},   {STATUS::CMDERR, "#CMD:ERR"},
        {STATUS::CHERR, "#CH:ERR"},   {STATUS::PARERR, "#PAR:ERR"},
        {STATUS::VALERR, "#VAL:ERR"}, {STATUS::LOCERR, "#LOC:ERR"},
        {STATUS::UNKNOWN, "UNKNOWN"}};

    std::string extractCommandStatus(std::string command);
    std::string extractCommandValue(std::string command);
    int moduleAddress;
};

#endif  // N1471_H
