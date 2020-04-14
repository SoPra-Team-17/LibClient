cmake_minimum_required(VERSION 3.10)
project(SopraClient)

# build options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror -mtune=native -march=native")

find_package(nlohmann_json REQUIRED)

set(SOURCE
        model/Model.cpp
        Network.cpp
        model/ClientState.cpp
        model/GameState.cpp
        LibClient.cpp
        )

set(INCLUDE
        Callback.hpp
        model/Model.hpp
        model/GameState.hpp
        model/ClientState.hpp
        Network.hpp
        LibClient.hpp
        )

include_directories(.)
include_directories(../extern/LibCommon/src)
include_directories(../extern/LibCommon/src/datatypes)

add_library(${PROJECT_NAME} SHARED ${SOURCE})
target_link_libraries(${PROJECT_NAME} SopraNetwork nlohmann_json::nlohmann_json)

install(TARGETS ${PROJECT_NAME} DESTINATION /usr/lib)
install(FILES ${INCLUDE} DESTINATION /usr/include/${PROJECT_NAME})