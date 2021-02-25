#ifndef DCS_RECOVERY_H
#define DCS_RECOVERY_H
#include "ConnectionParameters.h"
#include "DCSLogger.h"
#include "json.hpp"
#include <fstream>
#include <iomanip>
#include <map>
class DCSRecovery {
  public:
    DCSRecovery(const std::string &fileName) : fileName(fileName) {}
    ~DCSRecovery() {
        std::lock_guard<std::mutex> lock(mutex);
        std::ofstream file(fileName);
        auto json = nlohmann::json::object();
        file << json;
        file.close();
    }

    TCPConnectionParameters get(const std::string &name) {
        std::lock_guard<std::mutex> lock(mutex);
        auto json = parseFile();
        auto object = json.find(name);
        if(object == json.end()) {
            throw std::range_error("Can't find recovery for " + name);
        }
        if(object->contains("address") && object->contains("port")) {
            TCPConnectionParameters parameters;
            parameters.IPaddress = object->at("address").get<std::string>();
            parameters.port = object->at("port").get<int>();
            return parameters;
        }
        throw std::runtime_error("Invalid recovery entry for " + name);
    }

    void enable(const std::string &deviceName, std::string address, int port) {
        std::lock_guard<std::mutex> lock(mutex);
        auto j = parseFile();
        auto object = j.find(deviceName);
        if(object != j.end()) {
            j.erase(object);
        }
        j[deviceName]["address"] = address;
        j[deviceName]["port"] = port;
        std::ofstream file(fileName);
        if(file) {
            file << std::setw(4) << j;
        } else {
            UA_LOG_WARNING(DCSLogger::getLogger(), UA_LOGCATEGORY_SERVER,
                           "Can't write recovery file %s", fileName.c_str());
        }
        file.close();
    }

    void disable(const std::string &deviceName) {
        std::lock_guard<std::mutex> lock(mutex);
        auto j = parseFile();
        auto object = j.find(deviceName);
        if(object != j.end()) {
            j.erase(object);
            std::ofstream file(fileName);
            if(file) {
                file << std::setw(4) << j;
            } else {
                UA_LOG_WARNING(DCSLogger::getLogger(), UA_LOGCATEGORY_SERVER,
                               "Can't write recovery file %s", fileName.c_str());
            }
            file.close();
        }
    }

  private:
    nlohmann::json parseFile() const {
        std::ifstream file(fileName);
        nlohmann::json j = {};
        if(!file) {
            UA_LOG_WARNING(DCSLogger::getLogger(), UA_LOGCATEGORY_SERVER,
                           "Can't read recovery file %s. Using empty recovery list.",
                           fileName.c_str());
        } else {
            try {
                file >> j;
            } catch(nlohmann::json::exception &e) {
                UA_LOG_WARNING(DCSLogger::getLogger(), UA_LOGCATEGORY_SERVER,
                               "Can't parse recovery file %s", fileName.c_str());
                j = {};
            }
        }
        file.close();
        return j;
    }

    std::mutex mutex;
    const std::string fileName;
};
#endif  // DCS_RECOVERY_H
