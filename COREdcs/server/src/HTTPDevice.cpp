#include "HTTPDevice.h"
HTTPDevice::HTTPResponse::HTTPResponse(std::string resp) {
    std::stringstream ss(resp);
    std::string s;
    if(!getline(ss, s)) {
        throw std::runtime_error("Parsing empty http response");
    } else {
        std::stringstream statusStream(s);
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

HTTPDevice::HTTPDevice() : DCSBaseDevice(ConnectionType::TCP, ConnectionType::TCP) {}

void HTTPDevice::setConnectionParameters(const TCPConnectionParameters &p) {
    connectionParameters = p;
    auto i = p.IPaddress.find("://");
    if(i != std::string::npos) {
        hostname = p.IPaddress.substr(i + 3);
    }
}

void HTTPDevice::setConnectionParameters(std::string address, int port) {
    TCPConnectionParameters p;
    p.IPaddress = address;
    p.port = port;
    return setConnectionParameters(p);
}

TCPConnectionParameters HTTPDevice::getConnectionParameters() {
    return connectionParameters;
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
    msg += "Host: " + hostname + "\r\n";
    msg += "Content-Length: " + std::to_string(content.size()) + "\r\n\r\n" + content;

    auto r = HTTPResponse(sendWithResponse(msg));
    auto i = r.headers.find("Content-Length");
    if(i != r.headers.end()) {
        size_t h = std::stoul(i->second);
        if(r.body.size() + 1 < h) {  // ConnectionStream strips one \n // TO BE FIXED
            r.body += receiveResponse();
        }
    }
    return r;
}

void HTTPDevice::connect() {
    setConnectionStream(
        TCPConnector::connect(connectionParameters.IPaddress, connectionParameters.port));
}
