#include "gtest/gtest.h"
#include "crossguid/guid.hpp"
#include <iostream>

TEST(TEST_Xg, GUID_Empty) {
    auto g = xg::Guid();
    EXPECT_EQ(g.str(), "00000000-0000-0000-0000-000000000000");
}

TEST(TEST_Xg, GUID_New) {
    EXPECT_NO_FATAL_FAILURE({
                                auto g = xg::newGuid();
                                std::cout << "new guid: " << g << std::endl;
                            });
}
