
add_library(siplasplas_allocator STATIC
    freelist_allocator.cpp
    lifo_allocator.cpp
    lifo_allocator.cpp
    linear_allocator.cpp
)

target_include_directories(siplasplas_allocator
    PUBLIC ${CMAKE_SOURCE_DIR}/include
)

target_compile_options(siplasplas_allocator PRIVATE
    -std=c++14
)
