add_protobuf_library(examples-protoserialization-commmodel
SOURCES
    commmodel.proto
)
configure_siplasplas_reflection(examples-protoserialization-commmodel
FILES
    ${CMAKE_CURRENT_BINARY_DIR}/commmodel.pb.h
)

add_siplasplas_example(protoserialization
SOURCES
    protoserialization.cpp
    mapping.cpp
DEPENDS
    siplasplas-reflection-static
    examples-protoserialization-commmodel
)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    math(EXPR MAX_CONSTEXPR_STEPS "1024*1024*1024")
    math(EXPR MAX_CONSTEXPR_DEPTH "1024*1024*1024")

    message(STATUS "  Max constexpr steps: ${MAX_CONSTEXPR_STEPS}")
    message(STATUS "  Max constexpr depth: ${MAX_CONSTEXPR_DEPTH}")

    target_compile_options(examples-protoserialization PRIVATE
        -fconstexpr-steps=${MAX_CONSTEXPR_STEPS}
        -fconstexpr-depth=${MAX_CONSTEXPR_DEPTH}
        -fconstexpr-backtrace-limit=0
    )
endif()

configure_siplasplas_reflection(examples-protoserialization)
