add_protobuf_library(examples-protoserialization-commmodel
SOURCES
    commmodel.proto
)
configure_siplasplas_reflection(examples-protoserialization-commmodel
FILES
    ${CMAKE_CURRENT_BINARY_DIR}/commmodel.pb.h
)

add_siplasplas_example_simple(protoserialization
DEPENDS
    siplasplas-reflection-static
    examples-protoserialization-commmodel
)

configure_siplasplas_reflection(examples-protoserialization)
