
add_siplasplas_library(siplasplas-reflection-dynamic
SOURCES
    type.cpp
    attributes/attribute.cpp
    class.cpp
DEPENDS
    siplasplas-reflection-common
    siplasplas-reflection-static
    siplasplas-allocator
    siplasplas-utility
    ctti
)


