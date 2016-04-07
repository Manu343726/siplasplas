
add_siplasplas_library(siplasplas-reflection
SOURCES
    detail/metatype.cpp
    attributes/attribute.cpp
DEPENDS
    siplasplas-allocator
    siplasplas-utility
    ctti
)

install(DIRECTORY parser DESTINATION bin/reflection-parser
    COMPONENT tools
    PATTERN *.pyc EXCLUDE
)
