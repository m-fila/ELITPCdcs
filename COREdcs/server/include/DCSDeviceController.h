#ifndef DCS_DEVICE_CONTROLLER_H
#define DCS_DEVICE_CONTROLLER_H
#include "DCSLogger.h"
#include "DCSObject.h"
#include "DCSServer.h"
#include "DCSUAJson.h"
#include "DCSWorkerThread.h"
#include "TCPConnector.h"
#include "open62541/types_dcsnodeset_generated.h"
#include "open62541/types_dcsnodeset_generated_handling.h"
#include <fstream>
#include <iomanip>
// DCSObject encapsulating Device controller. Device specific controllers should
// be derived from this template.
// Provides connection and profile interfaces.
template <class Device> class DCSDeviceController : public DCSObject {

    friend DCSServer;

  public:
    // interface for adding device related methods to opc objects
    // if threaded==true metohdBody will be wrapped for execution in device worker
    // thread
    enum class Execution { Blocking, Parallel };
    enum class Protection { None, Connection };
    template <class T>
    void addControllerMethod(std::string methodName, std::string methodDescription,
                             std::vector<DCSObject::MethodArgs> inputArgs,
                             std::vector<DCSObject::MethodArgs> outputArgs,
                             const T &methodBody,
                             Execution threaded = Execution::Parallel);
    // interface for adding device related methods to opc objects
    // if threaded==true metohdBody will be wrapped for execution in device worker
    // thread
    template <class T, class B>
    void addControllerMethod(std::string methodName, std::string methodDescription,
                             std::vector<DCSObject::MethodArgs> inputArgs,
                             std::vector<DCSObject::MethodArgs> outputArgs,
                             const T &methodBody, B instance,
                             Execution threaded = Execution::Parallel);

    template <class T>
    void addVariableUpdate(DCSVariable &variable, size_t interval_ms, T updateMethod,
                           const Options &options = {},
                           Execution threaded = Execution::Parallel,
                           Protection deviceProtected = Protection::Connection);

    template <class T, class B>
    void addVariableUpdate(DCSVariable &variable, size_t interval_ms, T updateMethod,
                           B instance, const Options &options = {},
                           Execution threaded = Execution::Parallel,
                           Protection deviceProtected = Protection::Connection);

  protected:
    // Non trivial constructor
    DCSDeviceController(){};

    void addChildren(const Options &options) override {
        addConnection(options);
        addLabelInfo(options);
    }
    void addConnection(const Options &options);
    void virtual postConnect() {}
    void addLabelInfo(const Options &options);
    void addProfiles(DCSVariable &configVariable, const Options &options);

    virtual void parseProfile(const nlohmann::json &profile) {}
    virtual UA_DeviceInfo getDeviceInfo();
    Device device;
    DCSWorkerThread deviceThread;

  private:
    virtual void disconnectDevice(const UA_Variant *in, UA_Variant *out) {
        device.resetConnectionStream();
    }

    virtual void connectDevice(const UA_Variant *input, UA_Variant *output);

    virtual void setConnectionParameters(const UA_Variant *input, UA_Variant *output);

    bool isConnected() { return device.isConnected(); }

    void saveProfile(DCSVariable &profiles, DCSVariable &configVariable,
                     const UA_Variant *input, UA_Variant *output);
    void setProfile(DCSVariable &profile, const UA_Variant *input, UA_Variant *output);
    void applyProfile(const DCSVariable &profile, const DCSVariable &profiles,
                      const UA_Variant *input, UA_Variant *output);
    void updateProfiles(DCSVariable &profiles);
};

