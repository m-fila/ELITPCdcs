#include "DeviceController.h"
#include "GenericDevice.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <vector>
class HTTPDevice : public GenericDevice {
public:
  enum class Method { GET, POST, PUT, HEAD, DELETE, PATCH, OPTIONS };

  struct HTTPResponse {
    std::map<std::string, std::string> headers;
    std::string body;
    HTTPResponse(std::string resp) {
      stringstream ss(resp);
      std::string s;
      if (!getline(ss, s)) {
        throw std::runtime_error("Parsing empty http response");
      }
      headers.insert({"status", s});
      while (std::getline(ss, s)) {
        // new line between headers and body
        if (s.size() < 2) {
          break;
        }
        auto ind = s.find(':');
        if (ind != std::string::npos) {
          headers.insert({s.substr(0, ind), s.erase(0, ind + 1)});
        }
      }
      getline(ss, body, '\0');
    }
  };

  HTTPDevice() : GenericDevice(ConnectionType::TCP, ConnectionType::TCP) {}
  void setConnectionParameters(const TCPConnectionParameters &p) {
    connectionParameters = p;
  }
  HTTPResponse request(Method method, std::string path,
                       const std::string &content = "",
                       const std::vector<std::string> &headers = {}) {
    connect();

    auto msg = methodToString.at(method);
    msg += " " + path + " HTTP/1.0\r\n";
    for (const auto &header : headers) {
      msg += header + "\r\n";
    }
    msg += "Host: " + connectionParameters.IPaddress + "\r\n";
    msg += "Content-Length: " + std::to_string(content.size()) + "\r\n\r\n" +
           content;
    std::cout << msg << std::endl;
    return HTTPResponse(sendWithResponse(msg));
  }

private:
  void connect() {
    setConnectionStream(TCPConnector::connect(
        connectionParameters.IPaddress.c_str(), connectionParameters.port));
  }

  const std::map<Method, std::string> methodToString = {
      {Method::GET, "GET"},        {Method::POST, "POST"},
      {Method::PUT, "PUT"},        {Method::HEAD, "HEAD"},
      {Method::DELETE, "DELETE"},  {Method::PATCH, "PATCH"},
      {Method::OPTIONS, "OPTIONS"}};
  TCPConnectionParameters connectionParameters;
};

class Influx : public HTTPDevice {
public:
  Influx(std::string database = "", std::string user = "",
         std::string password = "", std::string time = "ns")
      : HTTPDevice(), database(database), user(user), password(password),
        time(time) {}

  void write(std::string command) {
    request(HTTPDevice::Method::POST, "/write?" + auth() + "&precision=" + time,
            command);
  }

  std::string query(std::string q) {
    std::replace(q.begin(), q.end(), ' ', '+');
    return request(HTTPDevice::Method::GET,
                   "/query?" + auth() + "&epoch=" + time + "&q=" + q)
        .body;
  }
  int ping() {
    auto r = request(HTTPDevice::Method::GET, "/ping?verbose=true");
    int code;
    std::string http, msg;
    std::stringstream ss(r.headers.at("status"));
    ss >> http >> code >> msg;
    return code;
  }

  std::string debugVars() {
    return request(HTTPDevice::Method::GET, "/debug/vars").body;
  }

  std::string debugRequests(uint time_s = 10) {
    return request(HTTPDevice::Method::GET,
                   "/debug/requests?seconds=" + std::to_string(time_s))
        .body;
  }

  inline void setDatabase(std::string i) { database = i; }
  inline void setUser(std::string i) { user = i; }
  inline void setPassword(std::string i) { password = i; }
  inline void setTime(std::string i) { time = i; }

private:
  std::string database, user, password, time;
  std::string auth() {
    return "db=" + database + "&u=" + user + "&p=" + password;
  }
};
/*
class TestDevice : public GenericDevice {
public:
  TestDevice()
      : GenericDevice(ConnectionType::TCP, ConnectionType::TCP), database(""),
        user(""), password(""), time("ns") {}

  std::string query(std::string q, const std::string &db, const std::string &u,
                    const std::string &p, const std::string &epoch) {
    std::replace(q.begin(), q.end(), ' ', '+');
    std::stringstream ss;
    ss << "GET /query?" << pre(db, u, p) << "&epoch=" << epoch << "&q=" << q
       << " HTTP/1.0\r\n\r\n";
    std::cout << ss.str() << std::endl;
    return sendWithResponse(ss.str());
  }

  std::string query(std::string q) {
    return query(q, database, user, password, time);
  }

  std::string write(const std::string &content, const std::string &db,
                    const std::string &u, const std::string &p,
                    const std::string &precision) {
    std::stringstream ss;
    ss << "POST "
       << "/write?" << pre(db, u, p) << "&precision=" << precision
       << " HTTP/1.0\r\nContent-Length: " << content.length() << "\r\n\r\n"
       << content;
    return sendWithResponse(ss.str());
  }

  std::string write(std::string content) {
    return write(content, database, user, password, time);
  }
  inline void setDatabase(std::string i) { database = i; }
  inline void setUser(std::string i) { user = i; }
  inline void setPassword(std::string i) { password = i; }
  inline void setTime(std::string i) { time = i; }

private:
  std::string database, user, password, time;
  std::string pre(const std::string &db, const std::string &u = "",
                  const std::string &p = "") {
    std::stringstream ss;
    ss << "db=" << db << "&u=" << u << "&p=" << p;
    return ss.str();
  }
};

class TestController : public AbstractDeviceController<TestDevice> {
public:
  void print(TCPConnectionParameters *p) {
    device->setDatabase("test");

    std::string s;
    //   while(0){
    //   connect(p);
    //  std::string v="foo n="+to_string(rand()%100);
    //   s=device->sendWithResponse("POST http://localhost:8086/write?db=test
    //   HTTP/1.0\r\nContent-Length: "+to_string(v.length())+"\r\n\r\n"+v);
    //  // std::cout<<(s)<<std::endl;
    //   sleep(1);
    //   }
    //   std::string v="foo n="+9;
    //  std::cout<<"POST http://localhost:8086/write?db=test
    //  HTTP/1.0\r\nContent-Length:
    //  "+to_string(v.length())+"\r\n\r\n"+v<<std::endl;
    // std::cout<<"reconnect!!!!!!!!!!!"<<std::endl;
    // connect(p);
    connect(p);
    std::string v = "foo n=" + std::to_string(22);
    std::string w = "POST /write?db=test HTTP/1.0\r\nContent-Length:
8\r\n\r\nfoo n=22"; std::cout<<w<<std::endl; s = device->sendWithResponse(w);
    //std::cout << s << std::endl;
    // connect(p);
    //  s = device->write(v);
    //   std::cout << s << std::endl;
    // connect(p);
    // s = device->query("select * from foo");
    // s = device->sendWithResponse(
    //    "GET http://localhost:8086/query?db=test&q=SELECT+*+from+foo "
    //    "HTTP/1.0\r\n\r\n"); // std::cout<<(s)<<std::endl;
    //    std::cout<<"GET
    //    http://localhost:8086/query?db=test&q=SELECT+*+from+foo "
    //   "HTTP/1.0\r\n\r\n"<<std::endl;
    // std::cout << s << std::endl;
    // for (auto i : s)
  }
};
*/
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
  std::cout << dev.debugRequests() << std::endl;
  return 0;
}
