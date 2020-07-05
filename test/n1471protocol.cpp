#include <iostream>
#include "DeviceController.h"
#include "GenericDevice.h"
#include <time.h>
#include <cstdlib>
#include <algorithm>
#include <sstream>
#include "N1471.h"

class TestController: public AbstractDeviceController<N1471> {
    public:

    void print(TCPConnectionParameters *p){
        connect(p);
        std::string s;
        s=device->getControlMode();std::cout<<s<<std::endl;
        device->setVoltageMax(N1471::CHANNEL::CH0,5000);
        s=device->getVoltageMax(N1471::CHANNEL::CH0);std::cout<<s<<std::endl;
        s=device->getStatus(N1471::CHANNEL::ALL);std::cout<<s<<std::endl;
        }
};

int main(int argc, char** argv){
    srand(time(NULL));
    TCPConnectionParameters p;
    p.IPaddress="192.168.168.103";
    p.port=5303;
    TestController t;
    t.print(&p);
    return 0;
}