template <class Device>
void DCSDeviceController<Device>::connectDevice(const UA_Variant *input,
                                                UA_Variant *output) {
    auto param = variables.at("connectionParameters")->getValue<UA_ParametersTCP>();
    UA_LOG_INFO(DCSLogger::getLogger(), UA_LOGCATEGORY_USERLAND,
                "%s device connecting to %s:%i", objectName.c_str(), param.address.data,
                param.port);
    if(param.address.length == 0) {
        return;
    }
    std::string address(reinterpret_cast<char *>(param.address.data),
                        param.address.length);
    try {
        auto stream = TCPConnector::connect(address, param.port);
        device.setConnectionStream(stream);
    } catch(const std::exception &e) {
        UA_LOG_WARNING(DCSLogger::getLogger(), UA_LOGCATEGORY_USERLAND,
                       "%s device controller catched: \"%s\"", objectName.c_str(),
                       e.what());
    }
    if(isConnected()) {
        {
            auto found = variables.find("deviceInfo");
            if(found != variables.end()) {
                found->second->setValue(getDeviceInfo());
            }
        }
        postConnect();
        {
            auto found = methods.find("applyProfile");
            if(found != methods.end()) {
                found->second(nullptr, nullptr);
            }
        }
    }
}

template <class Device>
void DCSDeviceController<Device>::setConnectionParameters(const UA_Variant *input,
                                                          UA_Variant *output) {
    if(isConnected()) {
        UA_LOG_WARNING(DCSLogger::getLogger(), UA_LOGCATEGORY_USERLAND,
                       "%s device: cannot change connection parameters of already "
                       "connected device.",
                       objectName.c_str());
        return;
    }
    UA_ParametersTCP par;
    UA_ParametersTCP_init(&par);
    UA_String_copy(static_cast<const UA_String *>(input[0].data), &par.address);
    par.port = *static_cast<UA_Int32 *>(input[1].data);
    variables.at("connectionParameters")->setValue(par);
}

template <class Device>
void DCSDeviceController<Device>::addConnection(const Options &options) {
    device.resetConnectionStream();
    auto &s = addVariable("status", &UA_TYPES[UA_TYPES_BOOLEAN]);
    addVariableUpdate(s, 1000, [this]() { return isConnected(); }, {},
                      Execution::Parallel, Protection::None);
    auto &p = addVariable("connectionParameters",
                          &UA_TYPES_DCSNODESET[UA_TYPES_DCSNODESET_PARAMETERSTCP]);
    addVariable("deviceInfo", &UA_TYPES_DCSNODESET[UA_TYPES_DCSNODESET_DEVICEINFO]);
    addControllerMethod("connect", "Connects device", {}, {},
                        &DCSDeviceController::connectDevice, this);
    addControllerMethod("setConnectionParameters", "Sets connection parameters",
                        {{"Address", "Host address", &UA_TYPES[UA_TYPES_STRING]},
                         {"Port", "Host port", &UA_TYPES[UA_TYPES_INT32]}},
                        {}, &DCSDeviceController::setConnectionParameters, this);
    addControllerMethod("disconnect", "Disconnects device", {}, {},
                        &DCSDeviceController::disconnectDevice, this);
    if(options.contains("address") and options.contains("port")) {
        try {
            UA_ParametersTCP param;
            UA_ParametersTCP_init(&param);
            param.address =
                UA_STRING_ALLOC(options.at("address").get<std::string>().c_str());
            param.port = std::stoi(options.at("port").get<std::string>());
            p.setValue(param);
        } catch(const std::exception &e) {
            UA_LOG_WARNING(DCSLogger::getLogger(), UA_LOGCATEGORY_SERVER,
                           "%s invalid configuration parameters address: %s port: %s",
                           objectName.c_str(), options.at("address").dump().c_str(),
                           options.at("port").dump().c_str());
        }
    }
}

