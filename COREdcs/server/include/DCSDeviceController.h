#ifndef DCS_DEVICE_CONTROLLER_H
#define DCS_DEVICE_CONTROLLER_H
#include "DCSLogger.h"
#include "DCSObject.h"
#include "DCSUAJson.h"
#include "DCSWorkerThread.h"
#include "TCPConnector.h"
#include <iostream>
using namespace std::placeholders;

template <class Device> class DCSDeviceController : public DCSObject {

  friend DCSServer;

public:
  template <class T>
  void addControllerMethod(std::string methodName,
                           std::string methodDescription,
                           std::vector<methodArgs> inputArgs,
                           std::vector<methodArgs> outputArgs,
                           const T &methodBody, bool threaded = true) {
    auto newBody = [methodBody, methodName, this](std::vector<UA_Variant> input,
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
      for (auto &i : input) {
        UA_Variant_deleteMembers(&i);
      }
    };
    if (threaded) {
      auto newerBody = [newBody, this](std::vector<UA_Variant> i,
                                       UA_Variant *o) {
        deviceThread.push_front(std::bind(newBody, i, o));
      };
      DCSObject::addMethod(methodName, methodDescription, inputArgs, outputArgs,
                           newerBody);
    } else {
      DCSObject::addMethod(methodName, methodDescription, inputArgs, outputArgs,
                           newBody);
    }
  }

  template <class T, class B>
  void
  addControllerMethod(std::string methodName, std::string methodDescription,
                      std::vector<methodArgs> inputArgs,
                      std::vector<methodArgs> outputArgs, const T &methodBody,
                      B instance, bool threaded = true) {

    std::function<void(std::vector<UA_Variant>, UA_Variant *)> newBody =
        std::bind(methodBody, instance, _1, _2);
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
  DCSDeviceController(UA_Server *server, std::string name)
      : DCSObject(server, name) {
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

  void dumpConfig(std::vector<UA_Variant>, UA_Variant *) {
    auto v = variables.at("configuration")->getValueByVariant();
    std::cout << DCSUAJson::toString(&v, &UA_TYPES[UA_TYPES_VARIANT])
              << std::endl;
  }

  Device device;
  DCSWorkerThread deviceThread;

private:
  void disconnectDevice(std::vector<UA_Variant>, UA_Variant *) {
    device.resetConnectionStream();
  }

  void connectDevice(std::vector<UA_Variant> input, UA_Variant *output) {
    UA_String host = *(UA_String *)input[0].data;
    if (host.length == 0) {
      return;
    }

    const char *address = reinterpret_cast<char *>(host.data);
    UA_Int32 port = *(UA_Int32 *)input[1].data;
    try {
      auto stream = TCPConnector::connect(address, port);
      device.setConnectionStream(stream);
    } catch (const std::runtime_error &e) {
      // TODO: return to method or send event informing of connection fail or
      // success
      UA_LOG_WARNING(DCSLogger::getLogger(),UA_LOGCATEGORY_USERLAND, "%s device controller catched: \"%s\"", objectName.c_str(), e.what());
    }
  }

  bool getConnectionStatus() { return device.isConnected(); }
};

#endif // DCS_DEVICE_CONTROLLER_H
