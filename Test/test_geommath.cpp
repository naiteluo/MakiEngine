#include "gtest/gtest.h"
#include "GeomMath.h"

TEST(GeoMath, CrossProduct) {
    Me::Matrix3X3 a = {1, 2, 3};
    Me::Matrix3X3 b = {5, 6, 7};
    Me::Matrix3X3 c = {};

    Me::CrossProduct(c, a, b);

    EXPECT_EQ(c[0], -4);
    EXPECT_EQ(c[1], 8);
    EXPECT_EQ(c[2], -4);
}
