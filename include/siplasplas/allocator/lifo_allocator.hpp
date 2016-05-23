#ifndef SIPLASPLAS_ALLOCATOR_LIFO_ALLOCATOR_HPP
#define SIPLASPLAS_ALLOCATOR_LIFO_ALLOCATOR_HPP

#include <memory>
#include <string>
#include <sstream>
#include <cassert>
#include "detail/track_top_allocator.hpp"
#include <siplasplas/allocator/export.hpp>

namespace cpp
{
    class SIPLASPLAS_ALLOCATOR_EXPORT LifoAllocator : protected TrackTopAllocator
    {
    public:
        using offset_t = std::uint8_t;

        LifoAllocator(void* begin, void* end);

        void* allocate(std::size_t size, std::size_t alignment, std::size_t offset = 0);
        void deallocate(void* pointer, std::size_t size);

        std::string dump() const;
    };
}

#endif // SIPLASPLAS_ALLOCATOR_LIFO_ALLOCATOR_HPP
