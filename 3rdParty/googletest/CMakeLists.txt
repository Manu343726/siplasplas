add_siplasplas_thirdparty(googletest
GIT_REPOSITORY
    "https://github.com/google/googletest"
CMAKE_EXTRA_ARGS
    -Dgtest_force_shared_crt=ON
)

find_package(Threads REQUIRED)

add_siplasplas_thirdparty_component(gtest STATIC
THIRD_PARTY
    googletest
INCLUDE_DIRS
    googletest/include
BINARY_DIR
    googlemock/gtest
LINK_LIBS
    ${CMAKE_THREAD_LIBS_INIT}
)

add_siplasplas_thirdparty_component(gmock STATIC
THIRD_PARTY
    googletest
INCLUDE_DIRS
    googlemock/include
BINARY_DIR
    googlemock
DEPENDS
    gtest
)

add_siplasplas_thirdparty_component(gtest-main STATIC
THIRD_PARTY
    googletest
BINARY_DIR
    googlemock
BINARY_NAME
    gtest_main
)

add_siplasplas_thirdparty_component(gmock-main STATIC
THIRD_PARTY
    googletest
BINARY_DIR
    googlemock
BINARY_NAME
    gmock_main
)
