if(WIN32)
    return()
endif()

include(libclang)

libclang_library(LIBCLANG_LIBRARY)
libclang_include_dir(LIBCLANG_INCLUDE_DIR)
libclang_system_include_dir(LIBCLANG_SYSTEM_INCLUDE_DIR)

if(SIPLASPLAS_VERBOSE_CONFIG)
    message(STATUS "libclang library: ${LIBCLANG_LIBRARY}")
    message(STATUS "libclang include dir: ${LIBCLANG_INCLUDE_DIR}")
    message(STATUS "libclang system include dir: ${LIBCLANG_SYSTEM_INCLUDE_DIR}")
endif()

add_siplasplas_thirdparty(standardese
GIT_REPOSITORY
"https://github.com/Manu343726/standardese"
GIT_TAG
    develop
CMAKE_EXTRA_ARGS
    -DSTANDARDESE_VERBOSE_CONFIG=ON
    -DLIBCLANG_LIBRARY=${LIBCLANG_LIBRARY}
    -DLIBCLANG_INCLUDE_DIR=${LIBCLANG_INCLUDE_DIR}
    -DLIBCLANG_SYSTEM_INCLUDE_DIR=${LIBCLANG_SYSTEM_INCLUDE_DIR}
)

set(STANDARDESE_TOOL "${STANDARDESE_BINARY_DIR}/tool/standardese" CACHE INTERNAL "Path to Standardese tool")

function(configure_standardese)
    cmake_parse_arguments(RUNTOOL
        ""
        "TARGET;ROOT_DIR"
        ""
        ${ARGN}
    )

    if(NOT RUNTOOL_TARGET OR NOT TARGET ${RUNTOOL_TARGET})
        message(FATAL_ERROR "No correct target given (${RUNTOOL_TARGET})")
    endif()
    if(NOT RUNTOOL_ROOT_DIR)
        message(FATAL_ERROR "No root dir to scan for headers given")
    endif()

    file(GLOB_RECURSE headers
        ${RUNTOOL_ROOT_DIR}/*.h
        ${RUNTOOL_ROOT_DIR}/*.hpp
    )

    get_target_include_directories(${RUNTOOL_TARGET} include_dirs)
    clangxx_stdlib_includes(libstdc++ STDLIB_INCLUDES)
    list(APPEND include_dirs ${LIBCLANG_SYSTEM_INCLUDE_DIR} ${STDLIB_INCLUDES})

    if(SIPLASPLAS_VERBOSE_CONFIG)
        message(STATUS "Setting up Standardese tool on ${RUNTOOL_TARGET}:")
        message(STATUS "Path to tool: ${STANDARDESE_TOOL}")

        message(STATUS "Headers:")
        foreach(header ${headers})
            message(STATUS "  ${header}")
        endforeach()

        message(STATUS "Include directories:")
        foreach(include_dir ${include_dirs})
            message(STATUS "  ${include_dir}")
        endforeach()
    endif()

    foreach(include_dir ${include_dirs})
        list(APPEND options -I "${include_dir}")
    endforeach()

    if(headers)
        set(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/doc/standardese/${RUNTOOL_TARGET}")

        if(NOT EXISTS "${OUTPUT_DIRECTORY}")
            file(MAKE_DIRECTORY "${OUTPUT_DIRECTORY}")
        endif()

        set(dependencies standardese)
        if(SIPLASPLAS_STANDARDESE_BUILD_TARGETS_FIRST)
            list(APPEND dependencies ${RUNTOOL_TARGET})
        endif()

        add_custom_target(standardese-${RUNTOOL_TARGET}-doc
            COMMAND ${STANDARDESE_TOOL} ${options} ${headers} ||
                ${CMAKE_COMMAND} -E echo Standardese ${RUNTOOL_TARGET} failed
            COMMAND ${CMAKE_COMMAND} -DSOURCE_DIR="${CMAKE_CURRENT_BINARY_DIR}"
                                     -DOUTPUT_DIR="${OUTPUT_DIRECTORY}"
                                     -P ${CMAKE_SOURCE_DIR}/3rdParty/standardese/copy_output.cmake
            DEPENDS ${dependencies}
            COMMENT "Running Standardese on ${RUNTOOL_TARGET} headers..."
        )

        if(NOT TARGET standardese-doc)
            add_custom_target(standardese-doc
                COMMENT "Building Standadese docs..."
            )
        endif()

        add_dependencies(standardese-doc standardese-${RUNTOOL_TARGET}-doc)
    endif()
endfunction()
