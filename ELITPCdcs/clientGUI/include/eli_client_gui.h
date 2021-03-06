#ifndef ELI_CLIENT_H
#define ELI_CLIENT_H
#include "opc_client_gui.h"
#include "open62541/types_dcsnodeset_generated.h"
#include "open62541/types_elitpcnodeset_generated.h"
class eli_client : public opc_client  // QThread
{
    Q_OBJECT
  public:
    eli_client(std::string address, int port);

  private:
    UA_DataTypeArray customDataTypesArray = {nullptr, UA_TYPES_DCSNODESET_COUNT,
                                             UA_TYPES_DCSNODESET};
    UA_DataTypeArray customDataTypesArray2 = {
        &customDataTypesArray, UA_TYPES_ELITPCNODESET_COUNT, UA_TYPES_ELITPCNODESET};
};

#endif  // ELI_CLIENT_H
