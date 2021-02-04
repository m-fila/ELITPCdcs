#include "MKS.h"
#include <sstream>

MKS::MKS()
    : DCSGenericDevice(ConnectionType::TCP | ConnectionType::Serial, ConnectionType::TCP),
      delay(200), address(253) {}

std::string MKS::encapsulate(std::string content) {
    std::stringstream ss;
    ss << "@" << address << content << ";FF";
    return ss.str();
}

std::string MKS::extract(std::string content) {
    const std::string beg("@" + std::to_string(address));
    const std::string end(";FF");
    if(content.find(beg) != std::string::npos && content.find(end) != std::string::npos) {
        std::string value = content.substr(
            beg.length() + 3, content.length() - beg.length() - 3 - end.length());
        if(content.find("ACK") != std::string::npos) {
            return value;
        } else {
            throw std::runtime_error("Encountered MKS error response: " + value);
        }
    } else {
        throw std::runtime_error("Encountered MKS broken response");
    }
}

std::string MKS::sendWithDelayedResponse(std::string command) {
    std::string s =
        DCSGenericDevice::sendWithDelayedResponse(encapsulate(command), delay);
    return extract(s);
}

std::string MKS::getUnits() { return sendWithDelayedResponse("U?"); }
