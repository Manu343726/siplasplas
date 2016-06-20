include(cmake/cmakepp)

function(export_target_properties TARGET)
    set(file "${CMAKE_BINARY_DIR}/${TARGET}.json")
    map_new()
    ans(target_properties)
    set(properties
        LOCATION
        DEBUG_LOCATION
        RELEASE_LOCATION
        INCLUDE_DIRECTORIES
        INTERFACE_INCLUDE_DIRECTORIES
        SOURCES
    )

    if(SIPLASPLAS_VERBOSE_CONFIG)
        message(STATUS "Exporting properties of target ${TARGET} to ${file}...")
    endif()

    map_set(target_properties NAME ${TARGET})
    map_set(target_properties SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
    map_set(target_properties BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}")

    foreach(property_name ${properties})
        get_target_property(property ${TARGET} ${property_name})
        map_set(target_properties ${property_name} ${property})
    endforeach()

    json_write("${file}" target_properties)
endfunction()
