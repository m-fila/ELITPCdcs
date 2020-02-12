#include "eli_server.h"
ELIServer::ELIServer(std::string address, std::string port): OpcServer(address,port)
{
    addCustomTypes(&customDataTypesArray);
    namespace_dcsnodeset_generated(server);
}