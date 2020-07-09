#ifndef CONNECTIONPARAMETERS_H
#define CONNECTIONPARAMETERS_H

#include <string>

struct ConnectionParameters {};

struct TCPConnectionParameters : ConnectionParameters {
    std::string IPaddress;
    int port;
};

#endif  // CONNECTIONPARAMETERS_H
