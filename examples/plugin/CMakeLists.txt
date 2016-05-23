include(${CMAKE_SOURCE_DIR}/src/reflection/parser/drlparser.cmake)

add_siplasplas_library(pluginexample
SOURCES
    plugin.cpp
    reflectionruntimeloader.cpp
DEPENDS
    siplasplas-reflection
    plugininterface
INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/examples
)

reflection_target(pluginexample)
