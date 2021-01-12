#ifndef DCS_BASE_DEVICE_H
#define DCS_BASE_DEVICE_H

#include "ConnectionStream.h"
#include <chrono>
#include <memory>
#include <regex>
#include <thread>
class DCSBaseDevice {
  public:
    DCSBaseDevice() = delete;
    virtual ~DCSBaseDevice();

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
    DCSBaseDevice(ConnectionType allowed, ConnectionType active)
        : connectionTypeAllowed(allowed), activeConnectionType(active) {}
    ConnectionType connectionTypeAllowed;
    ConnectionType activeConnectionType;
    ConnectionStreamPtr connectionStream;
    bool connected = false;
};

#endif  // DCS_BASE_DEVICE_H