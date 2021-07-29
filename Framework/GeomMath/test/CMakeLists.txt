# cases setup

me_add_test(Test_GeomMath_Inner test.cpp)
target_link_libraries(Test_GeomMath_Inner GeomMath)

me_add_test(Test_GeomMath_Vector test_vector.cpp)
target_link_libraries(Test_GeomMath_Vector GeomMath)

me_add_test(Test_GeomMath_Matrix test_matrix.cpp)
target_link_libraries(Test_GeomMath_Matrix GeomMath)