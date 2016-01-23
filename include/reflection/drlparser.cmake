

find_package(PythonInterp 2.7 REQUIRED)

if(NOT PYTHONINTERP_FOUND)
    message(FATAL_ERROR "Python interpreter not found")
else()
    message(STATUS "Python interpreter found: ${PYTHON_EXECUTABLE} (${PYTHON_VERSION_STRING})")
endif()

set(DRLPARSER_SCRIPT include/reflection/DRLParser)

function(generate_reflection_code SOURCEFILE)
    get_filename_component(PATH ${SOURCEFILE} ABSOLUTE)

    message(STATUS "[Reflection] Processing file '${SOURCEFILE}'...")

    execute_process(COMMAND ${DRLPARSER_SCRIPT} ${PATH}
        RESULT_VARIABLE result
        OUTPUT_VARIABLE output
        ERROR_VARIABLE error)

    message(STATUS "[Reflection] result: ${result}")

    if(NOT (result EQUAL 0))
        message(STATUS "[Reflection] Error processing ${SOURCEFILE}:")
        message(STATUS " error code: ${result}")
        message(STATUS " stderr: ${error}")
        message(FATAL_ERROR "[Reflection] ABORTED")
    endif()
endfunction()

function(reflection_target TARGET)
    if(NOT (TARGET ${TARGET}))
        message(FATA_ERROR "Target ${TARGET} not found!")
    endif()

    get_target_property(SOURCES ${TARGET} SOURCES)
    get_target_property(INCLUDE_DIRS ${TARGET} INCLUDE_DIRECTORIES)
    
    message(STATUS "[REFLECTION] Processing target ${TARGET}: ${SOURCES}")

    foreach(source ${SOURCES})
        message(STATUS "[REFLECTION] -> Overriding ${source}...")

        get_filename_component(ext ${source} EXT)
        get_filename_component(name ${source} NAME_WE)
        get_filename_component(path ${source} PATH)
        set(final_source "${path}/${name}.rfl${ext}")
        
        list(APPEND final_sources ${final_source})
    endforeach()

    message(STATUS "Final sources: ${final_sources}")
    set_target_properties(${TARGET} PROPERTIES SOURCES ${final_sources})

    add_custom_target(${TARGET}_prebuild)
    add_dependencies(${TARGET} ${TARGET}_prebuild)
    
    message(STATUS "Setting custom pre-build command for target ${TARGET}")
    string(REGEX REPLACE ";" " " SOURCES "${SOURCES}")
    string(REGEX REPLACE ";" " " INCLUDE_DIRS "${INCLUDE_DIRS}")

    add_custom_command(
        TARGET ${TARGET}_prebuild POST_BUILD 
        COMMAND ${CMAKE_SOURCE_DIR}/${DRLPARSER_SCRIPT} -I ${INCLUDE_DIRS} 
            -f ${SOURCES} -s ${CMAKE_SOURCE_DIR} -x *3rdParty*
    )    
endfunction()
