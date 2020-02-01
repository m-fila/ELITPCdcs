#include <iostream>
#include "DeviceController.h"
#include "GenericDevice.h"
class testController: public AbstractDeviceController<GenericDevice> {
    public:

    void print(TCPConnectionParameters *p){
        connect(p);
        std::string s;
        s=device->sendWithResponse("*IDN?\n");
        std::cout<<s<<std::endl;
        }
};

int main(){
    TCPConnectionParameters p;
    p.IPaddress="192.168.168.20";
    p.port=5025;
    testController t;
    t.print(&p);
    return 0;
}
