#include "gtest/gtest.h"
#include "OpenGEX.h"
#include "OpenDDL.h"

TEST(GEX, Basic_OGEX) {
    EXPECT_EQ(OGEX::kStructureName, "name"_i32);
}

TEST(GEX, Basic_ODDL) {
    EXPECT_EQ(ODDL::kDataMaxPrimitiveArraySize, 256);
}