#include "HTTPDevice.h"
#include "SecureStream.h"
#include "TCPConnector.h"
#include "gtest/gtest.h"
#include <chrono>
TEST(SecureStream, objectCreation) {
    /*auto *stream = TCPConnector::connect("httpbin.org", 443, true);
    stream->send("GET https://httpbin.org/get? HTTP/1.0\r\n\r\n");
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << stream->receive() << std::endl;
     std::cout << TCPConnector::connect("https://10.15.4.67", 3000)
                         ->sendWithResponse(
                             "GET https://10.15.4.67:3000/api/health HTTP/1.0\r\n\r\n")
                  << std::endl;
    */
    HTTPDevice device;
    device.setConnectionParameters("https://httpbin.org", 443);
    auto r = device.request(HTTPDevice::Method::GET, "/get?q=123", "", {});
    for(auto i : r.headers) {
        std::cout << i.first << " " << i.second << std::endl;
    }
    std::cout << "content: " << r.body.size() << std::endl << r.body << std::endl;
}