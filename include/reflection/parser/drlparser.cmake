include(cmake/custom_properties)

find_package(PythonInterp 2.7 REQUIRED)

if(NOT PYTHONINTERP_FOUND)
    message(FATAL_ERROR "Python interpreter not found")
else()
    message(STATUS "Python interpreter found: ${PYTHON_EXECUTABLE} (${PYTHON_VERSION_STRING})")
endif()

set(DRLPARSER_SCRIPT include/reflection/parser/DRLParser)

function(reflection_target TARGET)
    if(NOT (TARGET ${TARGET}))
        message(FATA_ERROR "Target ${TARGET} not found!")
    endif()

    get_target_property(SOURCES ${TARGET} SOURCES)
    get_target_property(INCLUDE_DIRS ${TARGET} INCLUDE_DIRECTORIES)
    
    message(STATUS "[REFLECTION] Processing target ${TARGET}:")

    foreach(source ${SOURCES})
        if(MSVC)
            source_file_has_custom_property(is_sourcegroups_header "${source}" VS_SOURCEGROUPS_HEADER)

            if(is_sourcegroups_header)
                continue()
            endif()
        endif()

        message(STATUS "[REFLECTION]  - ${source}")

        get_filename_component(ext ${source} EXT)
        get_filename_component(name ${source} NAME_WE)
        get_filename_component(path ${source} PATH)
        set(final_source "${path}/${name}.rfl${ext}")

        if(NOT (EXISTS ${final_source}))
            # Create a fake source file so target SOURCES property check doesn't fail
            execute_process(COMMAND ${CMAKE_COMMAND} -E touch ${final_source})
        endif()

        list(APPEND final_sources ${final_source})
        list(APPEND sources ${source})
    endforeach()

    set_target_properties(${TARGET} PROPERTIES SOURCES ${final_sources})

    message(STATUS "[REFLECTION] Setting preprocessor hook for target ${TARGET}")
    add_custom_target(${TARGET}_prebuild)
    add_dependencies(${TARGET} ${TARGET}_prebuild)
    string(REGEX REPLACE ";" " " SOURCES "${SOURCES}")
    string(REGEX REPLACE ";" " " INCLUDE_DIRS "${INCLUDE_DIRS}")

    if(DRLPARSER_IGNORE_DATABASE)
        set(ignore_database --ignore-database)
    endif()

    add_custom_command(
        TARGET ${TARGET}_prebuild POST_BUILD 
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/${DRLPARSER_SCRIPT} -I ${INCLUDE_DIRS} 
            -f ${sources} -s ${CMAKE_SOURCE_DIR} -x *3rdParty* ${ignore_database}
    )    
endfunction()
