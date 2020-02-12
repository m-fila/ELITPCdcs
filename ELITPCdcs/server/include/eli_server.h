#ifndef ELI_SERVER_H
#define ELI_SERVER_H
#include "opc_server.h"
#include "open62541/namespace_dcsnodeset_generated.h"


class ELIServer: public OpcServer
{
public:
    ELIServer(std::string address, std::string port);
private:
    UA_DataTypeArray customDataTypesArray={ nullptr, UA_TYPES_DCSNODESET_COUNT, UA_TYPES_DCSNODESET};
};

#endif // OPC_SERVER_H
