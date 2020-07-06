#ifndef DCS_DEVICE_CONTROLLER_H
#define DCS_DEVICE_CONTROLLER_H
#include "DCSLogger.h"
#include "DCSObject.h"
#include "DCSUAJson.h"
#include "DCSWorkerThread.h"
#include "TCPConnector.h"
#include <iostream>

template <class Device> class DCSDeviceController : public DCSObject {

  friend DCSServer;

public:
  template <class T>
  void addControllerMethod(std::string methodName,
                           std::string methodDescription,
                           std::vector<DCSObject::MethodArgs> inputArgs,
                           std::vector<DCSObject::MethodArgs> outputArgs,
                           const T &methodBody, bool threaded = true) {
    // wrap with exception
    auto newBody = [methodBody, methodName, this](const UA_Variant *input,
                                                  UA_Variant *output) {
      try {
        methodBody(input, output);
      } catch (const exception &e) {
        UA_LOG_ERROR(
            DCSLogger::getLogger(), UA_LOGCATEGORY_USERLAND,
            "Method %s encountered error: %s . Disconnecting device...",
            methodName.c_str(), e.what());
        fastEvent(this->getName(), 400,
                  "Device communication error occured. Disconnecting device.");
        this->device.resetConnectionStream();
      }
    };

    if (threaded) {
      DCSObject::addMethod(methodName, methodDescription, inputArgs, outputArgs,
                           newBody, &this->deviceThread);
    } else {
      DCSObject::addMethod(methodName, methodDescription, inputArgs, outputArgs,
                           newBody);
    }
  }

  template <class T, class B>
  void
  addControllerMethod(std::string methodName, std::string methodDescription,
                      std::vector<DCSObject::MethodArgs> inputArgs,
                      std::vector<DCSObject::MethodArgs> outputArgs,
                      const T &methodBody, B instance, bool threaded = true) {

    std::function<void(const UA_Variant *, UA_Variant *)> newBody = std::bind(
        methodBody, instance, std::placeholders::_1, std::placeholders::_2);
    addControllerMethod(methodName, methodDescription, inputArgs, outputArgs,
                        newBody, threaded);
  }

  template <class T>
  void addVariableUpdate(DCSVariable &variable, uint interval_ms,
                         T updateMethod, bool threaded = true,
                         bool deviceProtected = true) {
    std::function<void()> callback;
    if (deviceProtected) {
      callback = [this, &variable, updateMethod]() {
        if (device.isConnected()) {
          try {
            variable.setValue(updateMethod());
          } catch (const std::exception &e) {
            UA_LOG_ERROR(DCSLogger::getLogger(), UA_LOGCATEGORY_USERLAND,
                         "Variable %s update encountered error: %s . "
                         "Disconnecting device...",
                         variable.getFullName().c_str(), e.what());
            fastEvent(this->getName(), 40,
                      "Device communication error occured. Disconnecting "
                      "device.");
            this->device.resetConnectionStream();
          }
        } else {
          //  variable.setNull();
          void *fallback;
          UA_init(&fallback, variable.getDataType());
          variable.setValueByPointer(&fallback);
        }
      };
    } else {
      callback = [this, &variable, updateMethod]() {
        try {
          variable.setValue(updateMethod());
        } catch (const std::exception &e) {
          UA_LOG_ERROR(DCSLogger::getLogger(), UA_LOGCATEGORY_USERLAND,
                       "Variable %s update encountered error: %s . "
                       "Disconnecting device...",
                       variable.getFullName().c_str(), e.what());
          fastEvent(
              this->getName(), 40,
              "Device communication error occured. Disconnecting device.");
          this->device.resetConnectionStream();
        }
      };
    }
    if (threaded) {
      auto threadedCallback = [this, callback]() {
        this->deviceThread.push_back(callback);
      };
      variable.addUpdate(interval_ms, threadedCallback);
    } else {
      variable.addUpdate(interval_ms, callback);
    }
  }

  template <class T, class B>
  void addVariableUpdate(DCSVariable &variable, uint interval_ms,
                         T updateMethod, B instance, bool threaded = true,
                         bool deviceProtected = true) {
    auto callback = std::bind(updateMethod, instance);
    return addVariableUpdate(variable, interval_ms, callback, threaded,
                             deviceProtected);
  }

protected:
  DCSDeviceController() = default;
  void addChildren() override { addConnection(); }
  void addConnection() {
    device.resetConnectionStream();
    auto &v = addVariable("status", UA_TYPES[UA_TYPES_BOOLEAN]);
    addVariableUpdate(v, 1000, [this]() { return getConnectionStatus(); });
    addControllerMethod("connect", "Connects device",
                        {{"Address", "Host address", UA_TYPES[UA_TYPES_STRING]},
                         {"Port", "Host port", UA_TYPES[UA_TYPES_INT32]}},
                        {}, &DCSDeviceController::connectDevice, this);
    addControllerMethod("disconnect", "Disconnects device", {}, {},
                        &DCSDeviceController::disconnectDevice, this);
  }

  void dumpConfig(const UA_Variant *, UA_Variant *) {
    auto v = variables.at("configuration")->getValueByVariant();
    std::cout << DCSUAJson::toString(&v, &UA_TYPES[UA_TYPES_VARIANT])
              << std::endl;
  }

  Device device;
  DCSWorkerThread deviceThread;

protected:
  virtual void disconnectDevice(const UA_Variant *, UA_Variant *) {
    device.resetConnectionStream();
  }

  virtual void connectDevice(const UA_Variant *input, UA_Variant *output) {
    UA_String host = *(UA_String *)input[0].data;
    if (host.length == 0) {
      return;
    }
    std::string address(reinterpret_cast<char *>(host.data), host.length);
    UA_Int32 port = *(UA_Int32 *)input[1].data;
    try {
      auto stream = TCPConnector::connect(address.c_str(), port);
      device.setConnectionStream(stream);
    } catch (const std::runtime_error &e) {
      UA_LOG_WARNING(DCSLogger::getLogger(), UA_LOGCATEGORY_USERLAND,
                     "%s device controller catched: \"%s\"", objectName.c_str(),
                     e.what());
    }
  }

  bool getConnectionStatus() { return device.isConnected(); }
};

#endif // DCS_DEVICE_CONTROLLER_H
