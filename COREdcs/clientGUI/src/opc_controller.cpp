#include "opc_controller.h"

opc_controller::opc_controller(std::string OName, QObject *parent)
    : opcQObject(OName, parent) {
    addMonitoredItem(statusVariableName);
    addMonitoredItem(connectionParametersVariableName);
    addMonitoredItem(measurementsVariableName);
    addMonitoredItem(configurationVariableName);
    addMonitoredItem(enabledProfilesBrowseName);
    addMonitoredItem(selectedProfileBrowseName);
    addMonitoredItem(deviceInfoBrowseName);
}

void opc_controller::callConnect() {
    UA_Client_call_async(client, ObjectNodeId, browsedIds[connectBrowseName], 0, nullptr,
                         nullptr, nullptr, nullptr);
}
void opc_controller::callDisconnect() {

    UA_Client_call_async(client, ObjectNodeId, browsedIds[disconnectBrowseName], 0,
                         nullptr, nullptr, nullptr, nullptr);
}
void opc_controller::callSetConnectionParameters(std::string IPAddress, int port) {
    UA_Variant input[2];
    UA_Variant_init(input);
    UA_String address = UA_String_fromChars(IPAddress.c_str());
    UA_Variant_setScalarCopy(&input[0], &address, &UA_TYPES[UA_TYPES_STRING]);
    UA_Variant_setScalarCopy(&input[1], &port, &UA_TYPES[UA_TYPES_INT32]);
    UA_Client_call_async(client, ObjectNodeId,
                         browsedIds[setConnectionParametersBrowseName], 2, input, nullptr,
                         nullptr, nullptr);
    UA_Variant_clear(input);
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

void opc_controller::callSaveProfile(std::string key) {
    UA_Variant input[1];
    UA_Variant_init(input);
    UA_String keyStr = UA_String_fromChars(key.c_str());
    UA_Variant_setScalarCopy(&input[0], &keyStr, &UA_TYPES[UA_TYPES_STRING]);
    UA_Client_call_async(client, ObjectNodeId, browsedIds[saveProfileBrowseName], 1,
                         input, nullptr, nullptr, nullptr);
    UA_Variant_clear(input);
}
