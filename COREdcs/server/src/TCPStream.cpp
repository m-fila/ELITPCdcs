/*
 * TCPStream.cpp
 *
 *  Created on: 15 lut 2016
 *      Author: Marcin Zaremba
 */

#include "TCPStream.h"
#include <arpa/inet.h>

TCPStream::TCPStream() {}

TCPStream::~TCPStream() {}

TCPStream::TCPStream(int sd, struct sockaddr_in *address) : ConnectionStream(sd) {
    char ip[50];
    inet_ntop(PF_INET, (struct in_addr *)&(address->sin_addr.s_addr), ip, sizeof(ip) - 1);
    m_peerIP = ip;
    m_peerPort = ntohs(address->sin_port);
}

std::string TCPStream::getPeerIP() { return m_peerIP; }

int TCPStream::getPeerPort() { return m_peerPort; }

TCPStream::TCPStream(const TCPStream &stream)
    : ConnectionStream(stream.m_sd), m_peerIP(stream.m_peerIP),
      m_peerPort(stream.m_peerPort) {}
