include(CMakeParseArguments)
include(GenerateExportHeader)

include(cmake/boost)
include(cmake/list_dependencies)
include(cmake/gmock)
include(cmake/utils.cmake)
include(cmake/ctti.cmake)
include(cmake/install.cmake)


set(NAMESPACE_SEPARATOR "-")

function(add_run_target NAME TARGET_TYPE)
    set(global-run-test-all run${NAMESPACE_SEPARATOR}tests${NAMESPACE_SEPARATOR}all)
    set(global-run-all run${NAMESPACE_SEPARATOR}all)

    if(NOT (TARGET ${global-run-test-all}))
        add_custom_target(${global-run-test-all})
    endif()
    if(NOT (TARGET ${global-run-all}))
        add_custom_target(${global-run-all})
    endif()

    if(TARGET_TYPE MATCHES "UNIT_TEST")
        set(global ${global-run-test-all})
    else()
        set(global ${global-run-all})
    endif()

    if(ARGS_NAMESPACE)
        set(all-target run${NAMESPACE_SEPARATOR}${ARGS_NAMESPACE}${NAMESPACE_SEPARATOR}all)
        set(run-target run${NAMESPACE_SEPARATOR}${NAME})
    else()
        if(TARGET_TYPE MATCHES "UNIT_TEST")
            set(all-target ${global-run-test-all})
        else()
            set(all-target ${global-run-all})
        endif()
    endif()

    set(run-target run${NAMESPACE_SEPARATOR}${NAME})

    # Create custom target to run the executable/test
    add_custom_target(${run-target}
        COMMAND ${NAME} ${ARGS_RUN_ARGS}
        DEPENDS ${NAME}
        COMMENT "Running ${NAME}..."
        VERBATIM
    )

    # Add the run command to the run-all target
    if(NOT ARGS_EXCLUDE_FROM_RUN_ALL)
        if(NOT (TARGET ${all-target}))
            add_custom_target(${all-target})
        endif()

        add_dependencies(${all-target} ${run-target})

        # group run-all target should be run by global run-all
        if(NOT (all-target STREQUAL global))
            add_dependencies(${global} ${all-target})
        endif()
    endif()
endfunction()



