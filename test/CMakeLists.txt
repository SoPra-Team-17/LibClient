cmake_minimum_required(VERSION 3.10)

include_directories(../src)

set(SOURCE
		test1.cpp
	)

add_executable(allTests ${SOURCE})
target_compile_options(allTests PRIVATE ${COMMON_CXX_FLAGS})
target_link_libraries(allTests
		gtest
		gmock
		gtest_main
		SopraClient
		$<$<CONFIG:Debug>:--coverage>)
gtest_discover_tests(allTests)