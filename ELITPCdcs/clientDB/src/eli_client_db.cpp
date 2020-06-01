#include "eli_client_db.h"
eli_client::eli_client(std::string address, int port): opc_client_db(address,port) {
    addCustomTypes(&customDataTypesArray);
}