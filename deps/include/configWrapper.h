#ifndef CONFIGWRAPPER_H 
#define CONFIGWRAPPER_H
#include <sstream>
#include <string>
#include <fstream>
#include <json/json.h>
#include <json/json-forwards.h>
class ConfigWrapper {
public:
  ConfigWrapper();
Json::Value& parse(std::stringstream& ss);
Json::Value& parse(std::string& str);
Json::Value& parse(std::ifstream& ifs);
private:
  Json::CharReaderBuilder builder;
  std::string errs;
  Json::Value obj;
};
#endif // CONFIGWRAPPER_H
