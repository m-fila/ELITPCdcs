#ifndef DCS_DEVICE_CONTROLLER_H
#define DCS_DEVICE_CONTROLLER_H
#include "DCSObject.h"
#include "DCSWorkerThread.h"
#include "TCPConnector.h"
using namespace std::placeholders;

template <class Device> class DCSDeviceController : public DCSObject {

  friend DCSServer;

public:
  DCSDeviceController(UA_Server *server, std::string name)
      : DCSObject(server, name) {
    auto &v = addVariable("status", UA_TYPES[UA_TYPES_BOOLEAN]);
    // v.addUpdate(1000,[this](){threadJob(&DCSDeviceController::getStatus);});
    /*  addMethod("disconnect", "Disconnects device", {}, {},
                [this](const UA_Variant *, UA_Variant *) {
                  threadJob(&DCSDeviceController<Device>::disconnectDevice);
                  //
       deviceThread.pushJob(&DCSDeviceController<Device>::disconnectDevice,this);

                });
      */
    // addVariableUpdate(v,1000,&DCSDeviceController::getStatus,true);
    addControllerMethod("connect", "Connects device", {{"Address","Host address",UA_TYPES[UA_TYPES_STRING]},{"Port","Host port",UA_TYPES[UA_TYPES_INT32]}}, {},
                        &DCSDeviceController::connectDevice);
    addControllerMethod("try1", "Connects device", {{"aaa","ssss",UA_TYPES[UA_TYPES_BOOLEAN]}}, {{"aaa","ssss",UA_TYPES[UA_TYPES_BOOLEAN]}},
                        &DCSDeviceController::connectDevice);
    addControllerMethod("try2", "Connects device", {}, {},
                        &DCSDeviceController::connectDevice, false);
    //  addMethod("connect", "Connects device", {}, {},
    //  std::bind(&DCSDeviceController::connectDevice,this,_1,_2));
    addVariableUpdate(v,1000,&DCSDeviceController::getConnectionStatus);
  }

protected:
  Device device;
  DCSWorkerThread deviceThread;

  template <class Job, class... Args> void threadJob(Job f, Args... args) {
    deviceThread.pushJob(f, this, args...);
  }

  template <class T>
  void addControllerMethod(std::string methodName,
                           std::string methodDescription,
                           std::vector<methodArgs> inputs,
                           std::vector<methodArgs> outputs, const T &methodBody,
                           bool threaded = true) {

    std::function<void(const UA_Variant *, UA_Variant *)> newBody;
    if (threaded) {
      newBody = [this, methodBody](const UA_Variant *inputs,
                                   UA_Variant *outputs) {
        threadJob(methodBody, inputs, outputs);
      };
    } else {
      newBody = std::bind(methodBody, this, _1, _2);
    }
    DCSObject::addMethod(methodName, methodDescription, inputs, outputs,
                         newBody);
  }


template <class T>
void addVariableUpdate(DCSVariable & variable, uint interval_ms, T updateMethod, bool threaded=true){
  variable.setValue((this->*updateMethod)());
  auto val=std::bind(updateMethod,this);
  auto callback=[val,&variable](){
    //val();
    variable.setValue( val()  );
  };
  if(threaded){
  auto threadedCallback=[this,callback](){this->deviceThread.pushJob(callback);};
  variable.addUpdate(interval_ms,threadedCallback);}
  else{
    variable.addUpdate(interval_ms,callback);
    }
  }


template<class T>
void setVariableValue(DCSVariable& v, T newVal){
  v.setValue(newVal);
}

private:
  void disconnectDevice(const UA_Variant *, UA_Variant *) {
    device.resetConnectionStream();
  }

  void connectDevice(const UA_Variant *input, UA_Variant *output) {
    UA_Int32 port = *(UA_Int32*)input[1].data;
    UA_String host = *(UA_String*)input[0].data;
    std::string address(reinterpret_cast<char*>(host.data));
    try{
    auto stream =TCPConnector::connect(address.c_str(),port);
   
    device.setConnectionStream(stream);
   }
        catch(std::runtime_error e){
        //TODO: return to method or send event informing of connection fail or success 
        std::cerr<<objectName+" device controller catched: "<<e.what()<<std::endl;
    }
  }


  bool getConnectionStatus(){std::cout<<"aaa"<<std::endl; return device.isConnected();}
  void getStatus() { variables.at("status").setValue(device.isConnected()); }
};

#endif // DCS_DEVICE_CONTROLLER_H
