if(NOT SIPLASPLAS_LIB_DIR)
    message(FATAL_ERROR "Siplasplas library dir not found")
endif()
if(NOT SIPLASPLAS_INCLUDE_DIR)
    message(FATAL_ERROR "Siplasplas include dir not found")
endif()

function(siplasplas_module NAME)
    if(TARGET ${NAME})
        return()
    endif()

    if(SIPLASPLAS_LIBRARIES_STATIC)
        set(libtype STATIC)
    else()
        set(libtype SHARED)
    endif()

    add_library(_${NAME}_imported IMPORTED ${libtype} GLOBAL)

    cmake_parse_arguments(
        MODULE
        "HEADER_ONLY"
        ""
        "DEPENDS"
        ${ARGN}
    )

    if(NOT MODULE_HEADER_ONLY)
        find_library(${NAME}_bin ${NAME} "${SIPLASPLAS_LIB_DIR}")

        if(NOT ${NAME}_bin)
            message(FATAL_ERROR "Siplasplas library '${NAME}' not found in ${SIPLASPLAS_LIB_DIR}")
        else()
            message(STATUS "${NAME} binary found: ${${NAME}_bin}")
        endif()

        set_target_properties(_${NAME}_imported PROPERTIES
            IMPORTED_LOCATION "${${NAME}_bin}"
        )
    endif()

    add_library(${NAME} INTERFACE)
    target_link_libraries(${NAME} INTERFACE _${NAME}_imported)
    target_include_directories(${NAME} INTERFACE "${SIPLASPLAS_INCLUDE_DIR}")

    foreach(dep ${MODULE_DEPENDS})
        if(NOT TARGET ${dep})
            siplasplas_module(${dep})
        endif()

        target_link_libraries(${NAME} INTERFACE ${dep})
    endforeach()
endfunction()

siplasplas_module(siplasplas-logger)
siplasplas_module(siplasplas-utility     DEPENDS siplasplas-logger)
siplasplas_module(siplasplas-allocator   DEPENDS siplasplas-utility)
siplasplas_module(siplasplas-typeerasure DEPENDS siplasplas-utility)
siplasplas_module(siplasplas-reflection-common HEADER_ONLY)
siplasplas_module(siplasplas-reflection-static HEADER_ONLY DEPENDS siplasplas-utility)
siplasplas_module(siplasplas-reflection-dynamic DEPENDS siplasplas-reflection-static siplasplas-typeerasure)
siplasplas_module(siplasplas-signals            DEPENDS siplasplas-utility siplasplas-typeerasure)
siplasplas_module(siplasplas-fswatch            DEPENDS siplasplas-reflection-static)
siplasplas_module(siplasplas-cmake              DEPENDS siplasplas-signals siplasplas-reflection-static)
