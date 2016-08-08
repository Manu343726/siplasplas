include(cmake/cmakepp)

function(export_target_properties TARGET)
    set(file "${CMAKE_BINARY_DIR}/${TARGET}.json")
    map_new()
    ans(target_properties)
    set(properties
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

    map_set(target_properties LOCATION "@${TARGET}_LOCATION@")

    json_write("${file}.in" target_properties)

    # Write the LOCATION value at build time:
    add_custom_command(TARGET ${TARGET} POST_BUILD 
        COMMAND ${CMAKE_COMMAND} -DTARGET=${TARGET} -DINPUT_DIR="${CMAKE_BINARY_DIR}" -DOUTPUT_DIR="${CMAKE_BINARY_DIR}" -DLOCATION="$<TARGET_FILE_DIR:${TARGET}>/$<TARGET_FILE_NAME:${TARGET}>" -P "${CMAKE_SOURCE_DIR}/cmake/process_exported_target.cmake"
        COMMENT "Exporting ${TARGET} metadata to ${file}..."
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    )
endfunction()
