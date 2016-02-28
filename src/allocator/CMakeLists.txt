
add_siplasplas_library(siplasplas-allocator STATIC
SOURCES
    detail/track_top_allocator.cpp
    freelist_allocator.cpp
    lifo_allocator.cpp
    lifo_allocator.cpp
    linear_allocator.cpp
    embedded_allocator.cpp
DEPENDS
	PUBLIC siplasplas-utility
)
