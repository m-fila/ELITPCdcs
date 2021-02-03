/*
 * TCPConnector.h
 *
 *  Created on: 15 lut 2016
 *      Author: Marcin Zaremba
 */

#ifndef TCPCONNECTOR_H_
#define TCPCONNECTOR_H_

#include "TCPStream.h"
#include <netinet/in.h>
#include <string>

class TCPConnector {
  public:
    TCPConnector();
    virtual ~TCPConnector();

    static ConnectionStream *connect(const std::string &server, int port,
                                     bool secure = false) {
        std::string address;
        if(server.find("https://") == 0) {
            secure = true;
            address = server.substr(8);
        } else if(server.find("http://") == 0) {
            secure = false;
            address = server.substr(7);
        } else {
            address = server;
        }
        return get().createConnection(address.c_str(), port, secure);
    }
    static TCPConnector &get() {
        static TCPConnector instance;
        return instance;
    }
    // singleton is needed because there is more than 1 function (like
    // resolveHostName) and we dont want to make them public "Factory" should
    // produce TCPStreams without instance creation

  private:
    ConnectionStream *createConnection(const char *server, int port, bool secure = false);
    int resolveHostName(const char *host, struct in_addr *addr);
    int connect(int sockno, struct sockaddr *addr, size_t addrlen, timeval timeout);
};

#endif /* TCPCONNECTOR_H_ */
