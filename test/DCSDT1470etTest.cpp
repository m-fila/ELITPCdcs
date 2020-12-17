#include "DT1470ET.h"
#include "DeviceController.h"
#include "gtest/gtest.h"
#include <iostream>

AbstractDeviceController<DT1470ET> controller;
uint channels = 4;

TEST(DT1470ET, connection) {
    TCPConnectionParameters p;
    p.IPaddress = "192.168.168.42";
    p.port = 1470;

    auto &device = controller.device;
    device->setBD(0);
    ASSERT_EQ(0, device->getBD());
    controller.connect(&p);
    ASSERT_TRUE(device->isConnected());
    ASSERT_EQ("DT1470ET", device->getModel());
    std::cout << " firmware version: " << device->getFirmwareVersion() << std::endl;
}

TEST(DT1470ET, rampUp) {
    auto &device = controller.device;
    ASSERT_TRUE(device->isConnected());
    std::cout << "RAMP UP: " << device->getRampUp(DT1470ET::CHANNEL::ALL) << std::endl;
    for(size_t ch = 0; ch < channels; ++ch) {
        auto channel = static_cast<DT1470ET::CHANNEL>(ch);
        auto prev = std::stod(device->getRampUp(channel));
        double next = 300;
        device->setRampUp(channel, next);
        EXPECT_EQ(next, std::stod(device->getRampUp(channel)));
        device->setRampDown(channel, prev);
    }
}

TEST(DT1470ET, rampDown) {
    auto &device = controller.device;
    ASSERT_TRUE(device->isConnected());
    std::cout << "RAMP DOWN: " << device->getRampDown(DT1470ET::CHANNEL::ALL)
              << std::endl;
    for(size_t ch = 0; ch < channels; ++ch) {
        auto channel = static_cast<DT1470ET::CHANNEL>(ch);
        auto prev = std::stod(device->getRampDown(channel));
        double next = 300;
        device->setRampDown(channel, next);
        EXPECT_EQ(next, std::stod(device->getRampDown(channel)));
        device->setRampDown(channel, prev);
    }
}
TEST(DT1470ET, current) {
    auto &device = controller.device;
    ASSERT_TRUE(device->isConnected());
    std::cout << "CURRENT: " << device->getCurrent(DT1470ET::CHANNEL::ALL) << std::endl;
}
TEST(DT1470ET, currentSet) {
    auto &device = controller.device;
    ASSERT_TRUE(device->isConnected());
    std::cout << "CURRENT: " << device->getCurrentSet(DT1470ET::CHANNEL::ALL)
              << std::endl;
    for(size_t ch = 0; ch < channels; ++ch) {
        auto channel = static_cast<DT1470ET::CHANNEL>(ch);
        auto prev = std::stod(device->getCurrentSet(channel));
        double next = 300;
        device->setCurrentSet(channel, next);
        EXPECT_EQ(next, std::stod(device->getCurrentSet(channel)));
        device->setCurrentSet(channel, prev);
    }
}
TEST(DT1470ET, voltage) {
    auto &device = controller.device;
    ASSERT_TRUE(device->isConnected());
    std::cout << "VOLTAGE: " << device->getVoltage(DT1470ET::CHANNEL::ALL) << std::endl;
}
TEST(DT1470ET, voltageSet) {
    auto &device = controller.device;
    ASSERT_TRUE(device->isConnected());
    std::cout << "VOLTAGE SET: " << device->getVoltageSet(DT1470ET::CHANNEL::ALL)
              << std::endl;
    for(size_t ch = 0; ch < channels; ++ch) {
        auto channel = static_cast<DT1470ET::CHANNEL>(ch);
        auto prev = std::stod(device->getVoltageSet(channel));
        double next = 10;
        device->setVoltageSet(channel, next);
        EXPECT_EQ(next, std::stod(device->getVoltageSet(channel)));
        device->setVoltageSet(channel, prev);
    }
}
TEST(DT1470ET, voltageMax) {
    auto &device = controller.device;
    ASSERT_TRUE(device->isConnected());
    std::cout << "CURRENT MAX: " << device->getVoltageMax(DT1470ET::CHANNEL::ALL)
              << std::endl;
    for(size_t ch = 0; ch < channels; ++ch) {
        auto channel = static_cast<DT1470ET::CHANNEL>(ch);
        auto prev = std::stod(device->getVoltageMax(channel));
        double next = 200;
        device->setVoltageMax(channel, next);
        EXPECT_EQ(next, std::stod(device->getVoltageMax(channel)));
        device->setVoltageMax(channel, prev);
    }
}
TEST(DT1470ET, voltageStatus) {
    auto &device = controller.device;
    ASSERT_TRUE(device->isConnected());
    std::cout << "STATUS: " << device->getStatus(DT1470ET::CHANNEL::ALL) << std::endl;
}

TEST(DT1470ET, polarity) {
    auto &device = controller.device;
    ASSERT_TRUE(device->isConnected());
    std::cout << "POLARITY: " << device->getPolarity(DT1470ET::CHANNEL::ALL) << std::endl;
}

TEST(DT1470ET, interlock) {
    auto &device = controller.device;
    ASSERT_TRUE(device->isConnected());
    auto prev = device->getInterlockMode();
    std::cout << "MODE: " << prev << std::endl;
    std::cout << "STATUS: " << device->getInterlockStatus() << std::endl;
    device->setInterlockMode(DT1470ET::INTERLOCK::CLOSED);
    EXPECT_EQ("CLOSED", device->getInterlockMode());
    device->setInterlockMode(DT1470ET::INTERLOCK::OPEN);
    EXPECT_EQ("OPEN", device->getInterlockMode());
    device->setInterlockMode(prev == "OPEN" ? DT1470ET::INTERLOCK::OPEN
                                            : DT1470ET::INTERLOCK::CLOSED);
    std::cout << "ALARM: " << device->getAlarmSignal() << std::endl;
    device->clearAlarmSignal();
    std::cout << "ALARM: " << device->getAlarmSignal() << std::endl;
}
