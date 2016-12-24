include(export_target)

add_subdirectory(plugininterface)
add_subdirectory(plugin)

add_siplasplas_example_simple(reflection DEPENDS siplasplas-reflection-dynamic nljson-conan)
configure_siplasplas_reflection(examples-reflection)
export_target_properties(examples-reflection)

# See issue #26
if(NOT SIPLASPLAS_CI_BUILD)
    add_siplasplas_example_simple(plugin DEPENDS plugininterface siplasplas-reflection-dynamic)
    configure_siplasplas_reflection(examples-plugin)

    # Force pluginexample to be built first, else the shared library
    # could not be loaded
    add_dependencies(examples-plugin pluginexample)
    target_compile_definitions(examples-plugin PRIVATE PLUGIN_LIBRARY="$<TARGET_FILE:pluginexample>")
endif()
