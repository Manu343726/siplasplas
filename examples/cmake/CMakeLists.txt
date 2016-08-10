add_siplasplas_example_simple(cmake-project EXCLUDE_FROM_RUN_ALL DEPENDS siplasplas-cmake)
configure_siplasplas_reflection(examples-cmake-project)

target_compile_definitions(examples-cmake-project PRIVATE
    CMAKE_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
    CMAKE_BINARY_DIR="${CMAKE_BINARY_DIR}"
)
