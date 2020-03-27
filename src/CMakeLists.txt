cmake_minimum_required(VERSION 3.10)
project(SopraClient)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE
	library.cpp
	)

set(INCLUDE
	library.h
	)

add_library(${PROJECT_NAME} SHARED ${SOURCE})

install(TARGETS ${PROJECT_NAME} DESTINATION /usr/lib)
install(FILES ${INCLUDE} DESTINATION /usr/include/${PROJECT_NAME})