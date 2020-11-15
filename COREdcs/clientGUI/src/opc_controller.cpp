#include "opc_controller.h"
#include <iostream>
#include <open62541/client_highlevel_async.h>
opc_controller::opc_controller(std::string OName, QObject *parent)
    : opcQObject(OName, parent) {}

void opc_controller::opcInit(UA_Client *Client, UA_ClientConfig *Config,
                             UA_CreateSubscriptionResponse response) {
    opcQObject::opcInit(Client, Config, response);
    browseIds();
    addMonitoredItem(browsedIds[statusVariableName], response, StatusChangedCallback);
    addMonitoredItem(browsedIds[measurementsVariableName], response,
                     MeasurementsChangedCallback);
    addMonitoredItem(browsedIds[configurationVariableName], response,
                     ConfigurationChangedCallback);
    addMonitoredItem(browsedIds[enabledProfilesBrowseName], response,
                     EnabledProfilesChangedCallback);
    addMonitoredItem(browsedIds[activeProfileBrowseName], response,
                     ActiveProfileChangedCallback);
}

void opc_controller::StatusChangedCallback(UA_Client *client, UA_UInt32 subId,
                                           void *subContext, UA_UInt32 monId,
                                           void *monContext, UA_DataValue *value) {
    void *data = value->value.data;
    opc_controller *context = static_cast<opc_controller *>(monContext);
    emit context->statusChanged(data);
}

void opc_controller::MeasurementsChangedCallback(UA_Client *client, UA_UInt32 subId,
                                                 void *subContext, UA_UInt32 monId,
                                                 void *monContext, UA_DataValue *value) {
    void *data = value->value.data;
    opc_controller *context = static_cast<opc_controller *>(monContext);
    emit context->measurementsChanged(data);
}

void opc_controller::ConfigurationChangedCallback(UA_Client *client, UA_UInt32 subId,
                                                  void *subContext, UA_UInt32 monId,
                                                  void *monContext, UA_DataValue *value) {
    void *data = value->value.data;
    opc_controller *context = static_cast<opc_controller *>(monContext);
    emit context->configurationChanged(data);
}

void opc_controller::callConnect(std::string IPAddress, int port) {
    UA_Variant input[2];
    UA_Variant_init(input);
    UA_String address = UA_String_fromChars(IPAddress.c_str());
    UA_Variant_setScalarCopy(&input[0], &address, &UA_TYPES[UA_TYPES_STRING]);
    UA_Variant_setScalarCopy(&input[1], &port, &UA_TYPES[UA_TYPES_INT32]);
    //   UA_StatusCode retval= UA_Client_call(client, ObjectNodeId,
    //                               browsedIds[connectBrowseName], 2, input,
    //                               nullptr,nullptr);
    UA_Client_call_async(client, ObjectNodeId, browsedIds[connectBrowseName], 2, input,
                         nullptr, nullptr, nullptr);
    UA_Variant_clear(input);
}
void opc_controller::callDisconnect() {
    //  UA_StatusCode retval= UA_Client_call(client, ObjectNodeId,
    //                              browsedIds[disconnectBrowseName], 0, nullptr,
    //                              nullptr,nullptr);
    UA_Client_call_async(client, ObjectNodeId, browsedIds[disconnectBrowseName], 0,
                         nullptr, nullptr, nullptr, nullptr);
}

void opc_controller::callApplyProfile() {
    UA_Client_call_async(client, ObjectNodeId, browsedIds[applyProfileBrowseName], 0,
                         nullptr, nullptr, nullptr, nullptr);
}

void opc_controller::callSetProfile(std::string key) {
    UA_Variant input[1];
    UA_Variant_init(input);
    UA_String keyStr = UA_String_fromChars(key.c_str());
    UA_Variant_setScalarCopy(&input[0], &keyStr, &UA_TYPES[UA_TYPES_STRING]);
    UA_Client_call_async(client, ObjectNodeId, browsedIds[setProfileBrowseName], 1, input,
                         nullptr, nullptr, nullptr);
    UA_Variant_clear(input);
}

void opc_controller::callDumpProfile(std::string key) {
    UA_Variant input[1];
    UA_Variant_init(input);
    UA_String keyStr = UA_String_fromChars(key.c_str());
    UA_Variant_setScalarCopy(&input[0], &keyStr, &UA_TYPES[UA_TYPES_STRING]);
    UA_Client_call_async(client, ObjectNodeId, browsedIds[dumpProfileBrowseName], 1,
                         input, nullptr, nullptr, nullptr);
    UA_Variant_clear(input);
}

void opc_controller::ActiveProfileChangedCallback(UA_Client *client, UA_UInt32 subId,
                                                  void *subContext, UA_UInt32 monId,
                                                  void *monContext, UA_DataValue *value) {
    void *data = value->value.data;
    opc_controller *context = static_cast<opc_controller *>(monContext);
    emit context->activeProfileChanged(data);
}

void opc_controller::EnabledProfilesChangedCallback(UA_Client *client, UA_UInt32 subId,
                                                    void *subContext, UA_UInt32 monId,
                                                    void *monContext,
                                                    UA_DataValue *value) {
    void *data = value->value.data;
    opc_controller *context = static_cast<opc_controller *>(monContext);
    emit context->enabledProfilesChanged(data);
}
