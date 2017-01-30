add_subdirectory(test-utils)
add_subdirectory(clang)

add_siplasplas_test(reflection-parser-api-core
SOURCES
    matcher_test.cpp
DEPENDS
    siplasplas-reflection-parser-api-core-testutils
    siplasplas-reflection-parser-api-core
)
