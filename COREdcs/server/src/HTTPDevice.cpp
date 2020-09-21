#include "HTTPDevice.h"
HTTPDevice::HTTPResponse::HTTPResponse(std::string resp) {
    stringstream ss(resp);
    std::string s;
    if(!getline(ss, s)) {
        throw std::runtime_error("Parsing empty http response");
    } else {
        stringstream statusStream(s);
        statusStream >> httpVersion >> statusCode >> std::ws;
        getline(statusStream, statusPhrase);
    }
    while(std::getline(ss, s)) {
        // new line between headers and body
        if(s.size() < 2) {
            break;
        }
        auto ind = s.find(':');
        if(ind != std::string::npos) {
            headers.insert({s.substr(0, ind), s.erase(0, ind + 1)});
        }
    }
    getline(ss, body, '\0');
}

HTTPDevice::HTTPDevice() : GenericDevice(ConnectionType::TCP, ConnectionType::TCP) {}

void HTTPDevice::setConnectionParameters(const TCPConnectionParameters &p) {
    connectionParameters = p;
}
TCPConnectionParameters HTTPDevice::getConnectionParameters() {
    return connectionParameters;
}

void HTTPDevice::setConnectionParameters(std::string address, int port) {
    connectionParameters.IPaddress = address;
    connectionParameters.port = port;
}

HTTPDevice::HTTPResponse HTTPDevice::request(Method method, std::string path,
                                             const std::string &content,
                                             const std::vector<std::string> &headers) {
    connect();

    auto msg = methodToString.at(method);
    msg += " " + path + " HTTP/1.0\r\n";
    for(const auto &header : headers) {
        msg += header + "\r\n";
    }
    msg += "Host: " + connectionParameters.IPaddress + "\r\n";
    msg += "Content-Length: " + std::to_string(content.size()) + "\r\n\r\n" + content;
    return HTTPResponse(sendWithDelayedResponse(msg, 1));
}

void HTTPDevice::connect() {
    setConnectionStream(TCPConnector::connect(connectionParameters.IPaddress.c_str(),
                                              connectionParameters.port));
}
