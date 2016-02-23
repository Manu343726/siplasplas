include(cmake/generate_assembly)

function(add_example NAME)
    add_siplasplas_executable(${NAME}-example
        ${ARGN}
        GROUP examples
    )
endfunction()

add_example(variant
SOURCES
    variant.cpp
)

exec_target(NAME variant PREFIX example RUN)
exec_target(NAME gamelike_variant PREFIX example RUN)
exec_target(NAME messaging PREFIX example RUN)
exec_target(NAME multi_visitor PREFIX example RUN)
exec_target(NAME metatype PREFIX example RUN)
exec_target(NAME linear_allocator PREFIX example RUN)

# LIFO allocator cannot be used with MSVC std::list in debug
# mode since it does extra internal allocations in non-LIFO order
# (For ContainerProxy pointed by debug iterators)
if(MSVC AND (CMAKE_BUILD_TYPE MATCHES "Debug"))
	message(WARNING "Run LIFO allocator example target disabled (See notes in examples/CMakeLists.txt)")
	exec_target(NAME lifo_allocator PREFIX example)
else()
	exec_target(NAME lifo_allocator PREFIX example RUN)
endif()

exec_target(NAME free_list PREFIX example RUN)
exec_target(NAME canary_allocator PREFIX example RUN)

include(${CMAKE_SOURCE_DIR}/include/reflection/parser/drlparser.cmake)
exec_target(NAME reflection PREFIX example RUN)
reflection_target(example_reflection)

target_link_libraries(example_linear_allocator PRIVATE siplasplas_allocator)
target_link_libraries(example_lifo_allocator PRIVATE siplasplas_allocator)
target_link_libraries(example_free_list PRIVATE siplasplas_allocator)
target_link_libraries(example_canary_allocator PRIVATE siplasplas_allocator)
target_link_libraries(example_reflection PRIVATE siplasplas_allocator)


exec_target(NAME coroutine PREFIX example RUN)
include(cmake/boost)
set(Boost_USE_STATIC_LIBS ON)
bii_find_boost(
    VERSION 1.60.0
    REQUIRED
)

target_link_libraries(example_coroutine
    PUBLIC Boost_coroutine_TARGET
)

install(TARGETS example_coroutine DESTINATION siplasplas/examples/coroutine)

include(CPack)
