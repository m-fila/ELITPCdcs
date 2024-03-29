#include "DCSServer.h"

DCSServer::DCSServer(std::string address, int port) {
    auto envProfile = getenv("DCS_PROFILE_DIR");
    profileDir =
        envProfile != nullptr ? envProfile : std::string(getenv("HOME")) + "/.dcs/";
    if(profileDir.back() != '/') {
        profileDir.push_back('/');
    }
    server = UA_Server_new();
    config = UA_Server_getConfig(server);
    config->logger = *DCSLogger::getLogger();
    UA_ServerConfig_setMinimal(config, port, nullptr);
    UA_String hostname = UA_STRING_ALLOC(address.c_str());
    UA_ServerConfig_setCustomHostname(config, hostname);
    UA_String_deleteMembers(&hostname);

    auto retv =
        UA_Server_setNodeContext(server, UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER), this);
    if(retv != UA_STATUSCODE_GOOD) {
        throw std::runtime_error("Can't add context to server node");
    }
    config->asyncOperationNotifyCallback = asyncCallback;
    namespace_dcsnodeset_generated(server);
    addHistorizing();
    auto objectTypeId = addObjectType("DCSObject");
    addObjectType("DCSState", objectTypeId);
    for(auto i : DCSObjectFactory::allocators()) {
        addObjectType(i.first, objectTypeId);
    }
}

void DCSServer::setDescription(std::string appName, std::string appURI,
                               std::string productURI) {

    config->applicationDescription.applicationName =
        UA_LOCALIZEDTEXT_ALLOC("en-Us", appName.c_str());
    config->applicationDescription.applicationUri = UA_STRING_ALLOC(appURI.c_str());
    config->applicationDescription.productUri = UA_STRING_ALLOC(productURI.c_str());
    for(size_t i = 0; i != config->endpointsSize; ++i) {
        config->endpoints[i].server.applicationName =
            UA_LOCALIZEDTEXT_ALLOC("en-Us", appName.c_str());
        config->endpoints[i].server.applicationUri = UA_STRING_ALLOC(appURI.c_str());
        config->endpoints[i].server.productUri = UA_STRING_ALLOC(productURI.c_str());
    }
}

void DCSServer::setBuildInfo(std::string productUri, std::string productName,
                             std::string manufacturerName, std::string softwareVersion) {
    config->buildInfo.productUri = UA_STRING_ALLOC(productUri.c_str());
    config->buildInfo.productName = UA_STRING_ALLOC(productName.c_str());
    config->buildInfo.manufacturerName = UA_STRING_ALLOC(manufacturerName.c_str());
    config->buildInfo.softwareVersion = UA_STRING_ALLOC(softwareVersion.c_str());
}

DCSServer::~DCSServer() {
    for(auto i : objects) {
        delete i.second;
    }
    for(auto i : historyBackends) {
        delete i.second;
    }
    UA_Server_delete(server);
}

void DCSServer::addCustomTypes(UA_DataTypeArray *custom) {
    config->customDataTypes = custom;
}

void DCSServer::addHistorizing() {
    UA_HistoryDataGathering gathering = UA_HistoryDataGathering_Default(1000);
    config->historyDatabase = UA_HistoryDatabase_default(gathering);
}

UA_Boolean DCSServer::running = false;

int DCSServer::run() {
    signal(SIGINT, stopHandler);
    signal(SIGTERM, stopHandler);
    running = true;
    UA_Server_run(server, &running);
    signal(SIGINT, SIG_DFL);
    signal(SIGTERM, SIG_DFL);
    return EXIT_SUCCESS;
}

void DCSServer::asyncCallback(UA_Server *server) {
    auto dcsServer = getServerContext(server);
    dcsServer->dispatcherThread.push_back([server]() {
        const UA_AsyncOperationRequest *request = nullptr;
        void *context = nullptr;
        UA_AsyncOperationType type;
        while(UA_Server_getAsyncOperationNonBlocking(server, &type, &request, &context,
                                                     NULL) == false) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        void *methodContext;
        UA_Server_getNodeContext(server, request->callMethodRequest.methodId,
                                 &methodContext);
        auto thread = static_cast<DCSWorkerThread *>(
            static_cast<DCSObject::Method *>(methodContext)->context);
        thread->push_front([context, request, server]() {
            UA_CallMethodResult response =
                UA_Server_call(server, &request->callMethodRequest);
            UA_Server_setAsyncOperationResult(
                server, (UA_AsyncOperationResponse *)&response, context);
            UA_CallMethodResult_clear(&response);
        });
    });
}

UA_NodeId DCSServer::addObjectType(std::string typeName, UA_NodeId parentTypeId) {
    UA_NodeId objectTypeId = UA_NODEID_STRING_ALLOC(1, typeName.c_str());
    UA_QualifiedName qName = UA_QUALIFIEDNAME_ALLOC(1, typeName.c_str());
    UA_ObjectTypeAttributes dtAttr = UA_ObjectTypeAttributes_default;
    dtAttr.displayName = UA_LOCALIZEDTEXT_ALLOC("en-US", typeName.c_str());
    dtAttr.description = UA_LOCALIZEDTEXT_ALLOC("en-US", typeName.c_str());

    UA_NodeId resultId;
    UA_Server_addObjectTypeNode(server, objectTypeId, parentTypeId,
                                UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE), qName, dtAttr,
                                nullptr, &resultId);

    UA_NodeId_deleteMembers(&objectTypeId);
    UA_QualifiedName_deleteMembers(&qName);
    UA_ObjectTypeAttributes_deleteMembers(&dtAttr);

    return resultId;
    ;
}

void DCSServer::stopHandler(int sig) {
    UA_LOG_INFO(DCSLogger::getLogger(), UA_LOGCATEGORY_USERLAND, "Received ctrl-c");
    running = false;
}

DCSObject *DCSServer::addObject(std::string typeName, std::string name,
                                const Options &options) {
    if(objects.find(name) == objects.end()) {
        auto newObject = DCSObjectFactory::create(typeName, name, server, options);
        // auto newObject = DCSObjectFactory::create(typeName, name, server);
        if(newObject != nullptr) {
            objects[name] = newObject;
        } else {
            UA_LOG_ERROR(DCSLogger::getLogger(), UA_LOGCATEGORY_SERVER,
                         "Skipping unknown type %s.", typeName.c_str());
        }
        return newObject;
    } else {
        throw std::runtime_error("Not unique controller id: " + name);
    }
}
