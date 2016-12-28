include(libclang)
include(pip)
include(utils)

find_package(PythonInterp 2.7 REQUIRED)

macro(drlparser_setup)
    set(DRLPARSER_SCRIPT "${SIPLASPLAS_DRLPARSER_DIR}/DRLParser")
    set(DRLPARSER_CODEGEN_TEMPLATE "${SIPLASPLAS_DRLPARSER_DIR}/templates/reflection_template.hpp")

    if(NOT SIPLASPLAS_REFLECTION_OUTPUT_DIR)
        set(SIPLASPLAS_REFLECTION_OUTPUT_DIR "${CMAKE_BINARY_DIR}/output")
    endif()
    if(NOT SIPLASPLAS_REFLECTION_OUTPUT_INCLUDE_PREFIX)
        set(SIPLASPLAS_REFLECTION_OUTPUT_INCLUDE_PREFIX "reflection")
    endif()
    set(DSIPLASPLAS_REFLECTION_OUTPUT_DIR_FULLPATH "${SIPLASPLAS_REFLECTION_OUTPUT_DIR}/${SIPLASPLAS_REFLECTION_OUTPUT_INCLUDE_PREFIX}")

    if(NOT EXISTS "${DRLPARSER_SCRIPT}")
        message(FATAL_ERROR "No DRLParser script found in ${SIPLASPLAS_DRLPARSER_DIR}")
    endif()

    if(SIPLASPLAS_INSTALL_DRLPARSER_DEPENDENCIES AND NOT __SIPLASPLAS_DRLPARSER_DEPS_SATISFIED)
        pip_install_requirements("${SIPLASPLAS_DRLPARSER_DIR}")
        pip_package_version(clang libclang_bindings_version)

        if(libclang_bindings_version)
            # Note that LLVM releases libclang bindings for minor releases only
            # that's why the clang packages are tagged 3.x and we compare against
            # SIPLASPLAS_LIBCLANG_VERSION_MAJOR_MINOR instead of SIPLASPLAS_LIBCLANG_VERSION

            if(NOT libclang_bindings_version VERSION_EQUAL SIPLASPLAS_LIBCLANG_VERSION_MAJOR_MINOR_LEGACY)
                message(FATAL_ERROR "libclang python bindings (clang==${libclang_bindings_version}) do not match required libclang version (${SIPLASPLAS_LIBCLANG_VERSION_LEGACY})")
            else()
                message(STATUS "libclang python bindings found")
            endif()
        else()
            message(STATUS "Python bindings (package \"clang\") not found. Installing...")

            pip_install_package(clang ${SIPLASPLAS_LIBCLANG_VERSION_MAJOR_MINOR_LEGACY})
        endif()

        set(__SIPLASPLAS_DRLPARSER_DEPS_SATISFIED TRUE CACHE INTERNAL "")
    endif()
endmacro()

