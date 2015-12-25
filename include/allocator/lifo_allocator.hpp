#ifndef SIPLASPLAS_ALLOCATOR_LIFO_ALLOCATOR_HPP
#define SIPLASPLAS_ALLOCATOR_LIFO_ALLOCATOR_HPP

#include <memory>
#include <string>
#include <sstream>
#include "allocator/allocator_utils.hpp"
#include "utility/throw.hpp"

namespace cpp
{
    class LifoAllocator
    {
    public:
        using offset_t = std::uint16_t;

        LifoAllocator(char* begin, char* end) :
            _begin{begin},
            _end{end},
            _top{_begin}
        {}

        char* allocate(std::size_t size, std::size_t alignment, std::size_t offset = 0)
        {
            if(offset < sizeof(offset_t))
            {
                offset = sizeof(offset_t);
            }

            char* user_ptr = detail::aligned_ptr(_top + offset, alignment); 
            char* block_end = user_ptr + size;
            
            if(block_end <= _end)
            {
                // Store padding length between current top and returned (user) ptr 
                detail::write_before(user_ptr, static_cast<offset_t>(user_ptr - _top));
                
                _top = block_end;

                return user_ptr;
            }
            else
            {
                return nullptr;
            }
        }

        void deallocate(char* pointer, std::size_t size)
        {
            if(pointer < _begin || pointer + size > _end)
            {
                cpp::Throw<std::runtime_error>() 
                    << "Cannot deallocate, allocated buffer does not belong to storage" << std::endl
                    << dump() 
                    << "Pointer: " << (void*)pointer << std::endl
                    << "Block size: " << size << " bytes" << std::endl
                    << "Block end: " << (void*)(pointer + size);
            }

            // Restore the top to its position prior to allocation 
            char* old_top = pointer - detail::read_before<offset_t>(pointer);

            if(pointer + size != _top)
            {
                cpp::Throw<std::runtime_error>()
                   << "Cannot deallocate in non-LIFO order!\n"
                   << dump()
                   << "Pointer: " << (void*)pointer << std::endl
                   << "Block size: " << size << " bytes" << std::endl
                   << "Old top: " << (void*)old_top << std::endl
                   << "Block end: " << (void*)(pointer + size);
            }

            _top = old_top;
        }

        std::string dump() const
        {
            std::ostringstream os;

            os << "LIFO allocator dump:" << std::endl;
            os << "======================" << std::endl;
            os << " - Memory set begin: " << (void*)_begin << std::endl;
            os << " - Memory set end: " << (void*)_end << std::endl;
            os << " - Top: " << (void*)_top << std::endl;

            return os.str();
        }

    private:
        char* _begin;
        char* _end;
        char* _top;
    };
}

#endif // SIPLASPLAS_ALLOCATOR_LIFO_ALLOCATOR_HPP
