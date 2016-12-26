add_subdirectory(clang)

add_siplasplas_library(siplasplas-reflection-parser-api-core
SOURCES
    core.cpp
DEPENDS
    siplasplas-utility
    siplasplas-reflection-api-core-clang
)
