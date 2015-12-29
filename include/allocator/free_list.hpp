#ifndef SIPLASPLAS_ALLOCATOR_FREE_LIST_HPP
#define SIPLASPLAS_ALLOCATOR_FREE_LIST_HPP

#include <cassert>
#include <sstream>
#include <iostream>
#include <algorithm>
#include "allocator/allocator_utils.hpp"
#include "allocator/embedded_allocator.hpp"

namespace cpp
{
    class FreeList : EmbeddedAllocator 
    {
    public:
        struct FreeListNode
        {
            FreeListNode* next;
        };
        
        FreeList(void* begin, void* end, std::size_t block_length, std::size_t alignment, std::size_t offset = 0) :
            EmbeddedAllocator{begin, end}
        {    
            block_length = std::max(block_length, sizeof(void*));

            char* aligned_begin = detail::aligned_ptr(EmbeddedAllocator::begin() + offset, alignment);
            
            if(aligned_begin < EmbeddedAllocator::end())
                head = reinterpret_cast<FreeListNode*>(aligned_begin);

            while(aligned_begin < EmbeddedAllocator::end())
            {
                char* next = detail::aligned_ptr(aligned_begin + offset + block_length, alignment);

                if(next < EmbeddedAllocator::end())
                {
                    detail::write_at(aligned_begin, next);
                }
                else
                {
                    detail::write_at(aligned_begin, nullptr);
                }

                aligned_begin = next;
            }
        }

        void* allocate(std::size_t size, std::size_t alignment, std::size_t offset = 0)
        {
            if(head)
            {
                void* user_ptr = head;

                head = head->next;
                return user_ptr;
            }
            else
            {
                return nullptr;
            }
        }

        void deallocate(void* pointer, std::size_t size, std::size_t offset = 0)
        {
            assert(belongs_to_storage(pointer) && "Pointer out of storage");
            
            FreeListNode* node = reinterpret_cast<FreeListNode*>(pointer);
            node->next = head;
            head = node;
        }

        std::string dump() const
        {
            std::ostringstream os;
            FreeListNode* node = head;
            
            os << EmbeddedAllocator::dump();

            os << "free list dump:" << std::endl
               << "===============" << std::endl;

            while(node)
            {
                assert(belongs_to_storage(node) && "Free list corrupted, one node is out of storage");
                os << node << " -> ";
                node = node->next;
            }

            os << "(null)";

            return os.str();
        }

    private:
        FreeListNode* head;
    };
}

#endif // SIPLASPLAS_ALLOCATOR_FREE_LIST_HPP
