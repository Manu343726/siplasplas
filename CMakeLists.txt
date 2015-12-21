project(ceplusplus)
cmake_minimum_required(VERSION 2.8)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/3rdParty)

add_subdirectory(3rdParty/ctti)
include_directories(3rdParty/ctti/include)
include_directories(3rdParty/cppasscii/snippets)
include(3rdParty/cmake/exec_target.cmake)

configure_exec_targets(PROJECT CPP)
include_directories(include)

if(NOT MSVC)
    set(CPP_GLOBAL_ALL_COMPILE_OPTIONS -std=c++14 -Wall -pedantic -DCTTI_STRING_MAX_LENGTH=1024 -ftemplate-depth-1024)
    set(CPP_GLOBAL_DEBUG_COMPILE_OPTIONS -O0 -g3 -fstack-protector-strong)
    set(CPP_GLOABL_RELEASE_COMPILE_OPTIONS -O3 -g0)
else()
	set(CPP_GLOBAL_ALL_COMPILE_OPTIONS /DCTTI_STRING_MAX_LENGTH=512)
endif()

add_subdirectory(examples)

enable_testing()
add_subdirectory(tests)
