cmake_minimum_required(VERSION 3.10)
project(LibClient)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif (NOT CMAKE_BUILD_TYPE)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("\n##########################")
    message("###### DEBUG BUILD #######")
    message("##########################\n")
    set(COMMON_CXX_FLAGS ${COMMON_CXX_FLAGS} --coverage)
else ()
    set(COMMON_CXX_FLAGS ${COMMON_CXX_FLAGS} -O3)
    message("\n############################")
    message("###### RELEASE BUILD #######")
    message("############################\n")
endif ()

add_subdirectory(src)
add_subdirectory(extern/LibCommon)
add_subdirectory(extern/WebsocketCPP)

option(LIBCLIENT_TESTS "Build the tests" ON)
if (LIBCLIENT_TESTS)

    # Do not install googletest
    # Non-ideal solution, suggested by https://stackoverflow.com/a/35345933
    # Replace install() to do-nothing macro.
    macro(install)
    endmacro()

    if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
        # Download and unpack googletest at configure time
        configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
        execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
                RESULT_VARIABLE result
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download)
        if (result)
            message(FATAL_ERROR "CMake step for googletest failed: ${result}")
        endif ()

        execute_process(COMMAND ${CMAKE_COMMAND} --build .
                RESULT_VARIABLE result
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download)
        if (result)
            message(FATAL_ERROR "Build step for googletest failed: ${result}")
        endif ()

        # Add googletest directly to the build
        add_subdirectory("${CMAKE_BINARY_DIR}/googletest-src"
                "${CMAKE_BINARY_DIR}/googletest-build"
                )
    endif ()
    enable_testing()
    include(GoogleTest)
    add_subdirectory(test)

    # Restore original install() behavior.
    macro(install)
        _install(${ARGN})
    endmacro()
endif ()
