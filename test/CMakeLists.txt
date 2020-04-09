cmake_minimum_required(VERSION 3.10)

include_directories(../src)
include_directories(../src/datatypes)

add_executable(allTests test1.cpp)
target_link_libraries(allTests gtest gmock gtest_main SopraClient)
gtest_discover_tests(allTests)