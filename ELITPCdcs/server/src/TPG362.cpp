#include "TPG362.h"
#include <sstream>

TPG362::TPG362() : DCSGenericDevice(ConnectionType::TCP, ConnectionType::TCP) {}

std::string TPG362::sendWithEnquiry(std::string command) {
    std::string r = sendWithDelayedResponse(command + "\r", 100);
    auto code = r.at(0);
    if(code == 6) {
        // 6->ACK
        r = sendWithDelayedResponse(enq, 100);
        if(r.back() == '\r') {
            r.erase(r.length() - 1);
        }
        return r;
    } else if(code == 21) {
        // 21->NACK
        throw std::runtime_error("TPG362 acknowledgement error. Received NACK");
    } else {
        throw std::runtime_error("TPG362 acknowledgement error.");
    }
}

std::string TPG362::getGaugesData(CH ch) {
    if(ch == CH::ALL) {
        return sendWithEnquiry("PRX");
    } else {
        return (sendWithEnquiry("PR" + std::to_string(static_cast<int>(ch))));
    }
}
std::string TPG362::getGaugesIdentification() { return sendWithEnquiry("TID"); }

std::string TPG362::getUnits() { return sendWithEnquiry("UNI"); }

std::string TPG362::getSwitchingFunctionStatus() { return sendWithEnquiry("SPS"); }

std::string TPG362::getSwitchingFunction(SWITCHING_FUNCTION f) {
    int F = static_cast<int>(f);
    return sendWithEnquiry("SP" + std::to_string(F));
}

std::string TPG362::getFilter() { return sendWithEnquiry("FIL"); }

std::string TPG362::setFormat(FORMAT format) {
    int f = static_cast<int>(format);
    return sendWithEnquiry("FMT," + std::to_string(f));
}

std::string TPG362::setGaugesStatus(STATUS s1, STATUS s2) {
    int S1 = static_cast<int>(s1);
    int S2 = static_cast<int>(s2);
    return sendWithEnquiry("SEN," + std::to_string(S1) + "," + std::to_string(S2));
}

std::string TPG362::setGaugeStatus(CH channel, STATUS status) {
    switch(channel) {
    case CH::ALL: {
        return setGaugesStatus(status, status);
    }
    case CH::CH1: {
        return setGaugesStatus(status, STATUS::nochange);
    }
    case CH::CH2: {
        return setGaugesStatus(STATUS::nochange, status);
    }
    default:
        break;
    }
    return "";
}

std::string TPG362::setUnits(UNIT unit) {
    int u = static_cast<int>(unit);
    return sendWithEnquiry("UNI," + std::to_string(u));
}

std::string TPG362::setDisplayResolution(int r1, int r2) {
    return sendWithEnquiry("DCD," + std::to_string(r1) + "," + std::to_string(r2));
}

std::string TPG362::setSwitchingFunction(SWITCHING_FUNCTION f, SWITCHING_STATUS s,
                                         double lowThreshold, double highThreshold) {

    auto function = static_cast<int>(f);
    auto status = static_cast<int>(s);
    std::stringstream ss;
    ss << "SP" << function << "," << status << "," << lowThreshold << ","
       << highThreshold;
    return sendWithEnquiry(ss.str());
}

std::string TPG362::setFilter(FILTER f1, FILTER f2) {
    int F1 = static_cast<int>(f1);
    int F2 = static_cast<int>(f2);
    return sendWithEnquiry("FIL," + std::to_string(F1) + "," + std::to_string(F2));
}
std::string TPG362::getTemperature() { return sendWithEnquiry("TMP"); }
std::string TPG362::getFirmwareVersion() { return sendWithEnquiry("PNR"); }
std::string TPG362::getHardwareVersion() { return sendWithEnquiry("HDW"); }

std::string TPG362::getModel() { return parseIdentifier(0); }
std::string TPG362::getSerialNumber() { return parseIdentifier(2); }
std::string TPG362::getPartNumber() { return parseIdentifier(1); }

std::string TPG362::parseIdentifier(size_t n) {
    std::stringstream ss(sendWithEnquiry("AYT"));
    std::string segment;
    for(size_t i = 0; i <= n; ++i) {
        std::getline(ss, segment, ',');
    }
    return segment;
}
