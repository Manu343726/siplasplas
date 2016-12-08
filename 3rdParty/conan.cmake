function(add_conan_library NAME)
    cmake_parse_arguments(
        CONANLIB
        ""
        ""
        "DEPENDS;EXTRA_LIBS"
        ${ARGN}
    )

    string(TOUPPER "${NAME}" upper)
    string(TOLOWER "${NAME}" lower)
    set(target "${lower}-conan")

    message(STATUS "SIPLASPLAS CONAN LIBRARY '${target}'")

    set(CONAN_LIBRARY_DIRS ${CONAN_LIB_DIRS_${upper}})
    set(CONAN_LIBRARIES ${CONAN_LIBS_${upper}})

    add_library(${target} INTERFACE)

    target_include_directories(${target} INTERFACE ${CONAN_INCLUDE_DIRS})

    foreach(lib ${CONAN_LIBRARIES})
        set(libvar "${target}-${lib}")
        find_library(${libvar} ${lib} PATHS ${CONAN_LIBRARY_DIRS})

        if(${libvar})
            target_link_libraries(${target} INTERFACE ${${libvar}})
            message(STATUS " - library found: ${${libvar}}")
        else()
            message(FATAL_ERROR "Library '${lib}' for conan library ${NAME} not found")
        endif()
    endforeach()

    foreach(lib ${CONANLIB_EXTRA_LIBS})
        set(libvar "${target}-extra-${lib}")
        find_library(${libvar} ${lib} PATHS ${CONAN_LIBRARY_DIRS})

        if(${libvar})
            add_library(${target}-${lib} INTERFACE)
            target_link_libraries(${target}-${lib} INTERFACE ${${libvar}})

            message(STATUS " - Extra library found: ${${libvar}}")
            message(STATUS "   (Imported as target '${target}-${lib}')")
        else()
            message(FATAL_ERROR "Extra library '${lib}' for conan library ${NAME} not found")
        endif()
    endforeach()

    if(CONANLIB_DEPENDS)
        target_link_libraries(${target} INTERFACE ${CONANLIB_DEPENDS})
    endif()
endfunction()
