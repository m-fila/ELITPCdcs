#include "DCSArt.h"
#include "ProjectVersion.h"
#include <chrono>
#include <iostream>
#include <thread>
int main(int argc, char *argv[]) {
    std::cout << DCSArt::ascii << "v" << DCS_VERSION << std::endl;
    while(true) {
        std::this_thread::sleep_for(std::chrono::hours(1));
    }
    return 0;
}
