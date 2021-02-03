/*
 * TCPStream.h
 *
 *  Created on: 15 lut 2016
 *      Author: Marcin Zaremba
 */

#ifndef TCPSTREAM_H_
#define TCPSTREAM_H_

#include "ConnectionStream.h"
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

class TCPStream : public ConnectionStream {
    std::string m_peerIP;
    int m_peerPort;

  public:
    friend class TCPAcceptor;
    friend class TCPConnector;

    virtual ~TCPStream();

    std::string getPeerIP();
    int getPeerPort();

  private:
    TCPStream(int sd, struct sockaddr_in *address);
    TCPStream();
    TCPStream(const TCPStream &stream);
};

#endif /* TCPSTREAM_H_ */
