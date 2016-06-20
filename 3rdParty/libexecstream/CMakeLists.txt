add_siplasplas_thirdparty(libexecstream
GIT_REPOSITORY
    "https://github.com/Manu343726/libexecstream"
INCLUDE_DIRS
    include/
)

add_siplasplas_thirdparty_component(libexecstream DEFAULT
THIRD_PARTY
    libexecstream
BINARY_DIR
    src
)

find_package(Threads REQUIRED)

target_link_libraries(libexecstream INTERFACE ${CMAKE_THREAD_LIBS_INIT})
