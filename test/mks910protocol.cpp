#include <iostream>
#include "DeviceController.h"
#include "GenericDevice.h"
#include <time.h>
#include <cstdlib>
#include <algorithm>
#include <sstream>
#include "MKS910.h"

class TestController: public AbstractDeviceController<MKS910> {
    public:

    void print(TCPConnectionParameters *p){
        connect(p);
        std::string s;
        //s=device->sendWithDelayedResponse("@253RSD?;FF",100);
        s=device->getFirmware();std::cout<<s.c_str()<<std::endl;
        s=device->getHardware();std::cout<<s.c_str()<<std::endl;
        s=device->getModel();std::cout<<s.c_str()<<std::endl;
        s=device->getPartNumer();std::cout<<s.c_str()<<std::endl;
        s=device->getPCombined();std::cout<<s.c_str()<<std::endl;
        s=device->getPCombinedLong();std::cout<<s.c_str()<<std::endl;
        s=device->getPDiff();std::cout<<s.c_str()<<std::endl;
        s=device->getPPiezo();std::cout<<s.c_str()<<std::endl;
        s=device->getPPirani();std::cout<<s.c_str()<<std::endl;
        s=device->getSerialNumber();std::cout<<s.c_str()<<std::endl;
        s=device->getStatus();std::cout<<s.c_str()<<std::endl;
        s=device->getTemp();std::cout<<s.c_str()<<std::endl;
        device->setUnits(MKS910codes::Units::TORR);
        s=device->getUnits();std::cout<<s.c_str()<<std::endl;
        s=device->getUptime();std::cout<<s.c_str()<<std::endl;
        device->setUnits(MKS910codes::Units::TORR);
        s=device->getUnits();std::cout<<s.c_str()<<std::endl;
        s=device->getPCombinedLong();std::cout<<s.c_str()<<std::endl;
        device->setUnits(MKS910codes::Units::TORR);
        s=device->getUnits();std::cout<<s.c_str()<<std::endl;
        s=device->getPCombinedLong();std::cout<<s.c_str()<<std::endl;
       // for (auto &i :s){
            
       // }
        }
};

int main(int argc, char** argv){
    srand(time(NULL));
    TCPConnectionParameters p;
    p.IPaddress="192.168.168.102";
    p.port=5300;
 //   p.IPaddress="192.168.168.55";
 //   p.port=2244;
    TestController t;
    t.print(&p);
    return 0;
}

