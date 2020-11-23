
#ifndef DCS_HTTP_DEVICE_H
#define DCS_HTTP_DEVICE_H

#include "ConnectionParameters.h"
#include "GenericDevice.h"
#include "TCPConnector.h"
#include <algorithm>
#include <map>
#include <sstream>
#include <stdexcept>
#include <vector>

class HTTPDevice : public GenericDevice {
  public:
    enum class Method { GET, POST, PUT, HEAD, DELETE, PATCH, OPTIONS };

    /**
     * @brief HTTP response:
     *  HTTP version, status code, phrase
     *  header: header info
     *  response body
     */
    struct HTTPResponse {
        /**
         * @brief Construct a new HTTPResponse object
         *
         * @param resp HTTP string to be parsed
         */
        HTTPResponse(std::string resp);
        HTTPResponse() = default;
        HTTPResponse(const HTTPResponse &response) = default;
        /**
         * @brief map of response headers
         * eg. Date, Server, Content-Length, Content-Type, ...
         */
        std::map<std::string, std::string> headers;
        /**
         * @brief response content
         *
         */
        std::string body;
        /**
         * @brief HTTP version
         * HTTP/1.0 or HTTP/1.1
         *
         */
        std::string httpVersion;
        /**
         * @brief HTTP response status code
         * 1xx information
         * 2xx ok
         * 3xx redirection
         * 4xx client error
         * 5xx server error
         *
         */
        int statusCode = 0;
        /**
         * @brief status information
         *
         */
        std::string statusPhrase;
    };

    HTTPDevice();
    void setConnectionParameters(const TCPConnectionParameters &p);
    void setConnectionParameters(std::string address, int port);
    TCPConnectionParameters getConnectionParameters();
    HTTPResponse request(Method method, std::string path, const std::string &content = "",
                         const std::vector<std::string> &headers = {});

  private:
    void connect();

    const std::map<Method, std::string> methodToString = {
        {Method::GET, "GET"},        {Method::POST, "POST"},     {Method::PUT, "PUT"},
        {Method::HEAD, "HEAD"},      {Method::DELETE, "DELETE"}, {Method::PATCH, "PATCH"},
        {Method::OPTIONS, "OPTIONS"}};
    TCPConnectionParameters connectionParameters;
};
#endif  // DCS_HTTP_DEVICE_H
