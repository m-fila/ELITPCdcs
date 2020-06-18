#ifndef DCS_INFLUX_H
#define DCS_INFLUX_H
#include "HTTPDevice.h"

class Influx : public HTTPDevice {
    public:
  Influx(std::string database = "", std::string user = "",
         std::string password = "", std::string time = "ms")
      : HTTPDevice(), database(database), user(user), password(password),
        time(time) {}

  inline void write(std::string command) {
    request(HTTPDevice::Method::POST, "/write?" + auth() + "&precision=" + time,
            command);
  }

  inline std::string query(std::string q) {
    std::replace(q.begin(), q.end(), ' ', '+');
    return request(HTTPDevice::Method::GET,
                   "/query?" + auth() + "&epoch=" + time + "&q=" + q)
        .body;
  }
  inline int ping() {
    auto r = request(HTTPDevice::Method::GET, "/ping?verbose=true");
    int code;
    std::string http, msg;
    std::stringstream ss(r.headers.at("status"));
    ss >> http >> code >> msg;
    return code;
  }

  inline std::string debugVars() {
    return request(HTTPDevice::Method::GET, "/debug/vars").body;
  }

  inline std::string debugRequests(uint time_s = 10) {
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
  inline std::string auth() {
    return "db=" + database + "&u=" + user + "&p=" + password;
  }
};
#endif // DCS_INFLUX_H