include(CMakeParseArguments)

include(cmake/boost)

add_library(ctti INTERFACE)
target_include_directories(ctti INTERFACE ${CMAKE_SOURCE_DIR}/3rdParty/ctti/include)
target_compile_definitions(ctti INTERFACE
    $<$<CXX_COMPILER_ID:MSVC>: -DCTTI_STRING_MAX_LENGTH=512>
    $<$<OR:$<CXX_COMPILER_ID:gcc>,$<CXX_COMPILER_ID:clang>>: -DCTTI_STRING_MAX_LENGTH=1024>
)

function(add_siplasplas_target NAME TARGET_TYPE)
    cmake_parse_arguments(ARGS
        "EXCLUDE_FROM_RUN_ALL;STATIC;INSTALL;DEFAULT_TEST_MAIN"
        "BOOST_VERSION;GROUP"
        "SOURCES;DEPENDS;BOOST_COMPONENTS;LINK_LIBS;RUN_ARGS"
        ${ARGN}
    )

    if(TARGET_TYPE MATCHES "LIBRARY")
        if(ARGS_STATIC)
            set(link STATIC)
        else()
            set(link SHARED)
        endif()

        add_library(${NAME} ${link} ${ARGS_SOURCES})
    elseif(TARGET_TYPE MATCHES "HEADER ONLY LIBRARY")
        add_library(${NAME} INTERFACE)
    else()
        # Create the executable
        add_executable(${NAME} ${ARGS_SOURCES})

        if(ARGS_GROUP)
            string(TOLOWER "${ARGS_GROUP}" group)
            set(all-target run-${group}-all)
            set(run-target run-${group}-${NAME})
        else()
            set(all-target run-all)
            set(run-target run-${NAME})
        endif()

        # Create custom target to run the executable/test
        add_custom_target(${run-target}
            COMMAND ${NAME} ${ARGS_RUN_ARGS}
            DEPENDS ${NAME}
            COMMENT "Running ${NAME}..."
            VERBATIM
        )

        # Add the run command to the run-all target
        if(NOT ARGS_EXCLUDE_FROM_RUN_ALL)
            if(TARGET_TYPE MATCHES "UNIT_TEST")
                if(NOT (TARGET run-all-tests))
                    add_custom_target(run-all-tests)
                endif()

                add_dependencies(run-all-tests run-${NAME})
            else()
                if(NOT (TARGET "${all-target}"))
                    add_custom_target("${all-target}")
                endif()

                add_dependencies("${all-target}" "${run-target}")
            endif()
        endif()

        # gmock is linked against tests by default,
        # gmock default main if requested in settings
        if(TARGET_TYPE MATCHES "UNIT_TEST")
            target_link_libraries(${NAME} PRIVATE
                gmock
                $<ARGS_DEFAULT_TEST_MAIN:gmock_main>
            )
        endif()
    endif()

    target_link_libraries(${NAME} ${ARGS_DEPENDS} ${ARGS_LINK_LIBS})
    target_include_directories(${NAME} INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_SOURCE_DIR}/include
    )

    if(NOT MSVC)

        # gtest needs fileno() function at file gtest/include/gtest/internal/gtest-port.h,
        # but is considered an extension in cygwin GCC setup. Use GNU extensions.
        # See http://stackoverflow.com/questions/18784112/googletest-1-6-with-cygwin-1-7-compile-error-fileno-was-not-declared-in-this
        if(CYGWIN AND (TARGET_TYPE MATCHES "UNIT_TEST"))
            set(STD_CXX gnu++14)
        else()
            set(STD_CXX c++14)
        endif()

        set(common_options -std=${STD_CXX} -Wall -pedantic -DCTTI_STRING_MAX_LENGTH=1024 -ftemplate-depth-1024)
        set(debug_options -O0 -g3)
        set(release_options -O3 -g0)
    endif()

    target_compile_options(${NAME} PRIVATE
        ${common_options}
        $<$<CONFIG:Debug>:${debug_options}>
        $<$<CONFIG:Release>:${release_options}>
    )

    if(ARGS_BOOST_COMPONENTS)
        if(NOT ARGS_BOOST_VERSION)
            set(ARGS_BOOST_VERSION 1.60.0)
        endif()

        bii_find_boost(VERSION ${ARGS_BOOST_VERSION} REQUIRED)

        foreach(component ${ARGS_BOOST_COMPONENTS})
            target_link_libraries(${NAME} PUBLIC
                Boost_${component}_TARGET
            )
        endforeach()
    endif()

    message(STATUS "SIPLASPLAS ${TARGET_TYPE} ${NAME}")

    function(print_args var)
        string(REGEX REPLACE "ARGS_(.+)" "\\1" varname "${var}")
        string(TOLOWER "${varname}" varname)
        string(REGEX REPLACE "_" " " varname "${varname}")

        message(":: ${varname}:")

        foreach(elem ${${var}})
            message("  - ${elem}")
        endforeach()
    endfunction()

    if(SIPLASPLAS_VERBOSE_CONFIG)
        print_args(ARGS_DEPENDS)
        print_args(ARGS_LINK_LIBS)
        print_args(ARGS_BOOST_COMPONENTS)
        print_args(ARGS_SOURCES)

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

