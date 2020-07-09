#include "DCSServer.h"
#include "gtest/gtest.h"
TEST(DCSServer, factoryCreation) {
    DCSServer server("localhost", 6669);
    auto *object = server.addObject<DCSObject>("testObject");
    ASSERT_EQ("testObject", object->getName());
    ASSERT_EQ("DCSObject", object->getType());
    auto *object2 = server.addObject<DCSObject>("testObject2");
    ASSERT_NE(object, object2);
    ASSERT_THROW(server.addObject<DCSObject>("testObject"), std::runtime_error);
}
