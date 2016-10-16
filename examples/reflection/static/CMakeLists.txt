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
if(RAMMB GREATER 1024 OR SIPLASPLAS_BUILD_CHAISCRIPT_EXAMPLE)
    add_siplasplas_example_simple(chaiscript INCLUDE_DIRS .. DEPENDS chaiscript siplasplas-reflection-static)
    configure_siplasplas_reflection(examples-chaiscript)
endif()

add_siplasplas_example_simple(serialization INCLUDE_DIRS .. DEPENDS siplasplas-reflection-static json4moderncpp)
configure_siplasplas_reflection(examples-serialization)
add_siplasplas_example_simple(enum INCLUDE_DIRS .. DEPENDS siplasplas-reflection-static)
configure_siplasplas_reflection(examples-enum)
add_siplasplas_example_simple(membermatching INCLUDE_DIRS .. DEPENDS siplasplas-reflection-static)
configure_siplasplas_reflection(examples-membermatching)

add_subdirectory(protoserialization)
