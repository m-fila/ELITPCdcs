/*
 * TCPConnector.cpp
 *
 *  Created on: 15 lut 2016
 *      Author: Marcin Zaremba
 */

#include "../../include/utl/TCPConnector.h"
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdexcept>

TCPConnector::TCPConnector() {}

TCPConnector::~TCPConnector() {}

TCPStream* TCPConnector::createConnection(const char* server, int port)
{
	struct sockaddr_in address;

	memset (&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	if (resolveHostName(server, &(address.sin_addr)) != 0) {
		inet_pton(PF_INET, server, &(address.sin_addr));
	}
	int sd = socket(AF_INET, SOCK_STREAM, 0);
	if (::connect(sd, (struct sockaddr*)&address, sizeof(address)) != 0) {
        throw std::runtime_error("Error while connecting to endpoint or device.");
	}
	return new TCPStream(sd, &address);
}

int TCPConnector::resolveHostName(const char* host, struct in_addr* addr)
{
	struct addrinfo *res;

	int result = getaddrinfo (host, NULL, NULL, &res);
	if (result == 0) {
		memcpy(addr, &((struct sockaddr_in *) res->ai_addr)->sin_addr, sizeof(struct in_addr));
		freeaddrinfo(res);
	}
	return result;
}
