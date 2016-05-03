
add_siplasplas_library(siplasplas-reflection
SOURCES
    dynamic/type.cpp
    dynamic/attributes/attribute.cpp
    dynamic/class.cpp
DEPENDS
    siplasplas-allocator
    siplasplas-utility
    ctti
)

install(DIRECTORY parser DESTINATION bin/reflection-parser
    COMPONENT tools
    PATTERN *.pyc EXCLUDE
)
