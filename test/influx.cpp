#include "Influx.h"
#include <iostream>
int main(int argc, char **argv) {
  TCPConnectionParameters p;
  p.IPaddress = "127.0.0.1";
  p.port = 8086;
  //   p.IPaddress="192.168.168.55";
  //   p.port=2244;
  // TestController t;
  // t.print(&p);

  Influx dev("test");
  dev.setConnectionParameters(p);
  //  auto r = dev.request(HTTPDevice::Method::GET,
  //                       "/query?db=test&q=SELECT+*+from+foo");
  // auto r = dev.request(HTTPDevice::Method::GET,
  //                      "/query?db=test&q=SELECT+*+from+foo");
  //  for (auto i : r.headers) {
  //    std::cout << i.first << "\t\t" << i.second << std::endl;
  //  }
  //  std::cout << "\n\n\n\n" << r.body << std::endl;
  dev.write("foo n=30");
  std::cout << dev.query("SELECT * from foo") << std::endl;
 // std::cout << dev.debugRequests() << std::endl;
  return 0;
}
