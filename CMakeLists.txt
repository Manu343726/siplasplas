project(siplasplas)
cmake_minimum_required(VERSION 2.8)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/3rdParty ${CMAKE_SOURCE_DIR}/cmake)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(SIPLASPLAS_VERSION_MAJOR 0)
set(SIPLASPLAS_VERSION_MINOR 0)
set(SIPLASPLAS_VERSION_PATCH 0)

include(cmake/deftargets.cmake)
include(cmake/install.cmake)

add_subdirectory(3rdParty)
add_subdirectory(src)
add_subdirectory(examples)

option(SIPLASPLAS_TESTS_ENABLED "Enable/Disable unit tests" ON)

if(SIPLASPLAS_TESTS_ENABLED)
    enable_testing()
    add_subdirectory(test)
endif()

include(cmake/cpack.cmake)
