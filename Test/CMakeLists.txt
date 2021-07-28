# cases setup

me_add_test(TEST_Hello test_hello.cpp)

me_add_test(TEST_GeomMath test_geommath.cpp)
target_link_libraries(TEST_GeomMath gtest_main GeomMath)

me_add_test(TEST_Xg test_xg.cpp)
target_link_libraries(TEST_Xg ${XG_LIBRARY})
me_add_xg_platform_dependencies(TEST_Xg)
