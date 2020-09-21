#ifndef ELI_CLIENT_H
#define ELI_CLIENT_H

#include "opc_client_db.h"
#include "open62541/types_dcsnodeset_generated.h"

class eli_client : public opc_client_db {
  public:
    eli_client(std::string address, int port);

  private:
    UA_DataTypeArray customDataTypesArray = {nullptr, UA_TYPES_DCSNODESET_COUNT,
                                             UA_TYPES_DCSNODESET};
};

#endif  // ELI_CLIENT_H
