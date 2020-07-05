
#ifndef DCS_HTTP_DEVICE_H
#define DCS_HTTP_DEVICE_H

#include "GenericDevice.h"
#include <algorithm>
#include <map>
#include <sstream>
#include <stdexcept>
#include <vector>
#include "TCPConnector.h"
#include "ConnectionParameters.h"

class HTTPDevice : public GenericDevice {
public:
  enum class Method { GET, POST, PUT, HEAD, DELETE, PATCH, OPTIONS };

  struct HTTPResponse {
    std::map<std::string, std::string> headers;
    std::string body;
    HTTPResponse(std::string resp);
  };

  HTTPDevice();
  void setConnectionParameters(const TCPConnectionParameters &p);
  void setConnectionParameters(std::string address, int port);
  TCPConnectionParameters getConnectionParameters();
  HTTPResponse request(Method method, std::string path,
                       const std::string &content = "",
                       const std::vector<std::string> &headers = {});

private:
  void connect();

  const std::map<Method, std::string> methodToString = {
      {Method::GET, "GET"},        {Method::POST, "POST"},
      {Method::PUT, "PUT"},        {Method::HEAD, "HEAD"},
      {Method::DELETE, "DELETE"},  {Method::PATCH, "PATCH"},
      {Method::OPTIONS, "OPTIONS"}};
  TCPConnectionParameters connectionParameters;
};
#endif // DCS_HTTP_DEVICE_H