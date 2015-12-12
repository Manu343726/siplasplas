project(ceplusplus)
cmake_minimum_required(VERSION 3.3)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/3rdParty)

add_subdirectory(3rdParty/ctti)
include_directories(3rdParty/ctti/include)
include_directories(3rdParty/cppasscii/snippets)
include(3rdParty/cmake/exec_target.cmake)

configure_exec_targets(PROJECT CPP)
include_directories(include)

add_subdirectory(examples)

enable_testing()
add_subdirectory(tests)
