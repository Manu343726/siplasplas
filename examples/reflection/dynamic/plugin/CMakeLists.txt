include(export_target)

add_siplasplas_library(pluginexample NO_INSTALL
SOURCES
    plugin.cpp
    reflectionruntimeloader.cpp
DEPENDS
    siplasplas-reflection-dynamic
    plugininterface
INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/examples/reflection/dynamic
)

configure_siplasplas_reflection(pluginexample)
export_target_properties(pluginexample)