function(add_siplasplas_target NAME TARGET_TYPE)
    cmake_parse_arguments(ARGS
        "EXCLUDE_FROM_RUN_ALL;STATIC;SHARED;INSTALL;DEFAULT_TEST_MAIN"
        "BOOST_VERSION;NAMESPACE"
        "SOURCES;INCLUDE_DIRS;COMPILE_OPTIONS;LINK_OPTIONS;DEPENDS;BOOST_COMPONENTS;LINK_LIBS;RUN_ARGS"
        ${ARGN}
    )

    if(ARGS_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "Unparsed arguments: ${ARGS_UNPARSED_ARGUMENTS}")
    endif()

    if(ARGS_NAMESPACE)
        string(REGEX REPLACE ";" "${NAMESPACE_SEPARATOR}" ARGS_NAMESPACE "${ARGS_NAMESPACE}")
        set(NAME ${ARGS_NAMESPACE}${NAMESPACE_SEPARATOR}${NAME})
    endif()

    function(set_flags)
        if(NOT MSVC)
            # gtest needs fileno() function at file gtest/include/gtest/internal/gtest-port.h,
            # but is considered an extension in cygwin GCC setup. Use GNU extensions.
            # See http://stackoverflow.com/questions/18784112/googletest-1-6-with-cygwin-1-7-compile-error-fileno-was-not-declared-in-this
            if(CYGWIN AND (TARGET_TYPE MATCHES "UNIT_TEST"))
                set(STD_CXX gnu++14)
            else()
                set(STD_CXX c++14)
            endif()

            set(common_options -std=${STD_CXX} -Wall -pedantic -ftemplate-depth-1024)
            set(debug_options -O0 -g3)
            set(release_options -O3 -g0)
        endif()

        target_compile_options(${NAME} PRIVATE
            ${common_options}
            $<$<CONFIG:Debug>:${debug_options}>
            $<$<CONFIG:Release>:${release_options}>
            ${ARGS_COMPILE_OPTIONS}
        )

        set_property(TARGET ${NAME} APPEND PROPERTY LINK_FLAGS ${ARGS_LINK_OPTIONS})
    endfunction()

    if(ARGS_BOOST_COMPONENTS)
        if(NOT ARGS_BOOST_VERSION)
            set(ARGS_BOOST_VERSION 1.60.0)
        endif()

        bii_find_boost(VERSION ${ARGS_BOOST_VERSION} REQUIRED)

        foreach(component ${ARGS_BOOST_COMPONENTS})
            list(APPEND boost_targets Boost_${component}_TARGET)
        endforeach()
    endif()

    list(APPEND link_libraries ${ARGS_DEPENDS} ${boost_targets} ${ARGS_LINK_LIBS})

    if(TARGET_TYPE STREQUAL "HEADER_ONLY_LIBRARY")
        add_library(${NAME} INTERFACE)
        # Compute the include directory of the given target
        headerdir_from_sourcetree(current_includedir)
        # Add current include dir so we can just do '#include "foo.hpp"' in foo.cpp
        target_include_directories(${NAME} INTERFACE "${current_includedir}")
        install_siplasplas_headeronly_library(${NAME})
        set(linking INTERFACE)
    elseif(TARGET_TYPE STREQUAL "LIBRARY")
        if(SIPLASPLAS_LIBRARIES_STATIC)
            set(link STATIC)
        else()
            set(link SHARED)
        endif()

        if(ARGS_STATIC)
            set(link STATIC)
        endif()
        if(ARGS_SHARED)
            set(link SHARED)
        endif()

        add_library(${NAME} ${link} ${ARGS_SOURCES})

        # Compute the include directory of the given target
        headerdir_from_sourcetree(current_includedir)
        # Add current include dir so we can just do '#include "foo.hpp"' in foo.cpp
        target_include_directories(${NAME} PUBLIC "${current_includedir}")

        set_flags()
        set(linking PUBLIC)

        sourcetree_relative(current_relative_path)
        generate_export_header(${NAME}
            EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/exports/siplasplas/${current_relative_path}/export.hpp
        )
        target_include_directories(${NAME} PUBLIC ${CMAKE_BINARY_DIR}/exports)

        install_siplasplas_library(${NAME})
    else()
        # Create the executable
        add_executable(${NAME} ${ARGS_SOURCES})

        if(TARGET_TYPE STREQUAL "UNIT_TEST")
            set_target_properties(${NAME} PROPERTIES EXCLUDE_FROM_ALL TRUE)
            if(NOT ARGS_GMOCK_VERSION)
                set(ARGS_GMOCK_VERSION 1.57.0)
            endif()

            install_gtestgmock(VERSION ${ARGS_GMOCK_VERSION})

            # gmock is linked against tests by default,
            # gmock default main if requested in settings
            target_link_libraries(${NAME} PRIVATE
                ${GMOCK_LIB_TARGET}
                $<$<BOOL:ARGS_DEFAULT_TEST_MAIN>:${GMOCK_MAIN_TARGET}>
            )
        endif()

        add_run_target(${NAME} ${TARGET_TYPE})
        set_flags()
        set(linking PRIVATE)
    endif()

    target_link_libraries(${NAME} ${linking} ${link_libraries})
    copy_dll_dependencies(${NAME})

    target_include_directories(${NAME} ${linking}
        ${CMAKE_SOURCE_DIR}/include
        ${ARGS_INCLUDE_DIRS}
    )

    string(REGEX REPLACE "_" " " TARGET_TYPE "${TARGET_TYPE}")
    message(STATUS "SIPLASPLAS ${TARGET_TYPE} ${NAME}")

    function(print_args var)
        string(REGEX REPLACE "ARGS_(.+)" "\\1" varname "${var}")
        string(TOLOWER "${varname}" varname)
        string(REGEX REPLACE "_" " " varname "${varname}")

        if(${var})
            message(":: ${varname}:")

            foreach(elem ${${var}})
                message("  - ${elem}")
            endforeach()
        else()
            message(":: No ${varname}")
        endif()
    endfunction()

    if(SIPLASPLAS_VERBOSE_CONFIG)
        print_args(ARGS_DEPENDS)
        print_args(ARGS_LINK_LIBS)
        print_args(ARGS_BOOST_COMPONENTS)
        print_args(ARGS_SOURCES)
        print_args(ARGS_INCLUDE_DIRS)

        if(link)
            message(":: Link: ${link}")
        endif()
    endif()
