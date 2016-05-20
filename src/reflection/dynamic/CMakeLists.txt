
add_siplasplas_library(siplasplas-reflection-dynamic
SOURCES
    sourceinfo.cpp
    scope.cpp
    entity.cpp
    runtime.cpp
    namespace.cpp
    type.cpp
    attributes/attribute.cpp
    field.cpp
    function.cpp
    class.cpp
    runtimeloader.cpp
DEPENDS
    siplasplas-reflection-common
    siplasplas-reflection-static
    siplasplas-allocator
    siplasplas-utility
    ctti
)


