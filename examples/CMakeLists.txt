include(cmake/generate_assembly)

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

exec_target(NAME coroutine PREFIX example RUN)
include(cmake/boost)
set(Boost_USE_STATIC_LIBS OFF)
bii_find_boost(
	TARGET example_coroutine 
	VERSION 1.60.0 
	COMPONENTS coroutine AUTOCOMPUTE_DEPS
	REQUIRED
)

install(TARGETS example_coroutine DESTINATION siplasplas/examples/coroutine)

include(CPack)