function(configure_siplasplas_reflection TARGET)
    drlparser_setup()

    cmake_parse_arguments(DRLPARSER
        ""
        ""
        "EXCLUDE;BLACKLIST;FILES"
        ${ARGN}
    )

    function(log MESSAGE)
        if(SIPLASPLAS_VERBOSE_CONFIG)
            message(STATUS "[REFLECTION LOG] ${MESSAGE}")
        endif()
    endfunction()

    if(NOT (TARGET ${TARGET}))
        message(FATA_ERROR "Target ${TARGET} not found!")
    else()
        log("Configuring siplasplas reflection for target '${TARGET}'")
    endif()

    get_target_property(SOURCES ${TARGET} SOURCES)
    get_target_include_directories(${TARGET} INCLUDE_DIRS)
    get_target_compile_options(${TARGET} COMPILE_OPTIONS)

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

        list(APPEND EXTRA_LIBCLANG_INCLUDES ${STDLIBCPP_INCLUDES} ${SIPLASPLAS_LIBCLANG_SYSTEM_INCLUDE_DIR_LEGACY})
    endif()

    target_include_directories(${TARGET}
        PUBLIC "${SIPLASPLAS_REFLECTION_OUTPUT_DIR}"
        PUBLIC "${SIPLASPLAS_INCLUDE_DIR}"
    )
    target_compile_definitions(${TARGET} PUBLIC
        -DSIPLASPLAS_REFLECTION_OUTPUT_DIR="${SIPLASPLAS_REFLECTION_OUTPUT_DIR}"
        -DSIPLASPLAS_REFLECTION_OUTPUT_INCLUDE_PREFIX="${SIPLASPLAS_REFLECTION_OUTPUT_INCLUDE_PREFIX}"
        -DSIPLASPLAS_REFLECTION_OUTPUT_DIR_FULLPATH="${DSIPLASPLAS_REFLECTION_OUTPUT_DIR_FULLPATH}"
    )

    log("Processing target ${TARGET}:")
    log("Setting dlrparser hook for target ${TARGET}")
    string(REGEX REPLACE ";" "," SOURCES "${SOURCES}")
    string(REGEX REPLACE ";" "," INCLUDE_DIRS "${INCLUDE_DIRS}")

    set(includes ${INCLUDE_DIRS} ${EXTRA_LIBCLANG_INCLUDES})
    list(REMOVE_DUPLICATES includes)
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

    if(SIPLASPLAS_LIBCLANG_LIBRARY_LEGACY)
        log("DRLParser custom libclang file: ${SIPLASPLAS_LIBCLANG_LIBRARY_LEGACY}")
        set(libclang --libclang ${SIPLASPLAS_LIBCLANG_LIBRARY_LEGACY})
    endif()

    if(DRLPARSER_VERBOSE OR SIPLASPLAS_VERBOSE_DRLPARSER)
        set(verbose --verbose)
    endif()

    if(DRLPARSER_ASTDUMP)
        set(astdump --ast-dump)
    endif()

    if(DRLPARSER_EXCLUDE)
        string(REGEX REPLACE ";" "," exclude_globs "${DRLPARSER_EXCLUDE}")
        set(exclude --exclude "\"${exclude_globs}\"")
    endif()

    if(DRLPARSER_FILES)
        string(REGEX REPLACE ";" "," files "${DRLPARSER_FILES}")
        set(files --files "\"${files}\"")
    endif()

    list(APPEND DRLPARSER_BLACKLIST "${CMAKE_BINARY_DIR}")
    string(REGEX REPLACE ";" "," blacklist_dirs "${DRLPARSER_BLACKLIST}")
    set(blacklist --blacklist "\"${blacklist_dirs}\"")

    string(REGEX REPLACE ";" "," COMPILE_OPTIONS "${COMPILE_OPTIONS}")
    log("libclang compile options: ${COMPILE_OPTIONS}")

    set(options
        --compile-options "\"${COMPILE_OPTIONS}\""
        --searchdirs "\"${INCLUDE_DIRS}\""
        ${files}
        ${includedirs}
        -s ${CMAKE_SOURCE_DIR}
        -o "${DSIPLASPLAS_REFLECTION_OUTPUT_DIR_FULLPATH}"
        ${exclude}
        ${blacklist}
        ${database}
        ${libclang}
        ${ignore_database}
        ${verbose}
        ${astdump}
        --code-template-file ${DRLPARSER_CODEGEN_TEMPLATE}
    )

    set(command ${DRLPARSER_SCRIPT} ${options})
    log("DRLParser command: ${command}")

    if(DRLPARSER_FILES)
        # If user specified some files must be explicitly parsed,
        # wait for those files (Can be generated files) first
        add_custom_target(${TARGET}_drlparser_wait_for_files
        DEPENDS
            ${DRLPARSER_FILES}
        )

        set(waitForFiles DEPENDS ${TARGET}_drlparser_wait_for_files)
    endif()

    add_prebuild_command(
        NAME ${TARGET}_drlparser
        TARGET ${TARGET}
        COMMAND ${command}
        VERBATIM
        WORKING_DIRECTORY "${SIPLASPLAS_DRLPARSER_DIR}"
        COMMENT "Running siplasplas reflection parser for ${TARGET}"
        ${waitForFiles}
    )
endfunction()
