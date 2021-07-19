# gtest setup
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
FetchContent_MakeAvailable(googletest)
enable_testing()

# cases setup

add_executable(TEST_Hello test_hello.cpp)
target_link_libraries(TEST_Hello gtest_main)

add_executable(TEST_GeomMath test_geommath.cpp)
target_link_libraries(TEST_GeomMath gtest_main GeomMath)

include(GoogleTest)
gtest_discover_tests(
        TEST_Hello
        TEST_GeomMath
)