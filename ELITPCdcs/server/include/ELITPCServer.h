#ifndef ELITPC_SERVER_H
#define ELITPC_SERVER_H
#include "DCSServer.h"
#include "open62541/namespace_elitpcnodeset_generated.h"

class ELITPCServer : public DCSServer {
  public:
    ELITPCServer(std::string address, int port);

  private:
    UA_DataTypeArray customDataTypesArray = {nullptr, UA_TYPES_ELITPCNODESET_COUNT,
                                             UA_TYPES_ELITPCNODESET};
};

#endif  // ELITPC_SERVER_H
