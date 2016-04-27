include(cmake/generate_assembly)
include(${CMAKE_SOURCE_DIR}/src/reflection/parser/drlparser.cmake)

function(add_example NAME)
    cmake_parse_arguments(
        "EXAMPLE"
        ""
        ""
        "INSTALL_SOURCES"
        ${ARGN}
    )

    add_siplasplas_executable(${NAME}
        ${EXAMPLE_UNPARSED_ARGUMENTS}
        SOURCES ${NAME}.cpp
        INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/examples"
        NAMESPACE examples
    )

    install_siplasplas_example(examples-${NAME}
        SOURCES ${EXAMPLE_INSTALL_SOURCES} ${NAME}.cpp
    )
endfunction()

function(add_reflection_example NAME)
    add_example(${NAME} DEPENDS siplasplas-serialization ${ARGN})
    reflection_target(examples-${NAME})
endfunction()

# Variant examples
add_example(variant          DEPENDS siplasplas-variant)
add_example(gamelike_variant DEPENDS siplasplas-variant)
add_example(messaging        DEPENDS siplasplas-variant INSTALL_SOURCES messaging.hpp)
add_example(multi_visitor    DEPENDS siplasplas-variant)

# Metatype examples
add_example(metatype DEPENDS siplasplas-metatype)

# Allocator examples
if(MSVC AND (CMAKE_BUILD_TYPE MATCHES "Debug"))
    # LIFO allocator cannot be used with MSVC std::list in debug
    # mode since it does extra internal allocations in non-LIFO order
    # (For ContainerProxy pointed by debug iterators)
    message(WARNING "Run LIFO allocator example target disabled (See notes in examples/CMakeLists.txt)")
    set(exclude_from_run_all EXCLUDE_FROM_RUN_ALL)
endif()
add_example(lifo_allocator     DEPENDS siplasplas-allocator ${exclude_from_run_all})
add_example(linear_allocator   DEPENDS siplasplas-allocator)
add_example(freelist_allocator DEPENDS siplasplas-allocator)
add_example(canary_allocator   DEPENDS siplasplas-allocator)

# Reflection examples
add_reflection_example(reflection DEPENDS json4moderncpp INSTALL_SOURCES myclass.hpp)
add_reflection_example(chaiscript DEPENDS chaiscript INSTALL_SOURCES myclass.hpp)

if(NOT MINGW) # See https://github.com/GueimUCM/siplasplas/issues/16
    # Boost examples
    add_example(coroutine BOOST_COMPONENTS coroutine)
    if(MSVC)
        set_target_properties(examples-coroutine PROPERTIES LINK_FLAGS /SAFESEH:NO)
    endif()
endif()

# Meta examples
add_example(meta DEPENDS siplasplas-utility)
add_example(fusion DEPENDS siplasplas-utility)
