#include "DCSChecks.h"
#include "ProjectVersion.h"
#include "configloader.h"

int main(int argc, char **argv) {
    std::cout << "dcsCheck "
              << "v" << DCS_VERSION << std::endl;
    json config = ConfigLoader::getMasterConfig(argc, argv);
    int retv = 0;
    if(config.contains("server")) {
        auto server = config.at("server");
        retv |= checkOPC(server.at("address").get<std::string>(),
                         server.at("port").get<int>());
    } else {
        std::cout << "DCS OPC Server configuration not specified" << std::endl;
    }
    if(config.contains("database")) {
        auto server = config.at("database");
        if(server.at("type").get<std::string>() == "influxdb") {
            retv |= checkInfluxDb(server.at("address").get<std::string>(),
                                  server.at("port").get<int>());
        } else {
            std::cout << "Database configuration not specified" << std::endl;
        }
    }
    if(config.contains("grafana")) {
        auto server = config.at("grafana");
        retv |= checkGrafana(server.at("address").get<std::string>(),
                             server.at("port").get<int>());
    } else {
        std::cout << "Grafana configuration not specified" << std::endl;
    }
    return retv;
}
