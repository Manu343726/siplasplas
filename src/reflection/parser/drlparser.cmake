include(libclang)

find_package(PythonInterp 2.7 REQUIRED)

if(NOT PYTHONINTERP_FOUND)
    message(FATAL_ERROR "Python interpreter not found")
else()
    message(STATUS "Python interpreter found: ${PYTHON_EXECUTABLE} (${PYTHON_VERSION_STRING})")
endif()

set(DRLPARSER_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/DRLParser)
set(DRLPARSER_CODEGEN_TEMPLATE ${CMAKE_CURRENT_LIST_DIR}/templates/reflection_template.hpp)
set(OUTPUT_DIR ${CMAKE_BINARY_DIR}/ouput/reflection)
set(INCLUDE_OUTPUT_DIR ${CMAKE_BINARY_DIR}/ouput/)



function(reflection_target TARGET)
    function(log MESSAGE)
        if(SIPLASPLAS_VERBOSE_CONFIG)
            message(STATUS "[REFLECTION LOG] ${MESSAGE}")
        endif()
    endfunction()

    if(NOT (TARGET ${TARGET}))
        message(FATA_ERROR "Target ${TARGET} not found!")
    endif()

    get_target_property(SOURCES ${TARGET} SOURCES)
    get_target_property(COMPILE_OPTIONS ${TARGET} COMPILE_OPTIONS)
    get_target_include_directories(${TARGET} INCLUDE_DIRS)

    if(UNIX OR MINGW)
        clangxx_stdlib_includes(libstdc++ STDLIBCPP_INCLUDES)
        libclang_system_include_dir(LIBCLANG_SYSTEM_INCLUDE_DIR)

        foreach(include ${STDLIBCPP_INCLUDES})
            log("Extra stdlibc++ include: ${include}")
        endforeach()
        log("libclang include dir: ${LIBCLANG_INCLUDE_DIR}")

        list(APPEND EXTRA_LIBCLANG_INCLUDES ${STDLIBCPP_INCLUDES} ${LIBCLANG_SYSTEM_INCLUDE_DIR})
    endif()

    target_include_directories(${TARGET}
        PUBLIC ${INCLUDE_OUTPUT_DIR}
    )

    log("Processing target ${TARGET}:")
    log("Setting preprocessor hook for target ${TARGET}")
    add_custom_target(${TARGET}_prebuild)
    add_dependencies(${TARGET} ${TARGET}_prebuild)
    string(REGEX REPLACE ";" "," SOURCES "${SOURCES}")
    string(REGEX REPLACE ";" "," INCLUDE_DIRS "${INCLUDE_DIRS}")

    set(includes ${INCLUDE_DIRS} ${EXTRA_LIBCLANG_INCLUDES})
    string(REGEX REPLACE ";" "," includes  "${includes}")
    set(includedirs --includedirs "\"${includes}\"")

    if(DRLPARSER_DATABASE)
        log("DRLParser custom database file: ${DRLPARSER_DATABASE}")
        set(database --database ${DRLPARSER_DATABASE})
    else()
        set(database --database ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.json)
    endif()

    if(DRLPARSER_IGNORE_DATABASE)
        log("DRLParser ignore database")
        set(ignore_database --ignore-database)
    endif()

    if(DRLPARSER_LIBCLANG)
        log("DRLParser custom libclang file: ${DRLPARSER_LIBCLANG}")
        set(libclang --libclang ${DRLPARSER_LIBCLANG})
    endif()

    if(DRLPARSER_VERBOSE)
        set(verbose --verbose)
    endif()

    if(DRLPARSER_ASTDUMP)
        set(astdump --ast-dump)
    endif()

    string(REGEX REPLACE ";" "," COMPILE_OPTIONS "${COMPILE_OPTIONS}")

    set(options
        --compile-options "\"${COMPILE_OPTIONS}\""
        --searchdirs "\"${INCLUDE_DIRS}\""
        ${includedirs}
        -s ${CMAKE_SOURCE_DIR}
        -o ${OUTPUT_DIR}
        --blacklist "${CMAKE_BINARY_DIR}"
        ${database}
        ${libclang}
        ${ignore_database}
        ${verbose}
        ${astdump}
        --code-template-file ${DRLPARSER_CODEGEN_TEMPLATE}
    )

    add_custom_command(
        TARGET ${TARGET}_prebuild POST_BUILD
        COMMAND ${PYTHON_EXECUTABLE} ${DRLPARSER_SCRIPT}
            ${options}
        VERBATIM
    )
endfunction()
