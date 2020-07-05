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

class TCPConnector {
public:
  TCPConnector();
  virtual ~TCPConnector();

  static TCPStream *connect(const char *server, int port) {
    return get().createConnection(server, port);
  }
  static TCPConnector &get() {
    static TCPConnector instance;
    return instance;
  }
  // singleton is needed because there is more than 1 function (like
  // resolveHostName) and we dont want to make them public "Factory" should
  //produce TCPStreams without instance creation

private:
  TCPStream *createConnection(const char *server, int port);
  int resolveHostName(const char *host, struct in_addr *addr);
  int connect(int sockno, struct sockaddr *addr, size_t addrlen,
                   timeval timeout);
};

#endif /* TCPCONNECTOR_H_ */
