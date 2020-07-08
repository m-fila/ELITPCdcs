#include "json.hpp"
#include <fstream>
#include <string>

using json = nlohmann::json;
namespace ConfigLoader {
json getMasterConfig(int argc, char **argv) {
  json config;
  std::string configPath;
  if (argc > 1) {
    configPath = argv[1];
  } else {
    auto envConfig = getenv("DCS_CONFIG_FILE");
    configPath = envConfig != nullptr
                     ? envConfig
                     : std::string(getenv("HOME")) + "/.dcs/dcs.json";
  }
  std::ifstream ifs(configPath);
  if (ifs.is_open()) {
    ifs >> config;
    return config;
  } else {
    throw std::runtime_error("Can't find config file");
  }
}
}