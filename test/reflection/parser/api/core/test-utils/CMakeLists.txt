add_siplasplas_library(siplasplas-reflection-parser-api-core-testutils
SOURCES
    asttest.cpp
DEPENDS
    siplasplas-reflection-parser-api-core-clang
INCLUDE_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/.."
)

target_compile_definitions(siplasplas-reflection-parser-api-core-testutils
PUBLIC
    -DSIPLASPLAS_REFLECTION_PARSER_API_CORE_TESTUTILS_SOURCEDIR="${CMAKE_CURRENT_SOURCE_DIR}/files"
)
