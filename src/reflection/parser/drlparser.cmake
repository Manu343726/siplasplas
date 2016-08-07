include(libclang)
include(pip)

find_package(PythonInterp 2.7 REQUIRED)

set(DRLPARSER_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/DRLParser)
set(DRLPARSER_CODEGEN_TEMPLATE ${CMAKE_CURRENT_LIST_DIR}/templates/reflection_template.hpp)
set(OUTPUT_DIR ${CMAKE_BINARY_DIR}/ouput/reflection)
set(INCLUDE_OUTPUT_DIR ${CMAKE_BINARY_DIR}/ouput/)

if(SIPLASPLAS_INSTALL_DRLPARSER_DEPENDENCIES)
    pip_install_requirements("${CMAKE_SOURCE_DIR}/src/reflection/parser")
    pip_package_version(clang libclang_bindings_version)

    if(libclang_bindings_version)
        # Note that LLVM releases libclang bindings for minor releases only
        # that's why the clang packages are tagged 3.x and we compare against
        # SIPLASPLAS_LIBCLANG_VERSION_MAJOR_MINOR instead of SIPLASPLAS_LIBCLANG_VERSION

        if(NOT libclang_bindings_version VERSION_EQUAL SIPLASPLAS_LIBCLANG_VERSION_MAJOR_MINOR)
            message(FATAL_ERROR "libclang python bindings (clang==${libclang_bindings_version}) do not match required libclang version (${SIPLASPLAS_LIBCLANG_VERSION})")
        endif()
    else()
        message(STATUS "Python bindings (package \"clang\") not found. Installing...")

        pip_install_package(clang ${SIPLASPLAS_LIBCLANG_VERSION_MAJOR_MINOR})
    endif()
endif()

function(configure_siplasplas_reflection TARGET)
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

    # Filter MSVC compile options and use default C++11 options for libclang
    if(MSVC)
        foreach(option ${COMPILE_OPTIONS})
            log("Filtering MSVC compile options: ${option}")

            # TODO: Actual filtering...
        endforeach()

        list(APPEND COMPILE_OPTIONS ${LIBCLANG_EXTRA_COMPILE_OPTIONS})
    endif()

    if(UNIX OR MINGW OR MSVC)
        clangxx_stdlib_includes(libstdc++ STDLIBCPP_INCLUDES)
        libclang_system_include_dir(LIBCLANG_SYSTEM_INCLUDE_DIR)

        foreach(include ${STDLIBCPP_INCLUDES})
            log("Extra stdlibc++ include: ${include}")
        endforeach()
        log("libclang system include dir: ${LIBCLANG_SYSTEM_INCLUDE_DIR}")

        list(APPEND EXTRA_LIBCLANG_INCLUDES ${STDLIBCPP_INCLUDES} ${LIBCLANG_SYSTEM_INCLUDE_DIR})
    endif()

    target_include_directories(${TARGET}
        PUBLIC ${INCLUDE_OUTPUT_DIR}
    )

    log("Processing target ${TARGET}:")
    log("Setting dlrparser hook for target ${TARGET}")
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

    if(SIPLASPLAS_LIBCLANG_LIBRARY)
        log("DRLParser custom libclang file: ${SIPLASPLAS_LIBCLANG_LIBRARY}")
        set(libclang --libclang ${SIPLASPLAS_LIBCLANG_LIBRARY})
    endif()

    if(DRLPARSER_VERBOSE)
        set(verbose --verbose)
    endif()

    if(DRLPARSER_ASTDUMP)
        set(astdump --ast-dump)
    endif()

    string(REGEX REPLACE ";" "," COMPILE_OPTIONS "${COMPILE_OPTIONS}")
    log("libclang compile options: ${COMPILE_OPTIONS}")

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

    add_prebuild_command(
        NAME ${TARGET}_drlparser
        TARGET ${TARGET}
        COMMAND ${PYTHON_EXECUTABLE} ${DRLPARSER_SCRIPT}
            ${options}
        VERBATIM
    )
endfunction()
