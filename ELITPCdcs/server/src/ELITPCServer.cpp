#include "ELITPCServer.h"

ELITPCServer::ELITPCServer(std::string address, int port) : DCSServer(address, port) {
    addCustomTypes(&customDataTypesArray);
    namespace_dcsnodeset_generated(server);
}
