#include "DCSHMPController.h"
#include "DCSPiWeatherController.h"
#include "ELITPCServer.h"

int main() {
  ELITPCServer s("localhost", "8008");
  auto &o = s.addObject("wwwww");
  auto &v = o.addVariable("myV", UA_TYPES[UA_TYPES_INT32]);
  auto &o1 = s.addController<DCSDeviceController<GenericDevice>>("a");
  s.addController<DCSPiWeatherController>("Weather");
  s.addController<DCSHMP2020Controller>("LV");
  s.run();
  return 0;
}