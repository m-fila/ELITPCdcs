#ifndef DCS_INFLUX_H
#define DCS_INFLUX_H
#include "HTTPDevice.h"
class Influx : public HTTPDevice {
  public:
    Influx(std::string database = "", std::string user = "", std::string password = "",
           std::string time = "ms")
        : HTTPDevice(), database(database), user(user), password(password), time(time) {}

    inline HTTPResponse write(const std::string &command) {
        return request(HTTPDevice::Method::POST,
                       "/write?" + auth() + "&precision=" + time, command);
    }

    inline HTTPResponse query(std::string q) {
        std::replace(q.begin(), q.end(), ' ', '+');
        return request(HTTPDevice::Method::GET,
                       "/query?" + auth() + "&epoch=" + time + "&q=" + q);
    }
    inline HTTPResponse ping() {
        return request(HTTPDevice::Method::GET, "/ping?verbose=true");
    }

    inline HTTPResponse debugVars() {
        return request(HTTPDevice::Method::GET, "/debug/vars");
    }

    inline HTTPResponse debugRequests(size_t time_s = 10) {
        return request(HTTPDevice::Method::GET,
                       "/debug/requests?seconds=" + std::to_string(time_s));
    }
    /**
     * @brief InfluxDB v 2.0 API only.
     * Checks health of instance
     *
     * @return HTTPResponse
     */
    inline HTTPResponse health() { return request(HTTPDevice::Method::GET, "/health"); }

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
#endif  // DCS_INFLUX_H
