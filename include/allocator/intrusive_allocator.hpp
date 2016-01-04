#ifndef SIPLASPLAS_ALLOCATOR_INTRUSIVE_ALLOCATOR_HPP
#define SIPLASPLAS_ALLOCATOR_INTRUSIVE_ALLOCATOR_HPP

#include "allocator/allocator_utils.hpp"
#include "allocator/embedded_allocator.hpp"

#include <sstream>

namespace cpp {

    /*
     * The Standard Library allocator model does not play very well with stateful allocators. Actually 
     * allocator instances are meaningless (Usually optimized out through EBO) since certain operations
     * of the allocator model do not care too much about allocator state. See rebind for example.
     *
     * This class aims to model STL allocators as "accessors" to memory pools that have their own allocation
     * policy encoded in place. Specifically, given a pool of N bytes it represents a simple growing or fixed 
     * size storage as a pointer to the end of the storage and a pointer to the end of the currently committed 
     * memory (top of the storage). These two pointers are stored in the first bytes of the given pool and then
     * read and modified by the different allocator instances (See the diagram bellow). 
     *
     */

    class IntrusiveAllocator : protected EmbeddedAllocator 
    {
    protected:
        IntrusiveAllocator(void* begin, void* end) :
            EmbeddedAllocator{begin, end, sizeof(void*)}
        {
            /*
             * Pool layout:
             *
             *    metadata         storage
             *  <---------> <------------------>
             * +-----+-----+--------------------+
             * | end | top |         ...        |
             * +-----+-----+--------------------+
             *             ^            ^       ^
             *             |            |       |
             *           begin         top    end
             *
             *             <------------>
             *              storage used
             *
             */

            set_top(EmbeddedAllocator::begin());
        }

        char* top() const 
        {
            return metadata<char*>(0);
        }

        void set_top(void* pointer) 
        {
            metadata<void*>(0) = pointer;
        }

        void commit(std::size_t bytes) 
        {
            set_top(top() + bytes);
        }

        void decommit(std::size_t bytes) 
        {
            set_top(top() - bytes);
        }

        std::size_t bytes() const 
        {
            return top() - begin();
        }

        std::size_t bytes_left() const 
        {
            return end() - top();
        }
        
        bool belongs_to_storage(void* pointer) const
        {
            return begin() <= pointer && pointer < top();
        }

        bool full() const 
        {
            return bytes_left() == 0;
        }
        
        std::string dump() const
        {
            std::ostringstream os;

            os << "intrusive allocator dump:" << std::endl;
            os << "=========================" << std::endl;
            os << " - Memory set begin: " << (void*)begin() << std::endl;
            os << " - Memory set end: " << (void*)end() << std::endl;
            os << " - Top: " << (void*)top() << std::endl;

            return os.str();
        }

    private:
        char* _begin;
    };
}

#endif // SIPLASPLAS_ALLOCATOR_INTRUSIVE_ALLOCATOR_HPP
