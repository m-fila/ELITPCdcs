#ifndef DCS_GENERIC_DEVICE_H
#define DCS_GENERIC_DEVICE_H

#include "DCSBaseDevice.h"
class DCSGenericDevice : public DCSBaseDevice {
  public:
    DCSGenericDevice() = delete;
    virtual std::string getVendor() { return "-"; }
    virtual std::string getModel() { return "-"; }
    virtual std::string getPartNumber() { return "-"; }
    virtual std::string getSerialNumber() { return "-"; }
    virtual std::string getFirmwareVersion() { return "-"; }
    virtual std::string getHardwareVersion() { return "-"; }

  protected:
    DCSGenericDevice(ConnectionType allowed, ConnectionType active)
        : DCSBaseDevice(allowed, active) {}
};

#endif  // DCS_GENERIC_DEVICE_H
