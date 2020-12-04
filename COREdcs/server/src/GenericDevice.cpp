#include "GenericDevice.h"

GenericDevice::GenericDevice() {}

GenericDevice::~GenericDevice() {}

void GenericDevice::setConnectionStream(ConnectionStream *stream) {
    connectionStream.reset(stream);
    connected = true;
}

void GenericDevice::resetConnectionStream() {
    connectionStream.reset();
    connected = false;
}

void GenericDevice::sendCommand(std::string command) {
    if(isConnected()) {
        connectionStream->send(command);
    }
}

std::string GenericDevice::receiveResponse() {
    if(isConnected()) {
        return connectionStream->receive();
    } else {
        return "";
    }
}

std::string GenericDevice::sendWithResponse(std::string command) {
    if(isConnected()) {
        return connectionStream->sendWithResponse(command);
    } else {
        return "";
    }
}

std::string GenericDevice::sendWithDelayedResponse(std::string command, size_t delay) {
    if(isConnected()) {
        connectionStream->send(command);
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        return connectionStream->receive();
    } else {
        return "";
    }
}

void GenericDevice::validate(const std::string &response, const std::string &pattern) {
    std::regex rgex(pattern);
    if(!std::regex_match(response, std::regex(pattern))) {
        throw std::runtime_error(std::string("received invalid response: ") +
                                 response.c_str());
    }
}
