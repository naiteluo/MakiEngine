#include "gtest/gtest.h"
#include <iostream>
#include "GeomMath.h"

using namespace std;
using namespace Me;

TEST(GeomMathMatrix, Initiation) {
    Matrix<int, 3, 2> m1 = {{{
                                     {1, 2},
                                     {3, 4},
                                     {5, 6}
                             }}};
    Matrix4X4f m2 = {{{
                              {1.00000000f, 0.00000000f, 0.00000000f, 0.00000000f},
                              {0.00000000f, 1.00000000f, 0.00000000f, 0.00000000f},
                              {0.00000000f, 0.00000000f, 1.00000000f, 0.00000000f},
                              {0.00000000f, 0.00000000f, 0.00000000f, 1.00000000f},
                      }}};
    EXPECT_EQ(m1[0][1], 2);
    EXPECT_EQ(m1[1][1], 4);
    EXPECT_EQ(m1[2][0], 5);
    EXPECT_EQ(m2[0][0], 1.0f);
    EXPECT_EQ(m2[3][3], 1.0f);
    EXPECT_EQ(m2[3][0], 0.0f);
    EXPECT_EQ(sizeof(m1.data) / sizeof(m1.data[0]), 3);
    EXPECT_EQ(sizeof(m2.data) / sizeof(m2.data[0]), 4);
}

TEST(GeomMathMatrix, Equality) {
    Matrix<int, 3, 2> m1 = {{{
                                     {1, 2},
                                     {3, 4},
                                     {5, 6}
                             }}};
    Matrix<int, 3, 2> m2 = {{{
                                     {1, 2},
                                     {3, 4},
                                     {5, 6}
                             }}};
    Matrix<int, 3, 2> m3 = {{{
                                     {0, 2},
                                     {0, 4},
                                     {0, 6}
                             }}};
    EXPECT_TRUE(MatrixCompare(m1, m2));
    EXPECT_FALSE(MatrixCompare(m1, m3));
}

TEST(GeomMathMatrix, BuildIdentityMatrix) {
    Matrix4X4f m1;
    Matrix4X4f expected = {{{
                                    {1.00000000f, 0.00000000f, 0.00000000f, 0.00000000f},
                                    {0.00000000f, 1.00000000f, 0.00000000f, 0.00000000f},
                                    {0.00000000f, 0.00000000f, 1.00000000f, 0.00000000f},
                                    {0.00000000f, 0.00000000f, 0.00000000f, 1.00000000f},
                            }}};
    BuildIdentityMatrix(m1);
    EXPECT_TRUE(MatrixCompare(m1, expected));
}

TEST(GeomMathMatrix, MatrixRotationYawPitchRoll) {
    Matrix4X4f m1;
    Matrix4X4f expected = {{{
                                    {0.92556417f, 0.37202555f, -0.07019954f, 0.00000000f},
                                    {-0.32757968f, 0.87992316f, 0.34413192f, 0.00000000f},
                                    {0.18979606f, -0.29552022f, 0.93629342f, 0.00000000f},
                                    {0.00000000f, 0.00000000f, 0.00000000f, 1.00000000f},
                            }}};
    BuildIdentityMatrix(m1);
    MatrixRotationYawPitchRoll(m1, 0.2f, 0.3f, 0.4f);
    EXPECT_TRUE(MatrixCompare(m1, expected));
}

TEST(GeomMathMatrix, MatrixRotationY) {
    Matrix4X4f m1;
    Matrix4X4f expected = {{{
                                    {-0.0000000437113883f, 0.0000000000000000f, -1.0000000000000000f, 0.0000000000000000f},
                                    {0.0000000000000000f, 1.0000000000000000f, 0.0000000000000000f, 0.0000000000000000f},
                                    {1.0000000000000000f, 0.0000000000000000f, -0.0000000437113883f, 0.0000000000000000f},
                                    {0.0000000000000000f, 0.0000000000000000f, 0.0000000000000000f, 1.0000000000000000f},
                            }}};
    BuildIdentityMatrix(m1);
    MatrixRotationY(m1, PI / 2.0f);

    EXPECT_TRUE(MatrixCompare(m1, expected));
}

TEST(GeomMathMatrix, MatrixRotationZ) {
    Matrix4X4f m1;
    Matrix4X4f expected = {{{
                                    {-0.0000000437113883f, 1.0000000000000000f, 0.0000000000000000f, 0.0000000000000000f},
                                    {-1.0000000000000000f, -0.0000000437113883f, 0.0000000000000000f, 0.0000000000000000f},
                                    {0.0000000000000000f, 0.0000000000000000f, 1.0000000000000000f, 0.0000000000000000f},
                                    {0.0000000000000000f, 0.0000000000000000f, 0.0000000000000000f, 1.0000000000000000f},
                            }}};
    BuildIdentityMatrix(m1);
    MatrixRotationZ(m1, PI / 2.0f);

    EXPECT_TRUE(MatrixCompare(m1, expected));
}

