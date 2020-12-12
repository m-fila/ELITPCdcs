#include "DeviceController.h"
#include "TPG362.h"
#include "gtest/gtest.h"
#include <iostream>

AbstractDeviceController<TPG362> controller;

TEST(TPG362, connection) {
    TCPConnectionParameters p;
    p.IPaddress = "192.168.168.103";
    p.port = 5301;

    auto &device = controller.device;
    controller.connect(&p);
    ASSERT_TRUE(device->isConnected());
}

TEST(MKS946, identification) {
    auto &device = controller.device;
    ASSERT_TRUE(device->isConnected());

    std::cout << device->getFirmwareVersion() << std::endl;
    std::cout << device->getHardwareVersion() << std::endl;
    std::cout << device->getModel() << std::endl;
    std::cout << device->getPartNumber() << std::endl;
    std::cout << device->getSerialNumber() << std::endl;
}
