#ifndef DCS_CHECK_LOGGER_H
#define DCS_CHECK_LOGGER_H
#include <iostream>
#include <string>
static constexpr const char *tmark = "\u2713";
static constexpr const char *xmark = "\u2717";
static void checkHelper(std::string name, std::string address, int port) {
    std::cout << "Checking " << name << " at " << address << ":" << port << std::endl;
}

static void logger(bool isOk, std::string message) {
    if(isOk) {
        std::cout << tmark << "\t" << message << std::endl;
    } else {
        std::cout << xmark << "\t" << message << std::endl;
    }
}
static void logger(bool isOk) {
    if(isOk) {
        logger(isOk, "Service online");
    } else {
        logger(isOk, "Service not responding");
    }
}
#endif  // DCS_CHECK_LOGGER_H
