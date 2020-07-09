#ifndef TPG362_H
#define TPG362_H

#include "GenericDevice.h"

class TPG362 : public GenericDevice {
  public:
    TPG362();
    enum class CH { ALL, CH1, CH2 };
    enum class UNIT { mbar, torr, pascal, micron, hpascal, Volt };
    enum class FORMAT { floating, scientific };
    enum class STATUS { nochange, off, on };
    enum class SWITCHING_FUNCTION { F1 = 1, F2, F3, F4 };
    enum class SWITCHING_STATUS { off, on, CH1, CH2 };
    enum class FILTER { off, fast, normal, slow };
    // device specific commands

    std::string getIdentifier();
    std::string getGaugesData(CH ch = CH::ALL);
    std::string getGaugesIdentification();
    std::string getUnits();
    // returns a,b,c,d  -- 0 (OFF), 1 (ON) status
    std::string getSwitchingFunctionStatus();
    // returns a,x.xxxxEsxx,y.yyyyEsyy -- assignment, setpoint,hysteresis,
    std::string getSwitchingFunction(SWITCHING_FUNCTION f);
    std::string getFilter();

    std::string setFormat(FORMAT format);
    std::string setGaugesStatus(STATUS s1 = STATUS::nochange,
                                STATUS s2 = STATUS::nochange);
    std::string setUnits(UNIT unit);
    std::string setDisplayResolution(int r1 = 0, int r2 = 0);
    std::string setSwitchingFunction(SWITCHING_FUNCTION f, SWITCHING_STATUS s,
                                     double lowThreshold, double highThreshold);
    std::string setFilter(FILTER f1, FILTER f2);
    void Reset() {}

  private:
    //   std::string extractValue(std::string command){return "";}
    const std::string enq = std::string(1, 5);
    std::string sendWithEnquiry(std::string command);
};

#endif  // TPG362_H