template <class Device>
void DCSDeviceController<Device>::addLabelInfo(const Options &options) {
    UA_LabelInfo info;
    UA_LabelInfo_init(&info);
    if(options.contains("labels")) {
        auto &labels = options.at("labels");
        info.description = UA_STRING_ALLOC(labels.value("description", "a").c_str());
        if(labels.contains("channels") and labels.at("channels").is_array()) {
            info.channelLabelsSize = labels.at("channels").size();
            info.channelLabels = static_cast<UA_LabelItemInfo *>(
                UA_Array_new(info.channelLabelsSize,
                             &UA_TYPES_DCSNODESET[UA_TYPES_DCSNODESET_LABELITEMINFO]));
            for(auto it = labels.at("channels").begin();
                it != labels.at("channels").end(); ++it) {
                auto i = std::distance(labels.at("channels").begin(), it);
                UA_LabelItemInfo in;
                UA_LabelItemInfo_init(&in);
                info.channelLabels[i] = in;
                info.channelLabels[i].description =
                    UA_STRING_ALLOC(it->value("description", "z").c_str());
                info.channelLabels[i].label =
                    UA_STRING_ALLOC(it->value("label", "y").c_str());
            }
        }
    }
    auto &l =
        addVariable("labelInfo", &UA_TYPES_DCSNODESET[UA_TYPES_DCSNODESET_LABELINFO]);
    l.setValue(info);
}

