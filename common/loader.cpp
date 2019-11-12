#include "loader.h"

std::istream &operator>>(std::istream &input,Loader_item &L){
    std::getline(input>>std::ws,L.device,':');
    std::getline(input>>std::ws,L.Id,' ');
    std::getline(input>>std::ws,L.address,' ');
    std::getline(input>>std::ws,L.port,' ');
    return input;
}

Loader::Loader(int argc, char* argv[]){
    if(argc>1){
    configPath=argv[1];
    }
    else{
    std::string homedir=getenv("HOME");
    configPath=homedir+"/.config/dcs.config";
    }
    config=std::ifstream(configPath);
}

void Loader::parse(){
    std::string line;
    if (config.is_open()){
        while ( getline (config>>std::ws,line)){
            if(line[0]!='#'&& line.size()!=0){
                std::stringstream iss(line);
                Loader_item L;
                iss>>L;
                if(L.device=="SERVER"){
                    address=L.address;
                    port=L.port;
                }
                else{
                    items.push_back(L);
                }
            }
        }
    config.close();
    }
    else{
    std::cout << "Unable to load config file"<<std::endl;
    }
}