add_subdirectory(bdd)
add_subdirectory(unit)

add_siplasplas_library(siplasplas-reflection-parser-api-core-clang-test NO_INSTALL
SOURCES
    clangtest.cpp
INCLUDE_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}"
DEPENDS
    siplasplas-reflection-parser-api-core-clang
)
