#include <string>
#include <fstream>
#include "json.hpp"
using json = nlohmann::json;
namespace ConfigLoader{
    json getMasterConfig(int argc, char **argv){
        json config;
        std::string configPath;
        if(argc>1){
            configPath=argv[1];
        }
        else{
            std::string homePath=getenv("HOME");
            configPath=homePath+"/.dcs/dcs.json";
        }
        std::ifstream ifs(configPath);
        if(ifs.is_open()){
            ifs>>config;
            return config;
        }
        else{
            throw std::runtime_error("Can't find config file");
        }
    }
}