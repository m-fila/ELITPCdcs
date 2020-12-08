#include "DCSVariable.h"
#include "DCSServer.h"
DCSVariable::DCSVariable(UA_Server *server, UA_NodeId parentNodeId,
                         const std::string &parentName, const std::string &variableName,
                         UA_DataType type)
    : server(server), parentName(parentName), variableName(variableName), dataType(type) {
    UA_VariableAttributes attr = UA_VariableAttributes_default;
    attr.description = UA_LOCALIZEDTEXT_ALLOC("en-Us", variableName.c_str());
    attr.displayName = UA_LOCALIZEDTEXT_ALLOC("en-Us", variableName.c_str());
    attr.dataType = dataType.typeId;
    attr.valueRank = UA_VALUERANK_SCALAR;
    UA_QualifiedName qName = UA_QUALIFIEDNAME_ALLOC(1, variableName.c_str());
    UA_Server_addVariableNode(server, UA_NODEID_NUMERIC(1, 0), parentNodeId,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES), qName,
                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), attr,
                              this, &variableNodeId);
    UA_VariableAttributes_deleteMembers(&attr);
    UA_QualifiedName_deleteMembers(&qName);
}

void DCSVariable::stopHistorizing() {
    UA_Server_writeHistorizing(server, variableNodeId, false);
}

void DCSVariable::setHistorizing(std::string backendName) {
    auto serv = DCSServer::getServerContext(server);
    auto backend = serv->getHistoryBackend(backendName);
    if(backend == nullptr) {
        UA_LOG_WARNING(DCSLogger::getLogger(), UA_LOGCATEGORY_USERLAND,
                       "Historizing backend %s not found", backendName.c_str());
        return;
    }
    UA_Server_writeHistorizing(server, variableNodeId, true);
    auto gathering = backend->getGathering();
    auto setting =
        gathering.getHistorizingSetting(server, gathering.context, &variableNodeId);
    if(setting) {
        return;
    }
    UA_HistorizingNodeIdSettings histoSetting;
    histoSetting.historizingBackend = backend->getUaBackend();
    histoSetting.maxHistoryDataResponseSize = 1;  // max size client can ask for
    histoSetting.historizingUpdateStrategy =
        UA_HISTORIZINGUPDATESTRATEGY_VALUESET;  // when value updated or polling
    gathering.registerNodeId(server, gathering.context, &variableNodeId, histoSetting);
}
