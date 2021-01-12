#ifndef ELITPC_SERVER_H
#define ELITPC_SERVER_H
#include "DCSServer.h"
#include "open62541/namespace_elitpcnodeset_generated.h"

class ELITPCServer : public DCSServer {
  public:
    ELITPCServer(std::string address, int port);

  private:
    UA_DataTypeArray customDataTypesArray2 = {nullptr, UA_TYPES_ELITPCNODESET_COUNT,
                                              UA_TYPES_ELITPCNODESET};
    UA_DataTypeArray customDataTypesArray = {
        &customDataTypesArray2, UA_TYPES_DCSNODESET_COUNT, UA_TYPES_DCSNODESET};
};

#endif  // ELITPC_SERVER_H
