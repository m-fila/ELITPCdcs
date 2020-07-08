#include "gtest/gtest.h"

TEST(dummyTest, whatTest) {
    EXPECT_EQ(2, []() { return 2; }());
}