#include "configloader.h"
#include "dt1415_variable.h"
#include "eli_client_db.h"
#include "hmp_variable.h"
#include "mks910_variable.h"
#include "piweather_variable.h"
#include "tpg362_variable.h"
#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <iomanip>
using json = nlohmann::json;
int main(int argc, char *argv[]) {

  json config = ConfigLoader::getMasterConfig(argc, argv);

  eli_client client(config.at("address").get<std::string>(),
                    config.at("port").get<int>());
  std::string databasePath;
  try {
    databasePath = config.at("databasePath").get<std::string>();
  } catch (nlohmann::detail::out_of_range &) {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss <<"./dcs"<<std::put_time(std::localtime(&in_time_t), "%Y-%m-%d-%H-%M-%S")<<".db";
    databasePath = ss.str();
    std::cout << "Missing databasePath configuration. Using " << databasePath<< std::endl;
  }
  client.open(databasePath);
  for (auto &i : config.at("devices")) {
    auto type = i.at("type").get<std::string>();
    auto id = i.at("id").get<std::string>();
    if (type == "HMP2020") {
      client.addVariable(new HMP2020Variable(id, "Measurements"));
    } else if (type == "HMP4040") {
      client.addVariable(new HMP4040Variable(id, "Measurements"));
    } else if (type == "DT1415ET") {
      client.addVariable(new dt1415_variable(id, "Measurements"));
    } else if (type == "TPG362") {
      client.addVariable(new TPG362Variable(id, "Measurements"));
    } else if (type == "PiWeather") {
      client.addVariable(new PiWeatherVariable(id, "Measurements"));
    } else if (type == "MKS910") {
      client.addVariable(new MKS910Variable(id, "Measurements"));
    } else {
      std::cout << "Unknown device:" << type << std::endl;
      continue;
    }
    client.addVariable(new status_variable(id, "Status"));
  }
  client.addVariable(new state_variable("MachineState", "State"));
  client.run();

  return 0;
}
