project(ceplusplus)
cmake_minimum_required(VERSION 3.3)

add_subdirectory(3rdParty/ctti)
include_directories(3rdParty/ctti/include)
include(3rdParty/cmake/exec_target.cmake)

configure_exec_targets(PROJECT CPP)
include_directories(include)

add_subdirectory(examples)
