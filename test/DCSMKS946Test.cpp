#include "DeviceController.h"
#include "MKS946.h"
#include "gtest/gtest.h"
#include <iostream>

AbstractDeviceController<MKS946> controller;

TEST(MKS946, connection) {
    TCPConnectionParameters p;
    p.IPaddress = "192.168.168.105";
    p.port = 5300;

    auto &device = controller.device;
    controller.connect(&p);
    ASSERT_TRUE(device->isConnected());
}

TEST(MKS946, identification) {
    auto &device = controller.device;
    ASSERT_TRUE(device->isConnected());
    ASSERT_EQ("946", device->getModel());
    std::cout << device->getSerialNumber() << std::endl;
    std::cout << device->getModuleType() << std::endl;
    for(int i = 1; i <= 6; ++i) {
        std::cout << i << ": " << device->getFirmware(static_cast<MKS946::CH>(i))
                  << std::endl;
    }
    for(int i = 1; i <= 5; ++i) {
        std::cout << i << ": " << device->getSerialNumber(static_cast<MKS946::CH>(i))
                  << std::endl;
    }
}

TEST(MKS946, monitorPressure) {
    auto &device = controller.device;
    ASSERT_TRUE(device->isConnected());
    std::cout << device->getPressureAll() << std::endl;
    std::cout << device->getPressure(MKS946::CH::CH3) << std::endl;
}

TEST(MKS946, monitorFlow) {
    auto &device = controller.device;
    ASSERT_TRUE(device->isConnected());
    std::cout << device->getFlow(MKS946::CH::CH1) << std::endl;
    std::cout << device->getFlowSetPoint(MKS946::CH::CH1) << std::endl;
    std::cout << device->getFlowMode(MKS946::CH::CH1) << std::endl;
}
