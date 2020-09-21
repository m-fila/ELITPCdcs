#include "piweather_variable.h"
//#include "sstream"
PiWeatherVariable::PiWeatherVariable(std::string OName, std::string VName)
    : template_variable<UA_PiWeatherm>(OName, VName) {}

std::string PiWeatherVariable::translateKeys() {
    std::ostringstream oss;
    oss << "UA_timestamp INTEGER PRIMARY KEY";
    for(int i = 1; i != 4; i++) {
        oss << ", temp" << i << " REAL";
    }
    oss << ", temperature"
        << " REAL";
    oss << ", pressure"
        << " REAL";
    return oss.str();
}

std::ostream &operator<<(std::ostream &os, const UA_PiWeatherm &data) {
    int size = data.tempSize;
    for(int i = 0; i != size; ++i) {
        if(i) {
            os << ", ";
        }
        os << data.temp[i];
    }
    os << ", " << data.temperature;
    os << ", " << data.pressure;
    return os;
}
