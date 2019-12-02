#ifndef LOADER_H
#define LOADER_H
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>

struct Loader_item{
    std::string device;
    std::string Id;
    std::string address;
    std::string port;
    friend std::istream &operator>>( std::istream  &input, Loader_item &L );
};

class Loader{
public:
    Loader(int argc,char* argv[]);
    bool isOpened(){return config.is_open();}
    void parse();
    std::vector<Loader_item>getItems(){return items;}
    std::string getAddress(){return address;}
    std::string getPort(){return port;}
private:
    std::string configPath;
    std::ifstream config;
    std::string address;
    std::string port;
    std::vector<Loader_item> items;
};
#endif //Loader_H
