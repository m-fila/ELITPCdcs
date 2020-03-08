#ifndef mks946_H
#define mks946_H

#include "MKS.h"
#include "mks946codes.h"

class MKS946 : public MKS
{
public:
    MKS946();
    //channel enum 1-6
    enum class CH {CH1, CH2, CH3,CH4,CH5,CH6};
    enum class PID {R1, R2, R3,R4,R5,R6,R7,R8};
    enum class RATIO {R1, R2, R3,R4};
    //device specific commands

    //pressure commands
    std::string getPressure(CH ch);
    std::string getPressureAll();
    //flow commands
    std::string getFlow(CH ch);
    std::string getFlowFactor(CH ch);
    void setFlowFactor(CH ch,double factor);
    std::string getFlowNominalRange(CH ch);
    void setFlowNomialRange(CH ch,double range);
    void zeroMFC(CH ch);
    std::string getFlowPoint(CH ch);
    void setFlowPoint(CH ch,double range);
    std::string getFlowMode(CH ch);
    void setFlowMode(CH ch,double range);
    //pid recipe

    //ratio recipe
    std::string getRatioRecipe();
    void setRatioRecipe(RATIO recipe);
    //pid/ratio commands
    std::string getPIDRecipe();
    void setPIDRecipe(PID recipeeci);


    //unit commands
    std::string getFirmware();
    std::string getHardware();
    std::string getName();
    std::string getModel();
    std::string getSerialNumber();
    std::string getPartNumer();
    std::string getUptime();
    // TORR, PASCAL, MBAR
    void setUnits(MKS946codes::Units u);
};

#endif // mks946_H
