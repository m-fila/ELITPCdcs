#ifndef TPG362_H
#define TPG362_H

#include "../utl/GenericDevice.h"

class TPG362 : public GenericDevice
{
public:
    TPG362();
    enum class CH {ALL, CH1, CH2};
    //device specific commands
    std::string getIdentifier();
    std::string getGaugesData(CH ch=CH::ALL);
    std::string getGaugesIdentification(){return "";}
    std::string setGaugesStatus(){return "";}
    void Reset(){}
private:
    std::string extractValue(std::string command){return "";}
    const std::string enq=std::string(1,5);
    std::string sendWithEqnuiry(std::string command);
};

#endif // TPG362_H
