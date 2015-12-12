
include(cmake/gmock)

install_gtestgmock(VERSION 1.57.0)

if(NOT MSVC)
    set(CPP_test_ALL_COMPILE_OPTIONS -std=c++14 -Wall -pedantic -ftemplate-depth-1024)
    set(CPP_test_DEBUG_COMPILE_OPTIONS -O0 -g3 -fstack-protector-strong)
    set(CPP_test_RELEASE_COMPILE_OPTIONS -O3 -g0)
endif()

gmock_test_target(NAME variant) 

