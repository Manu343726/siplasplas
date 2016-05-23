add_siplasplas_thirdparty(googletest
GIT_REPOSITORY
    "https://github.com/google/googletest"
)

find_package(Threads REQUIRED)

add_siplasplas_thirdparty_component(gtest
THIRD_PARTY
    googletest
INCLUDE_DIRS
    googletest/include
BINARY
    googlemock/gtest/libgtest.a
LINK_LIBS
    ${CMAKE_THREAD_LIBS_INIT}
)

add_siplasplas_thirdparty_component(gmock
THIRD_PARTY
    googletest
INCLUDE_DIRS
    googlemock/include
BINARY
    googlemock/libgmock.a
DEPENDS
    gtest
)

add_siplasplas_thirdparty_component(gtest-main
THIRD_PARTY
    googletest
BINARY
    googlemock/gtest/libgtest_main.a
)

add_siplasplas_thirdparty_component(gmock-main
THIRD_PARTY
    googletest
BINARY
    googlemock/libgmock_main.a
)
