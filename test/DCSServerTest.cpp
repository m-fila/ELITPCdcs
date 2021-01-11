#include "DCSServer.h"
#include "open62541/types_dcsnodeset_generated.h"
#include "open62541/types_dcsnodeset_generated_handling.h"
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

TEST(DCSServer, arrayVariable) {
    DCSServer server("localhost", 6669);
    auto *object = server.addObject<DCSObject>("testObject");
    auto &v = object->addVariable("testVar", &UA_TYPES[UA_TYPES_DOUBLE], 1);
    v.setAccessWrite(true);
    auto a = static_cast<UA_Double *>(UA_Array_new(4, &UA_TYPES[UA_TYPES_DOUBLE]));
    a[0] = 1212;
    a[1] = 1212;
    a[2] = 1212;
    a[3] = 1212;
    v.setValue(a, 4);
    auto &v2 = object->addVariable("testVar2", &UA_TYPES[UA_TYPES_STRING], 1);
    auto a2 = static_cast<UA_String *>(UA_Array_new(4, &UA_TYPES[UA_TYPES_STRING]));
    a2[0] = UA_STRING_ALLOC("aasas");
    a2[1] = UA_STRING_ALLOC("aasds");
    a2[2] = UA_STRING_ALLOC("a23s");
    a2[3] = UA_STRING_ALLOC("asdasrs");
    v2.setValue(a2, 4);
    auto &v3 = object->addVariable(
        "testVar3", &UA_TYPES_DCSNODESET[UA_TYPES_DCSNODESET_LABELINFO], 0);
    UA_LabelInfo info;
    UA_LabelInfo_init(&info);
    info.description = UA_STRING_ALLOC("labels");
    info.channelLabelsSize = 2;
    info.channelLabels = static_cast<UA_LabelItemInfo *>(UA_Array_new(
        info.channelLabelsSize, &UA_TYPES_DCSNODESET[UA_TYPES_DCSNODESET_LABELITEMINFO]));
    info.channelLabels[0].description = UA_STRING_ALLOC("desc");
    info.channelLabels[0].label = UA_STRING_ALLOC("lab");
    info.channelLabels[1].description = UA_STRING_ALLOC("desc2");
    info.channelLabels[1].label = UA_STRING_ALLOC("lab2");
    v3.setValue(info);
    server.run();
}
