cmake_minimum_required(VERSION 3.10)
project(LibClient_test)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(googletest)

add_executable(${PROJECT_NAME} main.cpp test1.cpp)
target_link_libraries(${PROJECT_NAME}
        gtest gmock pthread
        )
