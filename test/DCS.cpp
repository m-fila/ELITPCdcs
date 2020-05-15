#include "DCSServer.h"
#include "DCSDeviceController.h"
#include "GenericDevice.h"
#include <chrono>
#include <iostream>
//void f(int x, int y, int n){std::this_thread::sleep_for(std::chrono::milliseconds(y));
//std::cout <<"Side "<<n<<": "<<x << std::endl;}
int main() {
  DCSServer s("localhost","8008");
    auto& o=s.addObject("wwwww");
    auto& v=o.addVariable("myV",UA_TYPES[UA_TYPES_INT32]);
    auto& oo=s.addController<DCSDeviceController<GenericDevice>>("m");
    oo.addMethod("wut","lolwut",{},{},[](const UA_Variant*,UA_Variant*){std::cout<<"aaaa"<<std::endl;});
   // v.addUpdate(100,[](){std::cout<<"!!!!!"<<std::endl;});
   s.run();
  return 0;
}