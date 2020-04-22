cmake_minimum_required(VERSION 3.10)
project(SopraClient)

set(SOURCE
        model/Model.cpp
        Network.cpp
        model/ClientState.cpp
        model/GameState.cpp
        LibClient.cpp
        )

set(HEADERS
        Callback.hpp
        model/Model.hpp
        Network.hpp
        model/ClientState.hpp
        model/GameState.hpp
        LibClient.hpp
        )

add_library(${PROJECT_NAME} SHARED ${SOURCE})
target_compile_options(${PROJECT_NAME} PRIVATE ${COMMON_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror -mtune=native -march=native)
target_link_libraries(${PROJECT_NAME}
        PUBLIC SopraCommon
        PUBLIC SopraNetwork
        $<$<CONFIG:Debug>:--coverage>)
target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)


execute_process(COMMAND cling-config --cmake OUTPUT_VARIABLE CPPYY_MODULE_PATH OUTPUT_STRIP_TRAILING_WHITESPACE)
list(INSERT CMAKE_MODULE_PATH 0 ${CPPYY_MODULE_PATH})
find_package(Cppyy)

cppyy_add_bindings(
        "SopraClient"
        LANGUAGE_STANDARD "17"
        INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
        LINK_LIBRARIES ${PROJECT_NAME}
        H_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
        H_FILES ${HEADERS})