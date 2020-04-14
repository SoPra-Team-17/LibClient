cmake_minimum_required(VERSION 3.10)
project(SopraClient)

# build options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror -mtune=native -march=native")


set(SOURCE
        Callback.cpp
        model/Model.cpp
        Network.cpp
        model/ClientState.cpp model/GameState.cpp model/GameState.hpp LibClient.cpp LibClient.hpp)

set(INCLUDE
        Callback.hpp
        model/Model.hpp
        Network.hpp
        model/ClientState.hpp
        )

include_directories(.)
include_directories(../extern/LibCommon/src)

add_library(${PROJECT_NAME} SHARED ${SOURCE})
target_link_libraries(${PROJECT_NAME} SopraNetwork)

install(TARGETS ${PROJECT_NAME} DESTINATION /usr/lib)
install(FILES ${INCLUDE} DESTINATION /usr/include/${PROJECT_NAME})