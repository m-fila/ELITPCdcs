#ifndef DT1415ET_H
#define DT1415ET_H

#include "DCSGenericDevice.h"
#include "version.h"
#include <map>
#include <string>

// enum class DT1415ETchannelStatus;

class DT1415ET : public DCSGenericDevice {
  public:
    enum class CMD { MON, SET };
    enum class CHANNEL { CH0, CH1, CH2, CH3, CH4, CH5, CH6, CH7, ALL, NONE };
    enum class STATUS { CMDOK, CMDERR, CHERR, PARERR, VALERR, LOCERR, UNKNOWN };

  public:
    DT1415ET();
    virtual ~DT1415ET();
    std::string sendDT1415ETcommand(CMD command, CHANNEL channel, std::string function,
                                    std::string value);
    void setConnectionStream(ConnectionStream *stream) override;
    std::string getModuleName();
    std::string getFirmwareVersion();
    std::string getSerialNumber();
    std::string getControlMode();
    bool isRemote();
    std::string getIdentifier();
    std::string getVoltageSet(CHANNEL channel);
    std::string getVoltage(CHANNEL channel);
    std::string getVoltageMax(CHANNEL channel);
    std::string getRampUp(CHANNEL channel);
    std::string getRampDown(CHANNEL channel);
    std::string getStatus(CHANNEL channel);
    std::string getCurrent(CHANNEL channel);
    std::string getCurrentSet(CHANNEL channel);

    void setON(CHANNEL channel);
    void setOFF(CHANNEL channel);
    void setVoltageSet(CHANNEL channel, double value);
    void setCurrentSet(CHANNEL channel, double value);
    void setVoltageMax(CHANNEL channel, double value);
    void setRampUp(CHANNEL channel, double value);
    void setRampDown(CHANNEL, double value);

  private:
    const std::map<CMD, std::string> CommandsMap = {{CMD::MON, "MON"}, {CMD::SET, "SET"}};
    const std::map<STATUS, std::string> StatusMap = {
        {STATUS::CMDOK, "#CMD:OK"},   {STATUS::CMDERR, "#CMD:ERR"},
        {STATUS::CHERR, "#CH:ERR"},   {STATUS::PARERR, "#PAR:ERR"},
        {STATUS::VALERR, "#VAL:ERR"}, {STATUS::LOCERR, "#LOC:ERR"},
        {STATUS::UNKNOWN, "UNKNOWN"}};

    std::string extractCommandStatus(std::string command);
    std::string extractCommandValue(std::string command);
    void setFirmwareVersion();
    Version firmwareVersion;
};

#endif  // DT1415ET_H
