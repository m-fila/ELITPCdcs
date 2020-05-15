#include "DCSServer.h"

DCSServer::DCSServer(std::string address, std::string port) {
  server = UA_Server_new();
  config = UA_Server_getConfig(server);
  UA_ServerConfig_setMinimal(config, std::stoi(port), nullptr);
  UA_String hostname = UA_STRING_ALLOC(address.c_str());
  UA_ServerConfig_setCustomHostname(config, hostname);
  UA_String_deleteMembers(&hostname);
  setDescription("DCS","urn:DCS.server.application","http://fuw.edu.pl/~mfila/dcs");
}


void DCSServer::setDescription(std::string  appName, std::string appURI, std::string productURI){
  UA_LocalizedText aName=UA_LOCALIZEDTEXT_ALLOC("en-Us",appName.c_str());
  UA_String aURI=UA_STRING_ALLOC(appURI.c_str());
  UA_String pURI=UA_STRING_ALLOC(productURI.c_str());
  UA_LocalizedText_copy(&aName,&config->applicationDescription.applicationName);
  UA_String_copy(&aURI,&config->applicationDescription.applicationUri);
  UA_String_copy(&pURI,&config->applicationDescription.productUri);
  for(size_t i=0; i!=config->endpointsSize;++i){
    config->endpoints[i].server.applicationName=aName;
    config->endpoints[i].server.applicationUri=aURI;
    config->endpoints[i].server.productUri=pURI;
  }
}
DCSServer::~DCSServer() { UA_Server_delete(server); }

void DCSServer::addCustomTypes(UA_DataTypeArray *custom) {
  config->customDataTypes = custom;
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
