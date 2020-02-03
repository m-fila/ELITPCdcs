#include <iostream>
#include "DeviceController.h"
#include "GenericDevice.h"
#include <time.h>
class TestDevice : public GenericDevice{
    public:
    TestDevice():GenericDevice(ConnectionType::TCP,ConnectionType::Telnet){
    }
};

class TestController: public AbstractDeviceController<TestDevice> {
    public:

    void print(TCPConnectionParameters *p){
        connect(p);
        std::string s;
       // s=device->receive()
        device->sendWithResponse("apc");//std::cout<<(s)<<std::endl;

        //for (auto i : s)
           // std::cout<<(s)<<std::endl;
        }
};

int main(int argc, char** argv){
    TCPConnectionParameters p;
    p.IPaddress="192.168.168.222";
    p.port=23;
 //   p.IPaddress="192.168.168.55";
 //   p.port=2244;
    TestController t;
    t.print(&p);
    return 0;
}
