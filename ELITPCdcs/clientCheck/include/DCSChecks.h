#ifndef DCS_CHECKS_H
#define DCS_CHECKS_H
#include "DCSCheckLogger.h"
#include "Influx.h"
#include "json.hpp"
#include <cstdarg>
#include <open62541/client_config_default.h>
#include <open62541/client_highlevel.h>
using json = nlohmann::json;
static constexpr const char *opcState[] = {
    "RUNNING",  "FAILED", "NOCONFIGURATION",    "SUSPENDED",
    "SHUTDOWN", "TEST",   "COMMUNICATIONFAULT", "UNKNOWN"};

static int checkOPC(std::string address, int port) {
    checkHelper("DCS OPC server", address, port);
    auto *client = UA_Client_new();
    auto *config = UA_Client_getConfig(client);
    config->logger.log = [](void *context, UA_LogLevel level, UA_LogCategory category,
                            const char *msg, va_list args) {};
    UA_ClientConfig_setDefault(config);

    std::string tcpAddress = "opc.tcp://" + address + ":" + std::to_string(port);

    UA_StatusCode retval = UA_Client_connect(client, tcpAddress.c_str());
    if(retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        logger(false);
        return 1;
    }
    logger(true);
    UA_Variant value;
    UA_Variant_init(&value);
    const UA_NodeId nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_STATE);
    retval = UA_Client_readValueAttribute(client, nodeId, &value);
    if(retval == UA_STATUSCODE_GOOD) {
        auto raw_state = *(UA_ServerState *)value.data;
        if(raw_state == 0) {
            logger(true, "Service state " + std::string(opcState[raw_state]));
        } else {
            logger(false, "Service state " + std::string(opcState[raw_state]));
            retval = UA_STATUSCODE_BADINVALIDSTATE;
        }
    } else {
        logger(false, "Service read error code " + std::to_string(retval));
    }

    UA_Variant_clear(&value);
    UA_Client_delete(client);
    return retval == UA_STATUSCODE_GOOD ? 0 : 1;
}

static int checkInfluxDb(std::string address, int port) {
    checkHelper("InfluxDb data base", address, port);
    Influx influx;
    influx.setConnectionParameters(address, port);
    HTTPDevice::HTTPResponse response;
    try {
        response = influx.health();
    } catch(std::runtime_error &e) {
        logger(false);
        return 1;
    }
    if(response.statusCode / 100 != 2) {
        logger(false, "Service communication error " + response.statusPhrase);
        return 1;
    }
    logger(true);
    try {
        json body = json::parse(response.body);
        auto status = body.at("status").get<std::string>();
        if(status != "pass") {
            logger(false, "Service state FAIL");
            return 1;
        }
        logger(true, "Service state PASS");
    } catch(json::exception &e) {
        logger(false, "Corrupted communication " + std::string(e.what()));
        return 1;
    }
    return 0;
}

static int checkGrafana(std::string address, int port) {
    checkHelper("Grafana dashboards", address, port);
    HTTPDevice grafana;
    grafana.setConnectionParameters(address, port);
    HTTPDevice::HTTPResponse response;
    try {
        // response = grafana.health();
        response = grafana.request(HTTPDevice::Method::GET, "/api/health");
    } catch(std::runtime_error &e) {
        logger(false);
        return 1;
    }
    logger(true);
    if(response.statusCode / 100 != 2) {
        logger(false, "Service communication error " + response.statusPhrase);
        return 1;
    }
    try {
        json body = json::parse(response.body);
        auto status = body.at("status").get<std::string>();
        if(status != "pass") {
            logger(false, "Service state FAIL");
            return 1;
        }
        logger(true, "Service state PASS");
    } catch(json::exception &e) {
        logger(false, "Corrupted communication " + std::string(e.what()));
        return 1;
    }
    return 0;
}
#endif  // DCS_CHECKS_H
