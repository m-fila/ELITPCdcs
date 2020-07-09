#include "DCSDummyController.h"
#include "json.hpp"
#include <fstream>
#include <iomanip>
/*
void DCSDummyController::addProfiles(Options options) {
  auto profiles = addVariable("enabledProfiles", UA_TYPES[UA_TYPES_STRING]);
  updateProfiles(profiles);
  auto activeProfile = addVariable("activeProfile", UA_TYPES[UA_TYPES_STRING]);
  auto initProfile =
      options.contains("profile")
          ? UA_STRING_ALLOC(options.at("profile").get<std::string>().c_str())
          : UA_STRING_ALLOC("None");
  activeProfile.setValue(initProfile);
  UA_String_deleteMembers(&initProfile);

  addMethod("dumpProfile", "dump profile",
            {{"name", "current profile name", UA_TYPES[UA_TYPES_STRING]}}, {},
            std::bind(&DCSDummyController::dumpProfile, this, profiles,
                      std::placeholders::_1, std::placeholders::_2));
  addMethod("setProfile", "set active profile",
            {{"key", "key", UA_TYPES[UA_TYPES_STRING]}}, {},
            std::bind(&DCSDummyController::setProfile, this, activeProfile,
                      std::placeholders::_1, std::placeholders::_2));
  addControllerMethod(
      "applyProfile", "apply active profile", {}, {},
      [this, activeProfile, profiles](const UA_Variant *in, UA_Variant *out) {
        applyProfile(activeProfile, profiles, in, out);
      });
}

void DCSDummyController::dumpProfile(DCSVariable &profiles,
                                     const UA_Variant *input,
                                     UA_Variant *output) {
  std::string name;
  {
    auto nameTmp = static_cast<UA_String *>(input[0].data);
    name =
        std::string(reinterpret_cast<char *>(nameTmp->data), nameTmp->length);
  }
  auto configPath =
      std::string(getenv("HOME")) + "/.dcs/" + objectType + ".json";
  std::ifstream fs(configPath);
  if (fs.is_open()) {
    nlohmann::json config;
    fs >> config;
    config[name] = {{"myVal", 6}};
    UA_LOG_INFO(DCSLogger::getLogger(), UA_LOGCATEGORY_USERLAND, "%s",
                config.dump().c_str());
    fs.close();
    std::ofstream ofs(configPath);
    if (ofs.is_open()) {
      ofs << std::setw(4) << config;
    }
    auto str = UA_STRING_ALLOC(config.dump().c_str());
    profiles.setValue(str);
  } else {
    UA_LOG_WARNING(DCSLogger::getLogger(), UA_LOGCATEGORY_USERLAND,
                   "%s config \"%s\"not found", objectName.c_str(),
                   configPath.c_str());
  }
}

void DCSDummyController::updateProfiles(DCSVariable &profiles) {
  auto configPath =
      std::string(getenv("HOME")) + "/.dcs/" + objectType + ".json";
  std::ifstream ifs(configPath);
  if (ifs.is_open()) {
    nlohmann::json config;
    ifs >> config;
    auto str = UA_STRING_ALLOC(config.dump().c_str());
    profiles.setValue(str);
  } else {
    UA_LOG_WARNING(DCSLogger::getLogger(), UA_LOGCATEGORY_USERLAND,
                   "%s config \"%s\"not found", objectName.c_str(),
                   configPath.c_str());
  }
}

void DCSDummyController::setProfile(DCSVariable &profile,
                                    const UA_Variant *input,
                                    UA_Variant *output) {
  auto str = static_cast<UA_String *>(input[0].data);
  profile.setValueByPointer(str);
}

void DCSDummyController::applyProfile(const DCSVariable &profile,
                                      const DCSVariable &profiles,
                                      const UA_Variant *input,
                                      UA_Variant *output) {
  auto profileTmp = profile.getValue<UA_String>();
  auto profilesTmp = profiles.getValue<UA_String>();
  std::string profileStr(reinterpret_cast<char *>(profileTmp.data),
                         profileTmp.length);
  nlohmann::json profilesJson = nlohmann::json::parse(std::string(
      reinterpret_cast<char *>(profilesTmp.data), profilesTmp.length));
  if (profilesJson.contains(profileStr)) {
    auto profileBody = profilesJson.at(profileStr);
    UA_LOG_INFO(DCSLogger::getLogger(), UA_LOGCATEGORY_USERLAND,
                "%s applying %s profile \"%s\" : %s", objectName.c_str(),
                objectType.c_str(), profileStr.c_str(),
                profileBody.dump().c_str());
    parseProfile(profileBody);
  } else {
    UA_LOG_ERROR(DCSLogger::getLogger(), UA_LOGCATEGORY_USERLAND,
                 "%s config doesn't contain \"%s\" profile", objectType.c_str(),
                 profileStr.c_str());
  }
}
*/