endfunction()

function(add_siplasplas_executable NAME)
    add_siplasplas_target("${NAME}" EXECUTABLE ${ARGN})
endfunction()

function(add_siplasplas_library NAME)
    add_siplasplas_target("${NAME}" LIBRARY ${ARGN})
endfunction()

function(add_siplasplas_header_only_library NAME)
    add_siplasplas_target("${NAME}" HEADER_ONLY_LIBRARY ${ARGN})
endfunction()

function(add_siplasplas_test NAME)
    add_siplasplas_target("${NAME}" UNIT_TEST ${ARGN})
endfunction()

include(ExternalProject)

function(add_siplasplas_thirdparty NAME)
    set(options HEADER_ONLY SKIP_CONFIGURE_STEP SKIP_BUILD_STEP)
    set(oneValueArgs URL GIT_REPOSITORY CONFIGURE_COMMAND CMAKE_ARGS EXTRA_CMAKE_ARGS BUILD_COMMAND)
    set(multiValueArgs INCLUDE_DIRS OUTPUT_BINARIES COMPILE_OPTIONS COMPILE_DEFINITIONS EXTERNAL_PROJECT_EXTRA_ARGS)
    cmake_parse_arguments(THIRDPARTY
        "${options}"
        "${oneValueArgs}"
        "${multiValueArgs}"
        ${ARGN}
    )

    set(externalProjectArgs)

    if(THIRDPARTY_URL)
        list(APPEND externalProjectArgs URL "${THIRDPARTY_URL}")
    elseif(THIRDPARTY_GIT_REPOSITORY)
        list(APPEND externalProjectArgs GIT_REPOSITORY "${THIRDPARTY_GIT_REPOSITORY}")
    else()
        message(FATAL_ERROR "No download command for '${NAME}' third party component")
    endif()

    if(NOT THIRDPARTY_HEADER_ONLY)
        if(THIRDPARTY_SKIP_CONFIGURE_STEP)
            set(NO_CONFIG TRUE)
        else()
            if(THIRDPARTY_CONFIGURE_COMMAND)
                list(APPEND externalProjectArgs CONFIGURE_COMMAND ${THIRDPARTY_CONFIGURE_COMMAND})
            elseif(THIRDPARTY_CMAKE_ARGS OR THIRDPARTY_CMAKE_EXTRA_ARGS)
                if(THIRDPARTY_CMAKE_ARGS)
                    # Default bypassed args are overrided by user args
                    list(APPEND externalProjectArgs CMAKE_ARGS ${THIRDPARTY_CMAKE_ARGS})
                else()
                    # Project ompiler and build settings are bypassed to external
                    # project
                    list(APPEND externalProjectArgs CMAKE_ARGS
                        -DCMAKE_BUILD_TYPE=\"${CMAKE_BUILD_TYPE}\"
                        -DCMAKE_C_COMPILER=\"${CMAKE_C_COMPILER}\"
                        -DCMAKE_CXX_COMPILER=\"${CMAKE_CXX_COMPILER}\"
                        -DCMAKE_CXX_FLAGS=\"${CMAKE_CXX_FLAGS};${THIRDPARTY_COMPILE_OPTIONS}\"
                        ${THIRDPARTY_CMAKE_EXTRA_ARGS}
                    )
                endif()
            endif()
        endif()

        if(THIRDPARTY_SKIP_BUILD_STEP)
            set(NO_BUILD TRUE)
        else()
            if(THIRDPARTY_BUILD_COMMAND)
                list(APPEND externalProjectArgs BUILD_COMMAND ${THIRDPARTY_BUILD_COMMAND})
            else()
                if(THIRDPARTY_CONFIGURE_COMMAND)
                    message(FATAL_ERROR "No build command for '${NAME}' third party with custom configure step")
                endif()
            endif()
        endif()
    else()
        set(NO_CONFIG TRUE)
        set(NO_BUILD TRUE)
    endif()

    list(APPEND externalProjectArgs ${THIRDPARTY_EXTERNAL_PROJECT_EXTRA_ARGS})
    set(external ${NAME}-external-project)

    if(THIRDPARTY_GIT_REPOSITORY)
        set(downloaddir "${CMAKE_CURRENT_BINARY_DIR}/THIRDPARTY/${NAME}/src/${external}")
    else()
        set(downloaddir "${CMAKE_CURRENT_BINARY_DIR}/THIRDPARTY/${NAME}/src/${NAME}")
    endif()
    set(repodir "${CMAKE_CURRENT_BINARY_DIR}/THIRDPARTY/${NAME}/src/${NAME}")

    # This is where your realise that CMake sucks. Cannot pass a <ARGUMENT> ""
    # like INSTALL_COMMAND "" from a list. Instead, we write the four
    # cases manually.

    if(NO_CONFIG AND NO_BUILD)
        ExternalProject_Add(${external}
            PREFIX THIRDPARTY/${NAME}
            ${externalProjectArgs}
            # Move the download destination directory to a directory named as the
            # third party, so we can include third party contents as #include <thirdpartyname/header.h>
            UPDATE_COMMAND ${CMAKE_COMMAND} -E copy_directory "${downloaddir}" "${repodir}"
            CONFIGURE_COMMAND ""
            BUILD_COMMAND ""
            INSTALL_COMMAND ""
        )
    elseif(NO_CONFIG)
        ExternalProject_Add(${external}
            PREFIX THIRDPARTY/${NAME}
            ${externalProjectArgs}
            UPDATE_COMMAND ${CMAKE_COMMAND} -E rename "${downloaddir}" "${repodir}"
            CONFIGURE_COMMAND ""
            INSTALL_COMMAND ""
        )
    elseif(NO_BUILD)
        ExternalProject_Add(${external}
            PREFIX THIRDPARTY/${NAME}
            ${externalProjectArgs}
            UPDATE_COMMAND ${CMAKE_COMMAND} -E rename "${downloaddir}" "${repodir}"
            BUILD_COMMAND ""
            INSTALL_COMMAND ""
        )
    else()
        ExternalProject_Add(${external}
            PREFIX THIRDPARTY/${NAME}
            ${externalProjectArgs}
            UPDATE_COMMAND ${CMAKE_COMMAND} -E rename "${downloaddir}" "${repodir}"
            INSTALL_COMMAND ""
        )
    endif()

    ExternalProject_Get_Property(${external} source_dir binary_dir)

    add_library(${NAME} INTERFACE)
    add_dependencies(${NAME} ${external})

    set(includedirs)
    foreach(includedir ${THIRDPARTY_INCLUDE_DIRS})
        list(APPEND includedirs "${repodir}/${includedir}")
    endforeach()
    target_include_directories(${NAME} INTERFACE "${repodir}/.." ${includedirs})

    foreach(binary ${THIRDPARTY_OUTPUT_BINARIES})
        if(SIPLASPLAS_VERBOSE_CONFIG)
            message(STATUS "Binary ${binary} for third party ${NAME}...")
        endif()

        set(importedlib ${NAME}-${binary}-imported-lib)
        add_library(${importedlib} IMPORTED)

        set_target_properties(${importedlib} PROPERTIES
            IMPORTED_LOCATION "${binary_dir}/${binary}"
        )

        target_link_libraries(${NAME} INTERFACE ${importedlib})
    endforeach()

    function(print_args var)
        string(REGEX REPLACE "THIRDPARTY_(.+)" "\\1" varname "${var}")
        string(TOLOWER "${varname}" varname)
        string(REGEX REPLACE "_" " " varname "${varname}")

        if(${var})
            message(":: ${varname}:")

            foreach(elem ${${var}})
                message("  - ${elem}")
            endforeach()
        else()
            message(":: No ${varname}")
        endif()
    endfunction()

    message(STATUS "SIPLASPLAS THIRD PARTY LIBRARY ${NAME}")
    if(SIPLASPLAS_VERBOSE_CONFIG)
        foreach(var ${options} ${oneValueArgs} ${multiValueArgs})
            print_args(THIRDPARTY_${var})
        endforeach()
    endif()
endfunction()

