
add_siplasplas_library(siplasplas-reflection
SOURCES
    detail/metatype.cpp
    attributes/attribute.cpp
DEPENDS
    siplasplas-allocator
    siplasplas-utility
    ctti
)
