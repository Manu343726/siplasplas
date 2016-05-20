add_subdirectory(common)
add_subdirectory(static)
add_subdirectory(dynamic)

add_siplasplas_library(siplasplas-reflection
SOURCES
    .dummy.cpp
DEPENDS
    siplasplas-reflection-common
    siplasplas-reflection-static
    siplasplas-reflection-dynamic
)

install(DIRECTORY parser DESTINATION bin/reflection-parser
    COMPONENT tools
    PATTERN *.pyc EXCLUDE
)
