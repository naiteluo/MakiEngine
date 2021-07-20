#include "gtest/gtest.h"
#include "GeomMath.h"

TEST(GeoMath, CrossProduct) {
    Me::Vector3Type<float> a = {1, 2, 3};
    Me::Vector3Type<float> b = {5, 6, 7};
    Me::Vector3Type<float> c;

    Me::CrossProduct(c, a, b);

    EXPECT_EQ(c[0], -4);
    EXPECT_EQ(c[1], 8);
    EXPECT_EQ(c[2], -4);
}
