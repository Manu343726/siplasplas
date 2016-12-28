include(libclang)

add_siplasplas_test(reflection-parser-api-core-clang
SOURCES
    handletest.cpp
    uniquehandle_test.cpp
    sharedhandle_test.cpp
    nullhandle_test.cpp
    index_test.cpp
DEPENDS
    siplasplas-reflection-parser-api-core-clang
)

target_compile_definitions(siplasplas-reflection-parser-api-core-clang PUBLIC
    -DSIPLASPLAS_LIBCLANG_VERSION="${SIPLASPLAS_LIBCLANG_VERSION}"
    -DSIPLASPLAS_LIBCLANG_LIBRARY="${SIPLASPLAS_LIBCLANG_LIBRARY}"
    -DSIPLASPLAS_LIBCLANG_INCLUDE_DIR="${SIPLASPLAS_LIBCLANG_INCLUDE_DIR}"
    -DSIPLASPLAS_LIBCLANG_SYSTEM_INCLUDE_DIR="${SIPLASPLAS_LIBCLANG_SYSTEM_INCLUDE_DIR}"
)

message(STATUS "libclang version: ${SIPLASPLAS_LIBCLANG_VERSION} (${SIPLASPLAS_LIBCLANG_VERSION_MAJOR_MINOR})")
message(STATUS "libclang library: ${SIPLASPLAS_LIBCLANG_LIBRARY}")
message(STATUS "libclang include dir: ${SIPLASPLAS_LIBCLANG_INCLUDE_DIR}")
message(STATUS "libclang system include dir: ${SIPLASPLAS_LIBCLANG_SYSTEM_INCLUDE_DIR}")
