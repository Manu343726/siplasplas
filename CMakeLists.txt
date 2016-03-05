project(siplasplas)
cmake_minimum_required(VERSION 2.8)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/3rdParty)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

add_subdirectory(3rdParty/ctti)
include_directories(3rdParty/ctti/include)
include_directories(3rdParty/cppasscii/snippets)
include(3rdParty/cmake/exec_target.cmake)

configure_exec_targets(PROJECT CPP)
include_directories(include)

if(NOT MSVC)

    # gtest needs fileno() function at file gtest/include/gtest/internal/gtest-port.h,
    # but is considered an extension in cygwin GCC setup. Use GNU extensions.
    # See http://stackoverflow.com/questions/18784112/googletest-1-6-with-cygwin-1-7-compile-error-fileno-was-not-declared-in-this
    if(CYGWIN)
        set(STD_CXX gnu++14)
    else()
        set(STD_CXX c++14)
    endif()
    
    set(CPP_GLOBAL_ALL_COMPILE_OPTIONS -std=${STD_CXX} -Wall -pedantic -DCTTI_STRING_MAX_LENGTH=1024 -ftemplate-depth-1024)
    set(CPP_GLOBAL_DEBUG_COMPILE_OPTIONS -O0 -g3)
    set(CPP_GLOABL_RELEASE_COMPILE_OPTIONS -O3 -g0)
else()
    set(CPP_GLOBAL_ALL_COMPILE_OPTIONS /DCTTI_STRING_MAX_LENGTH=512)
endif()

include(cmake/deftargets.cmake)

add_subdirectory(src)
add_subdirectory(examples)

enable_testing()
add_subdirectory(tests)

