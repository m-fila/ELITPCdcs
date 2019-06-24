#include "../../include/hw/HMP2020.h"
#include <sstream>

HMP2020::HMP2020() : GenericDevice(ConnectionType::TCP | ConnectionType::Serial, ConnectionType::TCP)
{

}

HMP2020::~HMP2020()
{

}

string HMP2020::getIdentifier()
{
    return sendWithResponse("*IDN?\n");
}

void HMP2020::setActiveChannel(int channel)
{
    std::stringstream cb;
    cb << "INST OUT" << channel << "\n";
    sendCommand(cb.str());
}

string HMP2020::getVoltageSet()
{
    return sendWithResponse("VOLT?\n");
}

string HMP2020::getCurrentSet()
{
    return sendWithResponse("CURR?\n");
}

string HMP2020::getOutputSel()
{
    return sendWithResponse("OUTP:SEL?\n");
}

string HMP2020::getOutputGen()
{
    return sendWithResponse("OUTP:GEN?\n");
}

string HMP2020::getVoltage()
{
    return sendWithResponse("MEAS:VOLT?\n");
}

string HMP2020::getCurrent()
{
    return sendWithResponse("MEAS:CURR?\n");
}

void HMP2020::setOutputSel(int channel, bool state)
{
    setActiveChannel(channel);
    std::stringstream cb;
    cb << "OUTP:SEL " << state << "\n";
    sendCommand(cb.str());
}

void HMP2020::setOutputGen(bool state)
{
    std::stringstream cb;
    cb << "OUTP:GEN " << state << "\n";
    sendCommand(cb.str());
}
