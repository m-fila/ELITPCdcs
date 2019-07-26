#ifndef LOADER_H
#define LOADER_H
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
struct loader{
    std::string device;
    std::string Id;
    std::string address;
    std::string port;
    friend std::istream &operator>>( std::istream  &input, loader &L );
    static std::vector<loader> parse(const char* filename);
};

#endif // LOADER_H
