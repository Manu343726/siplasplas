include(cmake/generate_assembly)
include(${CMAKE_SOURCE_DIR}/include/reflection/parser/drlparser.cmake)

function(add_example NAME)
    add_siplasplas_executable(${NAME}
        ${ARGN}
        SOURCES ${NAME}.cpp
        INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/examples"
        NAMESPACE examples
    )
endfunction()

function(add_reflection_example NAME)
    add_example(${NAME} ${ARGN} DEPENDS siplasplas-allocator)
    reflection_target(examples-${NAME})
endfunction()

# Variant examples
add_example(variant          DEPENDS siplasplas-variant)
add_example(gamelike_variant DEPENDS siplasplas-variant)
add_example(messaging        DEPENDS siplasplas-variant)
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
add_reflection_example(reflection)

if(NOT MINGW) # See https://github.com/GueimUCM/siplasplas/issues/16
    # Boost examples
    add_example(coroutine BOOST_COMPONENTS coroutine)
    add_example(coroutine2 BOOST_COMPONENTS coroutine context)

    # Pipeline examples
    add_example(pipeline DEPENDS siplasplas-pipeline)

    if(MSVC)
        set_target_properties(examples-coroutine PROPERTIES LINK_FLAGS /SAFESEH:NO)
        set_target_properties(examples-coroutine2 PROPERTIES LINK_FLAGS /SAFESEH:NO)
        set_target_properties(examples-pipeline PROPERTIES LINK_FLAGS /SAFESEH:NO)
    endif()
endif()

# Utility examples
add_example(meta DEPENDS INTERFACE siplasplas-utility)
