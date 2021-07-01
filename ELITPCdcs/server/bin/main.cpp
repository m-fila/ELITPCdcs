#include "DCSArt.h"
#include "DCSHistoryBackendInflux.h"
#include "DCSLogger.h"
#include "DCSState.h"
#include "ELITPCServer.h"
#include "ProjectVersion.h"
#include "configloader.h"
#include <iostream>
using json = nlohmann::json;
int main(int argc, char *argv[]) {
    std::cout << DCSArt::ascii << "v" << DCS_VERSION << std::endl;
    json config = ConfigLoader::getMasterConfig(argc, argv);

    DCSLogger::setFile(config.at("logFile").get<std::string>());
    if(config.contains("logLevel")) {
        DCSLogger::setLogLevel(config.at("logLevel").get<std::string>(),
                               DCSLogger::Output::all);
    }
    if(config.contains("logLevelTTY")) {
        DCSLogger::setLogLevel(config.at("logLevelTTY").get<std::string>(),
                               DCSLogger::Output::tty);
    }
    if(config.contains("logLevelFile")) {
        DCSLogger::setLogLevel(config.at("logLevelFile").get<std::string>(),
                               DCSLogger::Output::file);
    }
    ELITPCServer server(config.at("server").at("address").get<std::string>(),
                        config.at("server").at("port").get<int>());

    if(config.contains("recoveryFile")) {
        server.setRecovery(config.at("recoveryFile").get<std::string>());
    } else {
        UA_LOG_DEBUG(DCSLogger::getLogger(), UA_LOGCATEGORY_SERVER,
                     "Not using recovery file");
    }

    if(config.contains("profileDir")) {
        server.setProfileDir(config.at("profileDir").get<std::string>());
    } else {
        UA_LOG_INFO(DCSLogger::getLogger(), UA_LOGCATEGORY_SERVER,
                    "Using default profile dir: %s", server.getProfileDir().c_str());
    }
    if(config.contains("database")) {
        auto database = config.at("database");
        auto type = database.at("type").get<std::string>();
        if(type == "influxdb") {
            auto &db = server.addHistoryBackend<DCSHistoryBackendInflux>("default");
            db.setConnetionAddress(database.at("address").get<std::string>(),
                                   database.at("port").get<int>());
            db.setDatabase(database.at("db").get<std::string>());
        }
    }
    server.addObject<DCSState>("state");
    for(auto &i : config.at("devices")) {
        auto type = i.at("type").get<std::string>();
        auto id = i.at("id").get<std::string>();
        server.addObject(type, id, i);
    }


    server.setDescription(config.at("server").value("id","dcs"), "urn:DCS.server.application", "http://fuw.edu.pl/~mfila/dcs");
    server.setBuildInfo("http://fuw.edu.pl/~mfila/dcs","ELITPC dcs server", "ELITPC", DCS_VERSION);

    server.run();

    return 0;
}
