#ifndef GENERICDEVICE_H
#define GENERICDEVICE_H

#include "ConnectionStream.h"
#include <chrono>
#include <memory>
#include <regex>
#include <thread>
class GenericDevice {
  public:
    GenericDevice();
    virtual ~GenericDevice();

    virtual std::string getVendor() { return "-"; }
    virtual std::string getModel() { return "-"; }
    virtual std::string getSerialNumber() { return "-"; }
    virtual std::string getFirmwareVersion() { return "-"; }

    virtual void setConnectionStream(ConnectionStream *stream);
    void resetConnectionStream();
    bool isConnected() { return connected; }
    void sendCommand(std::string command);
    std::string receiveResponse();
    std::string sendWithResponse(std::string command);
    std::string sendWithDelayedResponse(std::string command, size_t delay_ms);
    ConnectionType getActiveConnectionType() { return activeConnectionType; }

  protected:
    void validate(const std::string &response, const std::string &regex);
    GenericDevice(ConnectionType allowed, ConnectionType active)
        : connectionTypeAllowed(allowed), activeConnectionType(active) {}
    ConnectionType connectionTypeAllowed;
    ConnectionType activeConnectionType;
    ConnectionStreamPtr connectionStream;
    bool connected = false;
};

#endif  // GENERICDEVICE_H
