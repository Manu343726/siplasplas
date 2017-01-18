add_subdirectory(clang)

add_siplasplas_library(siplasplas-reflection-parser-api-core
SOURCES
    cursormatcher.cpp
DEPENDS
    siplasplas-utility
    siplasplas-reflection-parser-api-core-clang
)
