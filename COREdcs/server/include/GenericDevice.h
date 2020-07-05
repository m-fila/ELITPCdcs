#ifndef GENERICDEVICE_H
#define GENERICDEVICE_H

#include "ConnectionStream.h"
#include <chrono>
#include <memory>
#include <thread>
class GenericDevice {
public:
  GenericDevice();
  virtual ~GenericDevice();

  virtual void setConnectionStream(ConnectionStream *stream);
  void resetConnectionStream();
  bool isConnected() { return connected; }
  void sendCommand(std::string command);
  std::string receiveResponse();
  std::string sendWithResponse(std::string command);
  std::string sendWithDelayedResponse(std::string command, uint delay_ms);
  ConnectionType getActiveConnectionType() { return activeConnectionType; }

protected:
  GenericDevice(ConnectionType allowed, ConnectionType active)
      : connectionTypeAllowed(allowed), activeConnectionType(active) {}
  ConnectionType connectionTypeAllowed;
  ConnectionType activeConnectionType;
  ConnectionStreamPtr connectionStream;
  bool connected = false;
};

#endif // GENERICDEVICE_H
