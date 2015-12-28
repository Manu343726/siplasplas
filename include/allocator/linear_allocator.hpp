#ifndef SIPLASPLAS_LINEAR_ALLOCATOR_HPP
#define SIPLASPLAS_LINEAR_ALLOCATOR_HPP

#include <memory>
#include <ctti/type_id.hpp>
#include <stdexcept>
#include <string>
#include <sstream>

#include "allocator_utils.hpp"
#include "allocator/intrusive_allocator.hpp"

namespace cpp
{
    class LinearAllocator : IntrusiveAllocator
    {
    public:
        LinearAllocator() = default;

		LinearAllocator(char* begin, char* end) :
			IntrusiveAllocator{ begin, end }
		{}

        /**
         * Allocates a memory block of size <size> aligned to a <alignment> boundary.
         * Optionally, the returned pointer has an offset of <offset> bytes to the beginning
         * of the allocated block (After alignment)
         */
        void* allocate(std::size_t size, std::size_t alignment, std::size_t offset = 0)
        {
            // Take the first address available (_top) plus the offset, then adjust
            // it to the required boundary
            char* user_ptr = detail::aligned_ptr(top() + offset, alignment);
            char* block_end = user_ptr + size;

            if(block_end <= end())
            {
                set_top(block_end);
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
            os << " - Memory set begin: " << (void*)begin() << std::endl;
            os << " - Memory set end: " << (void*)end() << std::endl;
            os << " - Top: " << (void*)top() << std::endl;

            return os.str();
        }

        void deallocate(void* ptr, std::size_t count, std::size_t offset = 0)
        {
            // nop
        }
    };
}

#endif // SIPLASPLAS_LINEAR_ALLOCATOR_HPP
