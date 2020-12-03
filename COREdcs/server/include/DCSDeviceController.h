#ifndef DCS_DEVICE_CONTROLLER_H
#define DCS_DEVICE_CONTROLLER_H
#include "DCSLogger.h"
#include "DCSObject.h"
#include "DCSServer.h"
#include "DCSUAJson.h"
#include "DCSWorkerThread.h"
#include "TCPConnector.h"
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
    template <class T>
    void addControllerMethod(std::string methodName, std::string methodDescription,
                             std::vector<DCSObject::MethodArgs> inputArgs,
                             std::vector<DCSObject::MethodArgs> outputArgs,
                             const T &methodBody, bool threaded = true);
    // interface for adding device related methods to opc objects
    // if threaded==true metohdBody will be wrapped for execution in device worker
    // thread
    template <class T, class B>
    void addControllerMethod(std::string methodName, std::string methodDescription,
                             std::vector<DCSObject::MethodArgs> inputArgs,
                             std::vector<DCSObject::MethodArgs> outputArgs,
                             const T &methodBody, B instance, bool threaded = true);

    template <class T>
    void addVariableUpdate(DCSVariable &variable, size_t interval_ms, T updateMethod,
                           bool threaded = true, bool deviceProtected = true);

    template <class T, class B>
    void addVariableUpdate(DCSVariable &variable, size_t interval_ms, T updateMethod,
                           B instance, bool threaded = true, bool deviceProtected = true);

  protected:
    // Non trivial constructor
    DCSDeviceController(){};

    void addChildren(const Options &options) override { addConnection(); }
    void addConnection();
    void addProfiles(DCSVariable &configVariable, const Options &options);
    virtual void parseProfile(const nlohmann::json &profile) {}

    Device device;
    DCSWorkerThread deviceThread;

  private:
    virtual void disconnectDevice(const UA_Variant *in, UA_Variant *out) {
        device.resetConnectionStream();
    }

    virtual void connectDevice(const UA_Variant *input, UA_Variant *output);

    bool getConnectionStatus() { return device.isConnected(); }

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
    auto *host = static_cast<UA_String *>(input[0].data);
    if(host->length == 0) {
        return;
    }
    std::string address(reinterpret_cast<char *>(host->data), host->length);
    auto port = *static_cast<UA_Int32 *>(input[1].data);
    try {
        auto stream = TCPConnector::connect(address.c_str(), port);
        device.setConnectionStream(stream);
    } catch(const std::exception &e) {
        UA_LOG_WARNING(DCSLogger::getLogger(), UA_LOGCATEGORY_USERLAND,
                       "%s device controller catched: \"%s\"", objectName.c_str(),
                       e.what());
    }
    if(getConnectionStatus()) {
        auto found = methods.find("applyProfile");
        if(found != methods.end()) {
            found->second(nullptr, nullptr);
        }
    }
}
template <class Device> void DCSDeviceController<Device>::addConnection() {
    device.resetConnectionStream();
    auto &v = addVariable("status", UA_TYPES[UA_TYPES_BOOLEAN]);
    addVariableUpdate(v, 1000, [this]() { return getConnectionStatus(); }, true, false);
    addControllerMethod("connect", "Connects device",
                        {{"Address", "Host address", UA_TYPES[UA_TYPES_STRING]},
                         {"Port", "Host port", UA_TYPES[UA_TYPES_INT32]}},
                        {}, &DCSDeviceController::connectDevice, this);
    addControllerMethod("disconnect", "Disconnects device", {}, {},
                        &DCSDeviceController::disconnectDevice, this);
}

template <class Device>
template <class T>
void DCSDeviceController<Device>::addControllerMethod(
    std::string methodName, std::string methodDescription,
    std::vector<DCSObject::MethodArgs> inputArgs,
    std::vector<DCSObject::MethodArgs> outputArgs, const T &methodBody, bool threaded) {
    // wrap with exception
    auto newBody = [methodBody, methodName, this](const UA_Variant *input,
                                                  UA_Variant *output) {
        try {
            methodBody(input, output);
        } catch(const exception &e) {
            UA_LOG_ERROR(DCSLogger::getLogger(), UA_LOGCATEGORY_USERLAND,
                         "Method %s encountered error: %s . Disconnecting device...",
                         methodName.c_str(), e.what());
            fastEvent(this->getName(), 400,
                      "Device communication error occured. Disconnecting device.");
            this->device.resetConnectionStream();
        }
    };

    if(threaded) {
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
    bool threaded) {
    std::function<void(const UA_Variant *, UA_Variant *)> newBody =
        std::bind(methodBody, instance, std::placeholders::_1, std::placeholders::_2);
    addControllerMethod(methodName, methodDescription, inputArgs, outputArgs, newBody,
                        threaded);
}
template <class Device>
template <class T>
void DCSDeviceController<Device>::addVariableUpdate(DCSVariable &variable,
                                                    size_t interval_ms, T updateMethod,
                                                    bool threaded, bool deviceProtected) {
    std::function<void()> callback;
    if(deviceProtected) {
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
    if(threaded) {
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
                                                    B instance, bool threaded,
                                                    bool deviceProtected) {
    auto callback = std::bind(updateMethod, instance);
    return addVariableUpdate(variable, interval_ms, callback, threaded, deviceProtected);
}

template <class Device>
void DCSDeviceController<Device>::addProfiles(DCSVariable &configVariable,
                                              const Options &options) {
    auto profiles = addVariable("enabledProfiles", UA_TYPES[UA_TYPES_STRING]);
    updateProfiles(profiles);
    auto selectedProfile = addVariable("selectedProfile", UA_TYPES[UA_TYPES_STRING]);
    auto initProfile =
        options.contains("profile")
            ? UA_STRING_ALLOC(options.at("profile").get<std::string>().c_str())
            : UA_STRING_ALLOC("None");
    selectedProfile.setValue(initProfile);
    UA_String_deleteMembers(&initProfile);

    addMethod("saveProfile", "save profile",
              {{"name", "current profile name", UA_TYPES[UA_TYPES_STRING]}}, {},
              std::bind(&DCSDeviceController<Device>::saveProfile, this, profiles,
                        configVariable, std::placeholders::_1, std::placeholders::_2));
    addMethod("setProfile", "set active profile",
              {{"key", "key", UA_TYPES[UA_TYPES_STRING]}}, {},
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

#endif  // DCS_DEVICE_CONTROLLER_H
