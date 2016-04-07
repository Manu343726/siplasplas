project(siplasplas)
cmake_minimum_required(VERSION 2.8)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/3rdParty)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(SIPLASPLAS_VERSION_MAJOR 0)
set(SIPLASPLAS_VERSION_MINOR 0)
set(SIPLASPLAS_VERSION_PATCH 0)

include(cmake/deftargets.cmake)
include(cmake/install.cmake)
add_subdirectory(src)
add_subdirectory(examples)
enable_testing()
add_subdirectory(tests)

include(cmake/cpack.cmake)
