# cases setup

me_add_test(TEST_Hello test_hello.cpp)
me_add_test(TEST_GeomMath test_geommath.cpp)
target_link_libraries(TEST_GeomMath gtest_main GeomMath)
