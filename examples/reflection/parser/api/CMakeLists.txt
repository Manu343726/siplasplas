add_siplasplas_example_simple(parsingheaders DEPENDS siplasplas-jobs siplasplas-reflection-parser-api-core)

target_compile_definitions(examples-parsingheaders PRIVATE
    -DSIPLASPLAS_EXAMPLES_REFLECTION_DIR="${CMAKE_SOURCE_DIR}/examples/reflection"
    -DSIPLASPLAS_INCLUDE_DIR="${CMAKE_SOURCE_DIR}/include"
    -DSIPLASPLAS_REFLECTION_OUTPUT_DIR="${CMAKE_BINARY_DIR}/output"
    -DSIPLASPLAS_EXPORTS_DIR="${CMAKE_BINARY_DIR}/exports"
    -DCTTI_INCLUDE_DIR="${CONAN_INCLUDE_DIRS_CTTI}"
)



