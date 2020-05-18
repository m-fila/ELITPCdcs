#include "DCSServer.h"

DCSServer::DCSServer(std::string address, std::string port) {
  server = UA_Server_new();
  config = UA_Server_getConfig(server);
  UA_ServerConfig_setMinimal(config, std::stoi(port), nullptr);
  UA_String hostname = UA_STRING_ALLOC(address.c_str());
  UA_ServerConfig_setCustomHostname(config, hostname);
  UA_String_deleteMembers(&hostname);
  //  setDescription("DCS", "urn:DCS.server.application",
  //                 "http://fuw.edu.pl/~mfila/dcs");

  /*config->asyncOperationNotifyCallback = [](UA_Server *server) {
    std::cout << "!!!!!!!!" << std::endl;
    dispatcherThread.pushJob([server]() {
      const UA_AsyncOperationRequest *request = nullptr;
      void *context = nullptr;
      UA_AsyncOperationType type;
      while(UA_Server_getAsyncOperationNonBlocking(server, &type, &request,
                                                 &context, NULL) == false)
        {std::this_thread::sleep_for(std::chrono::milliseconds(100));}
        void *context2;
        UA_Server_getNodeContext(server, request->callMethodRequest.methodId,
                                 &context2);
        auto thread = static_cast<DCSWorkerThread *>(context2);
        // std::cout<<thread<<std::endl;
        thread->pushJob([context,request,server]() {
          UA_CallMethodResult response =
              UA_Server_call(server, &request->callMethodRequest);
          UA_Server_setAsyncOperationResult(
              server, (UA_AsyncOperationResponse *)&response, context);
          UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER,
                      "AsyncMethod_Testing: Call done: OKAY");
          UA_CallMethodResult_clear(&response);
        });

    });
  };*/
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
  UA_Server_delete(server);
}

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
