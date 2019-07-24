#include "loader.h"

std::istream &operator>>(std::istream &input,loader &L){
    std::getline(input,L.device,':');
    std::getline(input>>std::ws,L.Id,' ');
    std::getline(input,L.address,' ');
    std::getline(input,L.port);
    return input;
}

std::vector<loader> loader::parse(const char *filename){
    std::ifstream config(filename);
    std::string line;
    std::vector<loader> loaders;
    if (config.is_open()){
        while ( getline (config>>std::ws,line)){
            if(line[0]!='#'&& line.size()!=0){
                std::stringstream iss(line);
                loader L;
                iss>>L;
                loaders.push_back(L);
                std::cout<<L.device<<","<<L.address<<","<<L.Id<<","<<L.port<<std::endl;
            }
        }
    config.close();
    }
    else{
    std::cout << "Unable to load config file"<<std::endl;
    }
    return loaders;
}
