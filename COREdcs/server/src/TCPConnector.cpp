/*
 * TCPConnector.cpp
 *
 *  Created on: 15 lut 2016
 *      Author: Marcin Zaremba
 */

#include "TCPConnector.h"
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdexcept>
#include <string.h>

TCPConnector::TCPConnector() {}

TCPConnector::~TCPConnector() {}

TCPStream *TCPConnector::createConnection(const char *server, int port) {
    struct sockaddr_in address;

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    if(resolveHostName(server, &(address.sin_addr)) != 0) {
        inet_pton(PF_INET, server, &(address.sin_addr));
    }
    int sd = socket(AF_INET, SOCK_STREAM, 0);
    // if (::connect(sd, (struct sockaddr *)&address, sizeof(address)) != 0) {
    if(connect(sd, (struct sockaddr *)&address, sizeof(address), {2, 0}) != 0) {
        close(sd);
        throw std::runtime_error("Error while connecting to endpoint or device.");
    }
    return new TCPStream(sd, &address);
}

int TCPConnector::resolveHostName(const char *host, struct in_addr *addr) {
    struct addrinfo *res;

    int result = getaddrinfo(host, NULL, NULL, &res);
    if(result == 0) {
        memcpy(addr, &((struct sockaddr_in *)res->ai_addr)->sin_addr,
               sizeof(struct in_addr));
        freeaddrinfo(res);
    } else {
        throw std::runtime_error("Error while resolving hostname." + std::string(host));
    }
    return result;
}

int TCPConnector::connect(int sockno, struct sockaddr *addr, size_t addrlen,
                          struct timeval timeout) {
    int res, opt;

    // get socket flags
    if((opt = fcntl(sockno, F_GETFL, NULL)) < 0) {
        return -1;
    }

    // set socket non-blocking
    if(fcntl(sockno, F_SETFL, opt | O_NONBLOCK) < 0) {
        return -1;
    }

    // try to connect
    if((res = ::connect(sockno, addr, addrlen)) < 0) {
        if(errno == EINPROGRESS) {
            fd_set wait_set;

            // make file descriptor set with socket
            FD_ZERO(&wait_set);
            FD_SET(sockno, &wait_set);

            // wait for socket to be writable; return after given timeout
            res = select(sockno + 1, NULL, &wait_set, NULL, &timeout);
        }
    }
    // connection was successful immediately
    else {
        res = 1;
    }

    // reset socket flags
    if(fcntl(sockno, F_SETFL, opt) < 0) {
        return -1;
    }

    // an error occured in connect or select
    if(res < 0) {
        return -1;
    }
    // select timed out
    else if(res == 0) {
        errno = ETIMEDOUT;
        return 1;
    }
    // almost finished...
    else {
        socklen_t len = sizeof(opt);

        // check for errors in socket layer
        if(getsockopt(sockno, SOL_SOCKET, SO_ERROR, &opt, &len) < 0) {
            return -1;
        }

        // there was an error
        if(opt) {
            errno = opt;
            return -1;
        }
    }
    struct timeval operationTimeout;
    operationTimeout.tv_sec = 10;
    operationTimeout.tv_usec = 0;

    if(setsockopt(sockno, SOL_SOCKET, SO_RCVTIMEO, (char *)&operationTimeout,
                  sizeof(operationTimeout)) < 0) {
    }

    if(setsockopt(sockno, SOL_SOCKET, SO_SNDTIMEO, (char *)&operationTimeout,
                  sizeof(operationTimeout)) < 0) {
    }

    return 0;
}
