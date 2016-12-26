include(CMakeParseArguments)
include(GenerateExportHeader)

include(cmake/utils.cmake)
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

    if(SIPLASPLAS_ENABLE_VALGRIND)
        find_program(MEMORYCHECK_COMMAND valgrind REQUIRED)
        set(valgrind_command ${MEMORYCHECK_COMMAND} --leak-check=full --track-origins=yes --trace-children=yes)
    endif()

    if(SIPLASPLAS_DUMP_ERRORS)
        if(NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/error.siplasplas.log")
            file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/error.siplasplas.log" "")
        endif()

        if(NOT WIN32)
            # This is a convoluted way to exit with error after printing the
            # log, needed because of the shit makefiles that CMake generates
            # (Parenthesis are quoted so it is not possible to group boolean ops)
            set(dump_errors
                || bash -c "\\(cat error.siplasplas.log && false\\)"
            )
        endif()
    endif()

    if(TARGET_TYPE MATCHES "UNIT_TEST")
        # Always enable gtest colors
        list(APPEND ARGS_RUN_ARGS --gtest_color=yes)
    endif()

    # Create custom target to run the executable/test
    add_custom_target(${run-target}
        COMMAND ${valgrind_command} $<TARGET_FILE_DIR:${NAME}>/$<TARGET_FILE_NAME:${NAME}> ${ARGS_RUN_ARGS} ${dump_errors}
        DEPENDS ${NAME}
        COMMENT "Running ${NAME}..."
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
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
    else()
        message(STATUS "  Executable ${NAME} excluded from ${all-target}")
    endif()
endfunction()



function(add_siplasplas_target NAME TARGET_TYPE)
    cmake_parse_arguments(ARGS
        "EXCLUDE_FROM_ALL;EXCLUDE_FROM_RUN_ALL;STATIC;SHARED;NO_INSTALL;DEFAULT_TEST_MAIN"
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

            set(common_options -std=${STD_CXX} -Wall -pedantic -ftemplate-depth-1024 -fPIC -ftemplate-backtrace-limit=0)
            set(debug_options -O0 -g3 -p)
            set(release_options -O3 -g0)

            if(TARGET_TYPE STREQUAL "LIBRARY" AND ARGS_SHARED)
                list(APPEND commonoptions -fPIC)
            endif()
        endif()

        target_compile_options(${NAME} PRIVATE
            ${common_options}
            $<$<CONFIG:Debug>:${debug_options}>
            $<$<CONFIG:Release>:${release_options}>
            ${ARGS_COMPILE_OPTIONS}
        )

        if(SIPLASPLAS_ENABLE_ASSERTS)
            target_compile_definitions(${NAME} PRIVATE SIPLASPLAS_ENABLE_ASSERTS)
        endif()

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

    if(ARGS_EXCLUDE_FROM_ALL)
        set(exclude_target_from_all EXCLUDE_FROM_ALL)
    endif()

    if(TARGET_TYPE STREQUAL "HEADER_ONLY_LIBRARY")
        add_library(${NAME} INTERFACE ${exclude_target_from_all})
        # Compute the include directory of the given target
        headerdir_from_sourcetree(current_includedir)
        # Add current include dir so we can just do '#include "foo.hpp"' in foo.cpp
        target_include_directories(${NAME} INTERFACE "${current_includedir}")

        if(NOT ARGS_NO_INSTALL)
            install_siplasplas_headeronly_library(${NAME})
        endif()

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
        add_library(${NAME} ${exclude_target_from_all} ${link} ${ARGS_SOURCES})

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

        if(NOT ARGS_NO_INSTALL)
            install_siplasplas_library(${NAME})
        endif()

    else()
        # Create the executable
        add_executable(${NAME} ${exclude_target_from_all} ${ARGS_SOURCES})

        if(TARGET_TYPE STREQUAL "UNIT_TEST")
            set_target_properties(${NAME} PROPERTIES EXCLUDE_FROM_ALL TRUE)

            if(NOT TARGET googlemock-conan)
                message(FATAL_ERROR "GoogleMock third party should be configured first")
            endif()

            # gmock is linked against tests by default,
            # gmock default main if requested in settings
            target_link_libraries(${NAME} PRIVATE
                googlemock-conan
                $<$<BOOL:ARGS_DEFAULT_TEST_MAIN>:googlemock-conan-gmock_main>
            )
        endif()

        # Examples and tests may have headers right alongside them:
        target_include_directories(${NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
        add_run_target(${NAME} ${TARGET_TYPE})
        set_flags()
        set(linking PRIVATE)
    endif()

    target_link_libraries(${NAME} ${linking} ${link_libraries})

    if(SIPLASPLAS_COPY_DLL_DEPENDENCIES AND (TARGET_TYPE STREQUAL "UNIT_TEST" OR TARGET_TYPE STREQUAL "EXECUTABLE"))
        copy_dll_dependencies(${NAME})
    endif()

    target_include_directories(${NAME} ${linking}
        ${CMAKE_SOURCE_DIR}/include
        ${ARGS_INCLUDE_DIRS}
    )

    if(TARGET_TYPE STREQUAL "LIBRARY" OR TARGET_TYPE STREQUAL "HEADER_ONLY_LIBRARY")
        configure_standardese(TARGET ${NAME} ROOT_DIR "${current_includedir}")
    endif()

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

    # Save useful info for the user
    set(${NAME}_SOURCES ${ARGS_SOURCES} PARENT_SCOPE)
endfunction()

macro(add_siplasplas_executable NAME)
    add_siplasplas_target("${NAME}" EXECUTABLE ${ARGN})
endmacro()

macro(add_siplasplas_library NAME)
    add_siplasplas_target("${NAME}" LIBRARY ${ARGN})
endmacro()

macro(add_siplasplas_header_only_library NAME)
    add_siplasplas_target("${NAME}" HEADER_ONLY_LIBRARY ${ARGN})
endmacro()

macro(add_siplasplas_test NAME)
    add_siplasplas_target("${NAME}" UNIT_TEST NAMESPACE tests ${ARGN})
endmacro()

macro(add_siplasplas_example NAME)
    add_siplasplas_executable("${NAME}" NAMESPACE examples ${ARGN})

    install_siplasplas_example(
        examples-${NAME} SOURCES ${examples-${NAME}_SOURCES}
    )
endmacro()

macro(add_siplasplas_test_simple NAME)
    add_siplasplas_test("${NAME}" SOURCES ${NAME}_test.cpp ${ARGN})
endmacro()

macro(add_siplasplas_example_simple NAME)
    add_siplasplas_example("${NAME}" SOURCES ${NAME}.cpp ${ARGN})
endmacro()
