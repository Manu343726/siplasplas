include(export_target)

add_subdirectory(plugininterface)
add_subdirectory(plugin)

function(get_ram_mb RESULT)
    if(UNIX)
        execute_process(
            COMMAND grep MemTotal /proc/meminfo
            OUTPUT_VARIABLE stdout
        )

        if(stdout)
            string(REGEX REPLACE "MemTotal:(\ *)([0-9]+)kB" "\\2" totalKilobytes "${stdout}")
            math(EXPR totalMegabytes "${totalKilobytes} / (1024)")
            message(STATUS "Total RAM: ${totalMegabytes}MB")
            set(${RESULT} "${totalMegabytes}" PARENT_SCOPE)
        endif()
    else()
        set(${RESULT} 0 PARENT_SCOPE)
    endif()
endfunction()

get_ram_mb(RAMMB)
if(RAMMB GREATER 1024)
    add_siplasplas_example_simple(chaiscript DEPENDS chaiscript siplasplas-reflection-static INSTALL_SOURCES myclass.hpp)
endif()

add_siplasplas_example_simple(reflection DEPENDS siplasplas-reflection-dynamic json4moderncpp)
export_target_properties(examples-reflection)

# See issue #26
if(NOT SIPLASPLAS_CI_BUILD)
    add_siplasplas_example_simple(plugin DEPENDS plugininterface siplasplas-reflection-dynamic)
    # Force pluginexample to be built first, else the shared library
    # could not be loaded
    add_dependencies(examples-plugin pluginexample)
    target_compile_definitions(examples-plugin PRIVATE PLUGIN_LIBRARY="$<TARGET_FILE:pluginexample>")
endif()
