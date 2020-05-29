cmake_minimum_required(VERSION 3.10)
project(SopraClient)

set(SOURCE
        model/Model.cpp
        Network.cpp
        model/ClientState.cpp
        model/GameState.cpp
        LibClient.cpp
        )

add_library(${PROJECT_NAME} SHARED ${SOURCE})
target_compile_options(${PROJECT_NAME} PRIVATE ${COMMON_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror -mtune=native -march=native)
target_link_libraries(${PROJECT_NAME}
        SopraCommon
        SopraNetwork
        $<$<CONFIG:Debug>:--coverage>)
target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/"
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
        FILES_MATCHING PATTERN "*.hpp"
        )