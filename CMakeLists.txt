project(siplasplas)
cmake_minimum_required(VERSION 2.8)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/3rdParty)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

include(cmake/deftargets.cmake)
add_subdirectory(src)
add_subdirectory(examples)
enable_testing()
add_subdirectory(tests)