TEST(GeomMathMatrix, MatrixTranslation) {
    Matrix4X4f m1;
    Matrix4X4f expected = {{{
                                    {1.00000000f, 0.00000000f, 0.00000000f, 0.00000000f},
                                    {0.00000000f, 1.00000000f, 0.00000000f, 0.00000000f},
                                    {0.00000000f, 0.00000000f, 1.00000000f, 0.00000000f},
                                    {5.00000000f, 6.50000000f, -7.00000000f, 1.00000000f},
                            }}};
    BuildIdentityMatrix(m1);
    MatrixTranslation(m1, 5.0f, 6.5f, -7.0f);

    EXPECT_TRUE(MatrixCompare(m1, expected));
}

TEST(GeomMathMatrix, TransformCoord) {
    Vector3f v = {1.0f, 0.0f, 0.0f};
    Vector3f vy = v;
    Matrix4X4f ry;
    Vector3f expected_y = {-0.0000000437113883f, 0.0000000000000000f, -1.0000000000000000f};
    BuildIdentityMatrix(ry);
    MatrixRotationY(ry, PI / 2.0f);
    TransformCoord(vy, ry);

    EXPECT_TRUE(VectorCompare(vy, expected_y));

    Vector3f vz = v;
    Matrix4X4f rz;
    Vector3f expected_z = {-0.0000000437113883f, 1.0000000000000000f, 0.0000000000000000f};
    BuildIdentityMatrix(rz);
    MatrixRotationZ(rz, PI / 2.0f);
    TransformCoord(vz, rz);

    EXPECT_TRUE(VectorCompare(vz, expected_z));
}

TEST(GeomMathMatrix, BuildViewMatrix) {
    Matrix4X4f view;
    Vector3f position = {0, 0, -5}, lookAt = {0, 0, 0}, up = {0, 1, 0};
    Matrix4X4f expected = {{{
                                    {1.0000000000000000f, 0.0000000000000000f, 0.0000000000000000f, 0.0000000000000000f},
                                    {0.0000000000000000f, 1.0000000000000000f, 0.0000000000000000f, 0.0000000000000000f},
                                    {0.0000000000000000f, 0.0000000000000000f, 1.0000000000000000f, 0.0000000000000000f},
                                    {-0.0000000000000000f, -0.0000000000000000f, 5.0000000000000000f, 1.0000000000000000f},
                            }}};
    BuildViewMatrix(view, position, lookAt, up);

    EXPECT_TRUE(MatrixCompare(view, expected));
}

TEST(GeomMathMatrix, BuildPerspectiveFovLHMatrix) {
//    Matrix4X4f view;
//    Vector3f position = {0, 0, -5}, lookAt = {0, 0, 0}, up = {0, 1, 0};
//    BuildViewMatrix(view, position, lookAt, up);
//    Matrix4X4f perspective;
//    float fov = PI / 2.0f, aspect = 16.0f / 9.0f, near = 1.0f, far = 100.0f;
//    BuildPerspectiveFovLHMatrix(perspective, fov, aspect, near, far);
//    Matrix4X4f mvp = view * perspective;

    Matrix4X4f view;
    Vector3f position = {0, 0, -5}, lookAt = {0, 0, 0}, up = {0, 1, 0};
    BuildViewMatrix(view, position, lookAt, up);
    Matrix4X4f perspective;
    float fov = PI / 2.0f, aspect = 16.0f / 9.0f, near = 1.0f, far = 100.0f;
    BuildPerspectiveFovLHMatrix(perspective, fov, aspect, near, far);
    Matrix4X4f mvp = view * perspective;

    Matrix4X4f expectedPerspective = {{{
                                               {0.5625000000000000f, 0.0000000000000000f, 0.0000000000000000f, 0.0000000000000000f},
                                               {0.0000000000000000f, 1.0000000000000000f, 0.0000000000000000f, 0.0000000000000000f},
                                               {0.0000000000000000f, 0.0000000000000000f, 1.0101009607315063f, 1.0000000000000000f},
                                               {0.0000000000000000f, 0.0000000000000000f, -1.0101009607315063f, 0.0000000000000000f},
                                       }}};

    Matrix4X4f expectedMvp = {{{
                                       {0.5625000000000000f, 0.0000000000000000f, 0.0000000000000000f, 0.0000000000000000f},
                                       {0.0000000000000000f, 1.0000000000000000f, 0.0000000000000000f, 0.0000000000000000f},
                                       {0.0000000000000000f, 0.0000000000000000f, 1.0101009607315063f, 1.0000000000000000f},
                                       {0.0000000000000000f, 0.0000000000000000f, 4.0404038429260254f, 5.0000000000000000f},
                               }}};

    EXPECT_TRUE(MatrixCompare(perspective, expectedPerspective));
    EXPECT_TRUE(MatrixCompare(mvp, expectedMvp));
}

TEST(GeomMathMatrix, Inverse) {
    Matrix4X4f invertable = {{{
        { 1.0f,  0.0f,  0.0f,  0.0f},
        { 0.0f,  1.0f,  0.0f,  0.0f},
        { 0.0f,  0.0f,  1.0f,  0.0f},
        {13.0f, 14.0f, 15.0f,  1.0f}
    }}};
    cout << "Known Invertable Matrix: " << invertable;
    assert(InverseMatrix4X4f(invertable));
    cout << "Inverse of Matrix: " << invertable;
    SUCCEED();
}