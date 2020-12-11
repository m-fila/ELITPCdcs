#include "DCSServer.h"
#include "gtest/gtest.h"
TEST(DCSServer, objectCreation) {
    DCSServer server("localhost", 6669);
    auto *object = server.addObject<DCSObject>("testObject");
    ASSERT_EQ("testObject", object->getName());
    ASSERT_EQ("DCSObject", object->getType());
    auto *object2 = server.addObject<DCSObject>("testObject2");
    ASSERT_NE(object, object2);
    ASSERT_THROW(server.addObject<DCSObject>("testObject"), std::runtime_error);
}

TEST(DCSServer, variableCreation) {
    DCSServer server("localhost", 6669);
    auto *object = server.addObject<DCSObject>("testObject");
    auto &v = object->addVariable("testVar", &UA_TYPES[UA_TYPES_INT32]);
    ASSERT_EQ("testVar", v.getName());
    v.setValue(2);
    ASSERT_EQ(2, v.getValue<int>());
    auto m = object->addMethod("testMethod", "test descr", {}, {},
                               [&v](const UA_Variant *, UA_Variant *) { v.setValue(3); });
    m(nullptr, nullptr);
    ASSERT_EQ(3, v.getValue<int>());
}
