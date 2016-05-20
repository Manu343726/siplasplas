#include "siplasplas/allocator/freelist_allocator.hpp"

#include <cassert>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace cpp;

cpp::detail::RawReaderWriter<FreeListAllocator::FreeListNode*> FreeListAllocator::head() const
{
    return EmbeddedAllocator::metadata<FreeListNode*>(0);
}

cpp::detail::RawReaderWriter<FreeListAllocator::FreeListNode*> FreeListAllocator::head()
{
    return EmbeddedAllocator::metadata<FreeListNode*>(0);
}

FreeListAllocator::FreeListAllocator(void* begin, void* end, std::size_t block_length, std::size_t alignment, std::size_t offset) :
    EmbeddedAllocator{begin, end, sizeof(FreeListNode*)}
{    
    block_length = std::max(block_length, sizeof(void*));

    char* aligned_begin = detail::aligned_ptr(EmbeddedAllocator::begin() + offset, alignment);
    
    if(aligned_begin < EmbeddedAllocator::end())
        head() = reinterpret_cast<FreeListNode*>(aligned_begin);

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

void* FreeListAllocator::allocate(std::size_t size, std::size_t alignment, std::size_t offset)
{
    if(head())
    {
        void* user_ptr = head();

        head() = head().get()->next;
        return user_ptr;
    }
    else
    {
        return nullptr;
    }
}

void FreeListAllocator::deallocate(void* pointer, std::size_t size, std::size_t offset)
{
    assert(belongs_to_storage(pointer) && "Pointer out of storage");
    
    FreeListNode* node = reinterpret_cast<FreeListNode*>(pointer);
    node->next = head();
    head() = node;
}

std::string FreeListAllocator::dump() const
{
    std::ostringstream os;
    FreeListNode* node = head();
    
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
