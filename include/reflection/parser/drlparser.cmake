include(cmake/custom_properties)

find_package(PythonInterp 2.7 REQUIRED)

if(NOT PYTHONINTERP_FOUND)
    message(FATAL_ERROR "Python interpreter not found")
else()
    message(STATUS "Python interpreter found: ${PYTHON_EXECUTABLE} (${PYTHON_VERSION_STRING})")
endif()

set(DRLPARSER_SCRIPT include/reflection/parser/DRLParser)
set(OUTPUT_DIR ${CMAKE_BINARY_DIR}/ouput/reflection)
set(INCLUDE_OUTPUT_DIR ${CMAKE_BINARY_DIR}/ouput/)

function(reflection_target TARGET)
    if(NOT (TARGET ${TARGET}))
        message(FATA_ERROR "Target ${TARGET} not found!")
    endif()

    get_target_property(SOURCES ${TARGET} SOURCES)
    get_target_property(INCLUDE_DIRS ${TARGET} INCLUDE_DIRECTORIES)
    get_target_property(INTERFACE_INCLUDE_DIRS ${TARGET} INTERFACE_INCLUDE_DIRECTORIES)
    get_target_property(COMPILE_OPTIONS ${TARGET} COMPILE_OPTIONS)

    if(NOT INTERFACE_INCLUDE_DIRS)
        set(INTERFACE_INCLUDE_DIRS)
    endif()

    target_include_directories(${TARGET}
        PUBLIC ${INCLUDE_OUTPUT_DIR}
    )

    list(APPEND INCLUDE_DIRS ${INTERFACE_INCLUDE_DIRS})
    
    message(STATUS "[REFLECTION] Processing target ${TARGET}:")

    foreach(source ${SOURCES})
        if(MSVC)
            source_file_has_custom_property(is_sourcegroups_header "${source}" VS_SOURCEGROUPS_HEADER)

            if(is_sourcegroups_header)
                list(APPEND headers ${source})
                continue()
            endif()
        endif()

        message(STATUS "[REFLECTION]  - ${source}")
    endforeach()

    message(STATUS "[REFLECTION] Setting preprocessor hook for target ${TARGET}")
    add_custom_target(${TARGET}_prebuild)
    add_dependencies(${TARGET} ${TARGET}_prebuild)
    string(REGEX REPLACE ";" " " SOURCES "${SOURCES}")
    string(REGEX REPLACE ";" " " INCLUDE_DIRS "${INCLUDE_DIRS}")

    if(DRLPARSER_DATABASE)
        message(STATUS "[REFLECTION] DRLParser custom database file: ${DRLPARSER_DATABASE}")
        set(database --database ${DRLPARSER_DATABASE})
    else()
        set(database --database ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.json)
    endif()

    if(DRLPARSER_IGNORE_DATABASE)
        message(STATUS "[REFLECTION] DRLParser ignore database")
        set(ignore_database --ignore-database)
    endif()

    if(DRLPARSER_LIBCLANG)
        message(STATUS "[REFLECTION] DRLParser custom libclang file: ${DRLPARSER_LIBCLANG}")
        set(libclang --libclang ${DRLPARSER_LIBCLANG})
    endif()

    if(DRLPARSER_VERBOSE)
        set(verbose --verbose)
    endif()

    string(REGEX REPLACE ";" " " COMPILE_OPTIONS "${COMPILE_OPTIONS}")

    set(options
        --compile-options \"${COMPILE_OPTIONS}\"
        -I ${INCLUDE_DIRS}
        -s ${CMAKE_SOURCE_DIR}
        -o ${OUTPUT_DIR}
        -x *3rdParty*
        ${database}
        ${libclang}
        ${ignore_database}
        ${verbose}
        --code-template-file ${CMAKE_SOURCE_DIR}/include/reflection/parser/templates/reflection_template.hpp
    )

    add_custom_command(
        TARGET ${TARGET}_prebuild POST_BUILD
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/${DRLPARSER_SCRIPT} ${options}
        VERBATIM
    )
endfunction()
