#include "DCSServer.h"

DCSServer::DCSServer(std::string address, int port) {
  server = UA_Server_new();
  config = UA_Server_getConfig(server);
  UA_ServerConfig_setMinimal(config, port, nullptr);
  UA_String hostname = UA_STRING_ALLOC(address.c_str());
  UA_ServerConfig_setCustomHostname(config, hostname);
  UA_String_deleteMembers(&hostname);
  //  setDescription("DCS", "urn:DCS.server.application",
  //                 "http://fuw.edu.pl/~mfila/dcs");
  auto retv =UA_Server_setNodeContext(server, UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER), this);
  if(retv!=UA_STATUSCODE_GOOD){
    throw std::runtime_error("Can't add context to server node");
  }
  addHistorizing();
}

void DCSServer::setDescription(std::string appName, std::string appURI,
                               std::string productURI) {
  UA_LocalizedText aName = UA_LOCALIZEDTEXT_ALLOC("en-Us", appName.c_str());
  UA_String aURI = UA_STRING_ALLOC(appURI.c_str());
  UA_String pURI = UA_STRING_ALLOC(productURI.c_str());

  config->applicationDescription.applicationName = aName;
  config->applicationDescription.applicationUri = aURI;
  config->applicationDescription.productUri = pURI;
  for (size_t i = 0; i != config->endpointsSize; ++i) {
    config->endpoints[i].server.applicationName = aName;
    config->endpoints[i].server.applicationUri = aURI;
    config->endpoints[i].server.productUri = pURI;
  }
}
DCSServer::~DCSServer() {
  for (auto i : objects) {
    delete i.second;
  }
  for (auto i : historyBackends) {
    delete i.second;
  }
  UA_Server_delete(server);
}

void DCSServer::addCustomTypes(UA_DataTypeArray *custom) {
  config->customDataTypes = custom;
}

void DCSServer::addHistorizing(){
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

DCSServer* DCSServer::getServerContext(UA_Server* server){
  void *context;
  auto retv = UA_Server_getNodeContext(server, UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER), &context);
	if (retv != UA_STATUSCODE_GOOD){
   throw std::runtime_error("Can't get server node context");
  }
  return static_cast<DCSServer*>(context);
}
