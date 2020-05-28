#ifndef DCS_DEVICE_CONTROLLER_H
#define DCS_DEVICE_CONTROLLER_H
#include "DCSObject.h"
#include "DCSWorkerThread.h"
#include "TCPConnector.h"
#include "DCSUAJson.h"

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
    auto newBody = [methodBody](std::vector<UA_Variant> input,
                                UA_Variant *output) {
      methodBody(input, output);
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
                         T updateMethod, bool threaded = true) {
    auto callback = [this, &variable, updateMethod]() {
      if (device.isConnected()) {
        variable.setValue(updateMethod());
      } else {
        void *fallback;
        UA_init(&fallback, variable.getdataType());
        variable.setValueByPointer(&fallback);
      }
    };

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
                         T updateMethod, B instance, bool threaded = true) {
    auto callback = std::bind(updateMethod, instance);
    return addVariableUpdate(variable, interval_ms, callback, threaded);
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



  void dumpConfig(std::vector<UA_Variant>, UA_Variant *){
   auto v=variables.at("configuration").getValueByVariant();
   std::cout<<UAJson::toString(&v,&UA_TYPES[UA_TYPES_VARIANT])<<std::endl;
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
    } catch (const std::runtime_error& e) {
      // TODO: return to method or send event informing of connection fail or
      // success
      std::cerr << objectName + " device controller catched: " << e.what()
                << std::endl;
    }
  }

  bool getConnectionStatus() {
    return device.isConnected();
  }

};

#endif // DCS_DEVICE_CONTROLLER_H