template <class Device>
template <class T>
void DCSDeviceController<Device>::addControllerMethod(
    std::string methodName, std::string methodDescription,
    std::vector<DCSObject::MethodArgs> inputArgs,
    std::vector<DCSObject::MethodArgs> outputArgs, const T &methodBody,
    Execution threaded) {
    // wrap with exception
    auto newBody = [methodBody, methodName, this](const UA_Variant *input,
                                                  UA_Variant *output) {
        try {
            methodBody(input, output);
        } catch(const std::exception &e) {
            UA_LOG_ERROR(DCSLogger::getLogger(), UA_LOGCATEGORY_USERLAND,
                         "Method %s encountered error: %s . Disconnecting device...",
                         methodName.c_str(), e.what());
            fastEvent(this->getName(), 400,
                      "Device communication error occured. Disconnecting device.");
            this->device.resetConnectionStream();
        }
    };

    if(threaded == Execution::Parallel) {
        DCSObject::addMethod(methodName, methodDescription, inputArgs, outputArgs,
                             newBody, &this->deviceThread);
    } else {
        DCSObject::addMethod(methodName, methodDescription, inputArgs, outputArgs,
                             newBody);
    }
}
template <class Device>
template <class T, class B>
void DCSDeviceController<Device>::addControllerMethod(
    std::string methodName, std::string methodDescription,
    std::vector<DCSObject::MethodArgs> inputArgs,
    std::vector<DCSObject::MethodArgs> outputArgs, const T &methodBody, B instance,
    Execution threaded) {
    std::function<void(const UA_Variant *, UA_Variant *)> newBody =
        std::bind(methodBody, instance, std::placeholders::_1, std::placeholders::_2);
    addControllerMethod(methodName, methodDescription, inputArgs, outputArgs, newBody,
                        threaded);
}
template <class Device>
template <class T>
void DCSDeviceController<Device>::addVariableUpdate(DCSVariable &variable,
                                                    size_t interval_ms, T updateMethod,
                                                    const Options &options,
                                                    Execution threaded,
                                                    Protection deviceProtected) {
    std::function<void()> callback;
    const std::string intervalTag{variable.getName() + "_interval"};
    if(options.contains(intervalTag)) {
        interval_ms = options.at(intervalTag).get<size_t>();
    } else if(options.contains("interval")) {
        interval_ms = options.at("interval").get<size_t>();
    }

    if(deviceProtected == Protection::Connection) {
        callback = [this, &variable, updateMethod]() {
            if(device.isConnected()) {
                try {
                    variable.setValue(updateMethod());
                } catch(const std::exception &e) {
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
                variable.setNull();
                // void *fallback = nullptr;
                // UA_init(&fallback, variable.getDataType());
                // variable.setValueByPointer(&fallback);
            }
        };
    } else {
        callback = [this, &variable, updateMethod]() {
            try {
                variable.setValue(updateMethod());
            } catch(const std::exception &e) {
                UA_LOG_ERROR(DCSLogger::getLogger(), UA_LOGCATEGORY_USERLAND,
                             "Variable %s update encountered error: %s . "
                             "Disconnecting device...",
                             variable.getFullName().c_str(), e.what());
                fastEvent(this->getName(), 40,
                          "Device communication error occured. Disconnecting device.");
                this->device.resetConnectionStream();
            }
        };
    }
    if(threaded == Execution::Parallel) {
        auto threadedCallback = [this, callback]() {
            this->deviceThread.push_back(callback);
        };
        variable.addUpdate(interval_ms, threadedCallback);
    } else {
        variable.addUpdate(interval_ms, callback);
    }
}

template <class Device>
template <class T, class B>
void DCSDeviceController<Device>::addVariableUpdate(DCSVariable &variable,
                                                    size_t interval_ms, T updateMethod,
                                                    B instance, const Options &options,
                                                    Execution threaded,
                                                    Protection deviceProtected) {
    auto callback = std::bind(updateMethod, instance);
    return addVariableUpdate(variable, interval_ms, callback, options, threaded,
                             deviceProtected);
}

template <class Device>
void DCSDeviceController<Device>::addProfiles(DCSVariable &configVariable,
                                              const Options &options) {
    auto profiles = addVariable("enabledProfiles", &UA_TYPES[UA_TYPES_STRING]);
    updateProfiles(profiles);
    auto selectedProfile = addVariable("selectedProfile", &UA_TYPES[UA_TYPES_STRING]);
    auto initProfile =
        options.contains("profile")
            ? UA_STRING_ALLOC(options.at("profile").get<std::string>().c_str())
            : UA_STRING_ALLOC("None");
    selectedProfile.setValue(initProfile);
    UA_String_deleteMembers(&initProfile);

    addMethod("saveProfile", "save profile",
              {{"name", "current profile name", &UA_TYPES[UA_TYPES_STRING]}}, {},
              std::bind(&DCSDeviceController<Device>::saveProfile, this, profiles,
                        configVariable, std::placeholders::_1, std::placeholders::_2));
    addMethod("setProfile", "set active profile",
              {{"key", "key", &UA_TYPES[UA_TYPES_STRING]}}, {},
              std::bind(&DCSDeviceController<Device>::setProfile, this, selectedProfile,
                        std::placeholders::_1, std::placeholders::_2));
    addControllerMethod(
        "applyProfile", "apply active profile", {}, {},
        [this, selectedProfile, profiles](const UA_Variant *in, UA_Variant *out) {
            applyProfile(selectedProfile, profiles, in, out);
        });
}

template <class Device>
void DCSDeviceController<Device>::saveProfile(DCSVariable &profiles,
                                              DCSVariable &configVariable,
                                              const UA_Variant *input,
                                              UA_Variant *output) {

    auto val = configVariable.getValueByVariant();
    auto value =
        nlohmann::json::parse(DCSUAJson::toString(&val, &UA_TYPES[UA_TYPES_VARIANT]));
    if(!value.contains("Body")) {
        UA_LOG_WARNING(DCSLogger::getLogger(), UA_LOGCATEGORY_USERLAND,
                       "%s config variable %s is empty. Skipping dumping current profile",
                       objectName.c_str(), configVariable.getName().c_str());
        return;
    }
    while(value.contains("Body")) {
        value = value.at("Body");
    }
    std::string name;
    {
        auto *nameTmp = static_cast<UA_String *>(input[0].data);
        name = std::string(reinterpret_cast<char *>(nameTmp->data), nameTmp->length);
    }
    if(name == "None") {
        UA_LOG_WARNING(DCSLogger::getLogger(), UA_LOGCATEGORY_USERLAND,
                       "%s : \"None\" reserved name for a profile", objectName.c_str());
        return;
    }
    auto configPath =
        DCSServer::getServerContext(server)->getProfileDir() + objectType + ".json";
    std::ifstream ifs(configPath);
    nlohmann::json config = {};
    if(ifs.is_open()) {
        try {
            ifs >> config;
        } catch(const std::exception &e) {
            UA_LOG_ERROR(DCSLogger::getLogger(), UA_LOGCATEGORY_USERLAND,
                         "%s config \"%s\" is corrupted. Skipping and overwriting",
                         objectName.c_str(), configPath.c_str());
            config = {};
        }
        ifs.close();
    } else {
        UA_LOG_WARNING(DCSLogger::getLogger(), UA_LOGCATEGORY_USERLAND,
                       "%s config \"%s\" not found. Creating new file",
                       objectName.c_str(), configPath.c_str());
    }
    std::ofstream ofs(configPath);
    if(ofs.is_open()) {
        config[name] = value;
        ofs << std::setw(4) << config;
    }
    auto str = UA_STRING_ALLOC(config.dump().c_str());
    profiles.setValue(str);
}

template <class Device>
void DCSDeviceController<Device>::updateProfiles(DCSVariable &profiles) {
    auto configPath =
        DCSServer::getServerContext(server)->getProfileDir() + objectType + ".json";
    std::ifstream ifs(configPath);
    if(ifs.is_open()) {
        nlohmann::json config;
        ifs >> config;
        auto str = UA_STRING_ALLOC(config.dump().c_str());
        profiles.setValue(str);
    } else {
        UA_LOG_WARNING(DCSLogger::getLogger(), UA_LOGCATEGORY_SERVER,
                       "%s config \"%s\" not found", objectName.c_str(),
                       configPath.c_str());
    }
}

template <class Device>
void DCSDeviceController<Device>::setProfile(DCSVariable &profile,
                                             const UA_Variant *input,
                                             UA_Variant *output) {
    auto *str = static_cast<UA_String *>(input[0].data);
    profile.setValueByPointer(str);
}

template <class Device>
void DCSDeviceController<Device>::applyProfile(const DCSVariable &profile,
                                               const DCSVariable &profiles,
                                               const UA_Variant *input,
                                               UA_Variant *output) {
    UA_String profileTmp;
    UA_String profilesTmp;
    try {
        profileTmp = profile.getValue<UA_String>();
        profilesTmp = profiles.getValue<UA_String>();
    } catch(std::runtime_error &e) {
        UA_LOG_ERROR(
            DCSLogger::getLogger(), UA_LOGCATEGORY_USERLAND,
            "%s encountered error when reading profile. Skipping applying profile",
            objectName.c_str());
        return;
    }
    std::string profileStr(reinterpret_cast<char *>(profileTmp.data), profileTmp.length);
    if(profileStr == "None") {
        UA_LOG_WARNING(DCSLogger::getLogger(), UA_LOGCATEGORY_USERLAND,
                       "%s requested applying special profile \"None\". Skipping",
                       objectName.c_str());
        return;
    }
    nlohmann::json profilesJson = nlohmann::json::parse(
        std::string(reinterpret_cast<char *>(profilesTmp.data), profilesTmp.length));
    if(profilesJson.contains(profileStr)) {
        auto profileBody = profilesJson.at(profileStr);
        UA_LOG_INFO(DCSLogger::getLogger(), UA_LOGCATEGORY_USERLAND,
                    "%s applying %s profile \"%s\" : %s", objectName.c_str(),
                    objectType.c_str(), profileStr.c_str(), profileBody.dump().c_str());
        parseProfile(profileBody);
    } else {
        UA_LOG_ERROR(DCSLogger::getLogger(), UA_LOGCATEGORY_USERLAND,
                     "%s config doesn't contain \"%s\" profile", objectType.c_str(),
                     profileStr.c_str());
    }
}

template <class Device> UA_DeviceInfo DCSDeviceController<Device>::getDeviceInfo() {
    UA_DeviceInfo info;
    UA_DeviceInfo_init(&info);
    info.vendor = UA_STRING_ALLOC(device.getVendor().c_str());
    info.model = UA_STRING_ALLOC(device.getModel().c_str());
    info.serialNumber = UA_STRING_ALLOC(device.getSerialNumber().c_str());
    info.partNumber = UA_STRING_ALLOC(device.getPartNumber().c_str());
    info.firmwareVersion = UA_STRING_ALLOC(device.getFirmwareVersion().c_str());
    info.hardwareVersion = UA_STRING_ALLOC(device.getHardwareVersion().c_str());
    return info;
}

#endif  // DCS_DEVICE_CONTROLLER_H
