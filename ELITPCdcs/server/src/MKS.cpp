#include "MKS.h"
#include <sstream>

MKS::MKS()
    : GenericDevice(ConnectionType::TCP | ConnectionType::Serial,
                    ConnectionType::TCP),
      delay(200), channel(253) {}

std::string MKS::encapsulate(std::string content) {
  std::stringstream ss;
  ss << "@" << channel << content << ";FF";
  return ss.str();
}

std::string MKS::extract(std::string content) {
  const std::string beg("@" + std::to_string(channel));
  const std::string end(";FF");
  if (content.find(beg) != std::string::npos &&
      content.find(end) != std::string::npos) {
    std::string value = content.substr(
        beg.length() + 3, content.length() - beg.length() - 3 - end.length());
    if (content.find("ACK") != std::string::npos) {
      return value;
    } else {
      throw std::runtime_error("Encountered MKS error response");
    }
  } else {
    throw std::runtime_error("Encountered MKS broken response");
  }
}

std::string MKS::sendWithDelayedResponse(std::string command) {
  std::string s =
      GenericDevice::sendWithDelayedResponse(encapsulate(command), delay);
  return extract(s);
}

std::string MKS::getUnits() { return sendWithDelayedResponse("U?"); }
