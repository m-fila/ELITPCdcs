#include "eli_client_gui.h"
// opc_client* opc_client::context=nullptr;

eli_client::eli_client(std::string address, int port) : opc_client(address, port) {
    addCustomTypes(&customDataTypesArray);
}
