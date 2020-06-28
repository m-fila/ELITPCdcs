/*
 * TCPConnector.cpp
 *
 *  Created on: 15 lut 2016
 *      Author: Marcin Zaremba
 */

#include "TCPConnector.h"
#include <arpa/inet.h>
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
  if (resolveHostName(server, &(address.sin_addr)) != 0) {
    inet_pton(PF_INET, server, &(address.sin_addr));
  }
  int sd = socket(AF_INET, SOCK_STREAM, 0);
  // if (::connect(sd, (struct sockaddr *)&address, sizeof(address)) != 0) {
  if (connectWithTimeout(sd, &address, 2) != 0) {
    throw std::runtime_error("Error while connecting to endpoint or device.");
  }
  return new TCPStream(sd, &address);
}

int TCPConnector::resolveHostName(const char *host, struct in_addr *addr) {
  struct addrinfo *res;

  int result = getaddrinfo(host, NULL, NULL, &res);
  if (result == 0) {
    memcpy(addr, &((struct sockaddr_in *)res->ai_addr)->sin_addr,
           sizeof(struct in_addr));
    freeaddrinfo(res);
  } else {
    throw std::runtime_error("Error while resolving hostname." +
                             std::string(host));
  }
  return result;
}
#include <fcntl.h>
int TCPConnector::connectWithTimeout(int sd, sockaddr_in *address,
                                     uint timeout_s) {
  // Set non-blocking
  int arg;
  if ((arg = fcntl(sd, F_GETFL, NULL)) < 0) {
    //   fprintf(stderr, "Error fcntl(..., F_GETFL) (%s)\n", strerror(errno));
    return 1;
  }
  arg |= O_NONBLOCK;
  if (fcntl(sd, F_SETFL, arg) < 0) {
    // fprintf(stderr, "Error fcntl(..., F_SETFL) (%s)\n", strerror(errno));
    return 1;
  }
  auto rv = ::connect(sd, (struct sockaddr *)address, sizeof(*address));
  if (rv < 0) {
    if (errno == EINPROGRESS) {
      //     fprintf(stderr, "EINPROGRESS in connect() - selecting\n");
      do {
        timeval tv{timeout_s, 0};
        fd_set myset;
        FD_ZERO(&myset);
        FD_SET(sd, &myset);
        rv = select(sd + 1, NULL, &myset, NULL, &tv);
        if (rv < 0 && errno != EINTR) {
          //  fprintf(stderr, "Error connecting %d - %s\n", errno,
          //  strerror(errno));
          return 1;
        } else if (rv > 0) {
          // Socket selected for write
          int valopt;
          socklen_t optlen = sizeof(valopt);
          if (getsockopt(sd, SOL_SOCKET, SO_ERROR, (void *)(&valopt), &optlen) <
              0) {
            //     fprintf(stderr, "Error in getsockopt() %d - %s\n", errno,
            //     strerror(errno));
            return 1;
          }
          // Check the value returned...
          if (valopt) {
            //          fprintf(stderr, "Error in delayed connection() %d -
            //          %s\n", valopt, strerror(valopt));
            return 1;
          }
          break;
        } else {
          //         fprintf(stderr, "Timeout in select() - Cancelling!\n");
          return 1;
        }
      } while (1);
    } else {
      //   fprintf(stderr, "Error connecting %d - %s\n", errno,
      //   strerror(errno));
      return 1;
    }
  }
  // Set to blocking mode again...
  if ((arg = fcntl(sd, F_GETFL, NULL)) < 0) {
    // fprintf(stderr, "Error fcntl(..., F_GETFL) (%s)\n", strerror(errno));
    return 1;
  }
  arg &= (~O_NONBLOCK);
  if (fcntl(sd, F_SETFL, arg) < 0) {
    //  fprintf(stderr, "Error fcntl(..., F_SETFL) (%s)\n", strerror(errno));
    return 1;
  }
  return 0;
}
