if(NOT SIPLASPLAS_ROOT)
    if(SIPLASPLAS_PACKAGE_URL)
        get_filename_component(SIPLASPLAS_PACKAGE "${SIPLASPLAS_PACKAGE_URL}" NAME)
        message(STATUS "Downloading siplasplas package (${SIPLASPLAS_PACKAGE}) from ${SIPLASPLAS_PACKAGE_URL} ...")
        set(SIPLASPLAS_PACKAGE "${CMAKE_SOURCE_DIR}/${SIPLASPLAS_PACKAGE}")

        file(DOWNLOAD "${SIPLASPLAS_PACKAGE_URL}" "${SIPLASPLAS_PACKAGE}" SHOW_PROGRESS)
    endif()

    if(NOT SIPLASPLAS_PACKAGE OR NOT EXISTS "${SIPLASPLAS_PACKAGE}")
        message(FATAL_ERROR "No siplasplas package found")
    endif()

    get_filename_component(SIPLASPLAS_PACKAGE_NAME "${SIPLASPLAS_PACKAGE}" NAME_WE)

    if(NOT EXISTS "${SIPLASPLAS_PACKAGE_NAME}")
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E tar xvf "${SIPLASPLAS_PACKAGE}"
        )
    endif()

    set(SIPLASPLAS_ROOT "${CMAKE_BINARY_DIR}/${SIPLASPLAS_PACKAGE_NAME}" CACHE INTERNAL "")
endif()

if(EXISTS "${SIPLASPLAS_ROOT}")
    list(APPEND CMAKE_MODULE_PATH "${SIPLASPLAS_ROOT}/cmake")
    include(siplasplas)
else()
    message(FATAL_ERROR "The siplasplas root directory (${SIPLASPLAS_ROOT}) does not exist")
endif()
