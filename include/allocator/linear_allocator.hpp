#ifndef SIPLASPLAS_LINEAR_ALLOCATOR_HPP
#define SIPLASPLAS_LINEAR_ALLOCATOR_HPP

#include <memory>
#include <ctti/type_id.hpp>
#include <stdexcept>
#include <string>
#include <sstream>

#include "allocator_utils.hpp"

namespace cpp
{
    class LinearAllocator
    {
    public:
        LinearAllocator() = default;

        LinearAllocator(char* begin, char* end) :
            _begin{begin}, _end{end},
            _top{_begin}
        {}

        /**
         * Allocates a memory block of size <size> aligned to a <alignment> boundary.
         * Optionally, the returned pointer has an offset of <offset> bytes to the beginning
         * of the allocated block (After alignment)
         */
        char* allocate(std::size_t size, std::size_t alignment, std::size_t offset = 0)
        {
            // Take the first address available (_top) plus the offset, then adjust
            // it to the required boundary
            char* user_ptr = detail::aligned_ptr(_top + offset, alignment);
            char* block_end = user_ptr + size;

            if(block_end <= _end)
            {
                _top = block_end;
                return user_ptr;
            }
            else
            {
                // Out of space
                return nullptr;
            }
        }

        std::string dump() const
        {
            std::ostringstream os;

            os << "Linear allocator dump:" << std::endl;
            os << "======================" << std::endl;
            os << " - Memory set begin: " << (void*)_begin << std::endl;
            os << " - Memory set end: " << (void*)_end << std::endl;
            os << " - Top: " << (void*)_top << std::endl;

            return os.str();
        }

        void deallocate(char* ptr, std::size_t count)
        {
            // nop
        }

    private:
        char* _begin;
        char* _end;
        char* _top;
    };
}

#endif // SIPLASPLAS_LINEAR_ALLOCATOR_HPP
