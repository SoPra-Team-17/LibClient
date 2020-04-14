cmake_minimum_required(VERSION 3.10)
project(SopraClient)

# build options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror -mtune=native -march=native")


set(SOURCE
        model/Model.cpp
        model/ClientState.cpp
        model/GameState.cpp
        Network.cpp
        )

set(INCLUDE
        Callback.hpp
        model/Model.hpp
        model/ClientState.hpp
        model/GameState.hpp
        Network.hpp
        )

include_directories(.)
include_directories(../extern/LibCommon/src)

add_library(${PROJECT_NAME} SHARED ${SOURCE})
target_link_libraries(${PROJECT_NAME} SopraNetwork)

install(TARGETS ${PROJECT_NAME} DESTINATION /usr/lib)
install(FILES ${INCLUDE} DESTINATION /usr/include/${PROJECT_NAME})