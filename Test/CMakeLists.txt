# cases setup

me_add_test(Test_Hello test_hello.cpp)

me_add_test(Test_GeomMath test_geommath.cpp)
target_link_libraries(Test_GeomMath gtest_main GeomMath)

me_add_test(Test_Xg test_xg.cpp)
target_link_libraries(Test_Xg ${XG_LIBRARY})
me_add_xg_platform_dependencies(Test_Xg)

me_add_test(Test_Gex test_gex.cpp)
target_link_libraries(Test_Gex ${OPENDDL_LIBRARY} ${OPENGEX_LIBRARY})
