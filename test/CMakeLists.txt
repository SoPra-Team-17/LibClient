cmake_minimum_required(VERSION 3.10)

include_directories(../src)

set(SOURCE
		test1.cpp
	)

add_executable(allTests ${SOURCE})
target_link_libraries(allTests gtest gmock gtest_main SopraClient)
gtest_discover_tests(allTests)