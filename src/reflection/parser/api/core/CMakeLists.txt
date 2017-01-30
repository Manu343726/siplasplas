add_subdirectory(clang)

add_siplasplas_library(siplasplas-reflection-parser-api-core
SOURCES
    matcher.cpp
    matchfinder.cpp
    logger.cpp
    matchers/kind.cpp
    matchers/boolean.cpp
DEPENDS
    siplasplas-utility
    siplasplas-reflection-parser-api-core-clang
)
