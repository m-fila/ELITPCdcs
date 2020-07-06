#include "DCSArt.h"
//#include "DCSDT1415ETController.h"
//#include "DCSDummyController.h"
//#include "DCSHMPController.h"
#include "DCSHistoryBackendInflux.h"
#include "DCSLogger.h"
//#include "DCSMKS910Controller.h"
//#include "DCSN1471Controller.h"
//#include "DCSPiWeatherController.h"
#include "DCSState.h"
//#include "DCSTPG362Controller.h"
#include "ELITPCServer.h"
#include "configloader.h"
using json = nlohmann::json;
int main(int argc, char *argv[]) {
  std::cout << DCSArt::ascii << std::endl;
  json config = ConfigLoader::getMasterConfig(argc, argv);

  DCSLogger::setFile(config.at("logFile").get<std::string>());
  ELITPCServer server(config.at("server").at("address").get<std::string>(),
                      config.at("server").at("port").get<int>());

  if (config.contains("database")) {
    auto database = config.at("database");
    auto type = database.at("type").get<std::string>();
    if (type == "influxdb") {
      auto &db = server.addHistoryBackend<DCSHistoryBackendInflux>("default");
      db.setConnetionAddress(database.at("address").get<std::string>(),
                             database.at("port").get<int>());
      db.setDatabase(database.at("db").get<std::string>());
    }
  }
  server.addObject<DCSState>("state");
  for (auto &i : config.at("devices")) {
    auto type = i.at("type").get<std::string>();
    auto id = i.at("id").get<std::string>();
    server.addObject(type, id, i);
  }
  server.addObject("Dummy", "dummy");

  server.run();

  return 0;
}
