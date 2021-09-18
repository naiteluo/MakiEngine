#include "gtest/gtest.h"
#include <iostream>
#include "GeomMath.h"

using namespace std;
using namespace Me;

TEST(GeomMathVector, Initiation) {
    Vector3f v1 = {1.0f, 2.0f, 3.0f};
    EXPECT_EQ(v1.x, 1.0f);
    EXPECT_EQ(v1.y, 2.0f);
    EXPECT_EQ(v1.z, 3.0f);
}

TEST(GeomMathVector, Equality) {
    Vector3f v1 = {1.0f, 0.0f, 0.0f};
    Vector3f v2 = {1.0f, 0.0f, 0.0f};
    Vector3f v3 = {1.0f, 1.0f, 1.0f};
    EXPECT_TRUE(VectorCompare(v1, v2));
    EXPECT_FALSE(VectorCompare(v1, v3));
}

TEST(GeomMathVector, CrossProduct) {
    Vector3f a = {1.0f, 2.0f, 3.0f};
    Vector3f b = {5.0f, 6.0f, 7.0f};
    Vector3f c;
    Vector3f expected = {-4.000000f, 8.000000f, -4.000000f};
    CrossProduct(c, a, b);
    EXPECT_TRUE(VectorCompare(c, expected));
}

TEST(GeomMathVector, DotProduct) {
    Vector3f a = {1.0f, 2.0f, 3.0f};
    Vector3f b = {5.0f, 6.0f, 7.0f};
    float c;
    float expected = 38.0f;
    DotProduct(c, a, b);
    EXPECT_EQ(c, expected);
}

TEST(GeomMathVector, ElementProduct) {
    Vector3f a = {1.0f, 2.0f, 3.0f};
    Vector3f b = {5.0f, 6.0f, 7.0f};
    Vector3f c;
    Vector3f expected = {5.000000f, 12.000000f, 21.000000f};
    MulByElement(c, a, b);
    EXPECT_TRUE(VectorCompare(c, expected));
}

TEST(GeomMathVector, Add) {
    Vector4f a = {-3.0f, 3.0f, 6.0f, 1.0f};
    Vector4f b = {2.0f, 0.0f, -0.7f, 0.0f};
    Vector4f c;
    Vector4f expected = {-1.000000f, 3.000000f, 5.300000f, 1.000000f};
    VectorAdd(c, a, b);
    EXPECT_TRUE(VectorCompare(c, expected));
}

TEST(GeomMathVector, Sub) {
    Vector4f a = {-3.0f, 3.0f, 6.0f, 1.0f};
    Vector4f b = {2.0f, 0.0f, -0.7f, 0.0f};
    Vector4f c;
    Vector4f expected = {-5.000000f, 3.000000f, 6.700000f, 1.000000f};
    VectorSub(c, a, b);
    EXPECT_TRUE(VectorCompare(c, expected));
}

TEST(GeomMathVector, Normalize) {
    Vector4f a = {-5.000000f, 3.000000f, 6.700000f, 1.000000f};
    Vector4f expected = {
            -0.5594016909599304f,
            0.3356410264968872f,
            0.7495982646942139f,
            0.1118803396821022f
    };
    Normalize(a);
    EXPECT_TRUE(VectorCompare(a, expected));
}


