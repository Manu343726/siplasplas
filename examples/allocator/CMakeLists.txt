if(MSVC AND (CMAKE_BUILD_TYPE MATCHES "Debug"))
    # LIFO allocator cannot be used with MSVC std::list in debug
    # mode since it does extra internal allocations in non-LIFO order
    # (For ContainerProxy pointed by debug iterators)
    message(WARNING "Run LIFO allocator example target disabled (See notes in examples/CMakeLists.txt)")
    set(exclude_from_run_all EXCLUDE_FROM_RUN_ALL)
endif()
add_siplasplas_example_simple(lifo_allocator     DEPENDS siplasplas-allocator ${exclude_from_run_all})
add_siplasplas_example_simple(linear_allocator   DEPENDS siplasplas-allocator)
add_siplasplas_example_simple(freelist_allocator DEPENDS siplasplas-allocator)
add_siplasplas_example_simple(canary_allocator   DEPENDS siplasplas-allocator)
