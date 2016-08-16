#include "memory_manip.hpp"
#include <siplasplas/utility/assert.hpp>
#include <siplasplas/utility/logger.hpp>

#include <cassert>

namespace cpp
{

namespace detail
{

const char* aligned_ptr(const char* pointer, std::size_t alignment)
{
    return pointer + missalignment(pointer, alignment);
}

const void* aligned_ptr(const void* pointer, std::size_t alignment)
{
    return aligned_ptr(reinterpret_cast<const char*>(pointer), alignment);
}

char* aligned_ptr(char* pointer, std::size_t alignment)
{
    return const_cast<char*>(aligned_ptr(const_cast<const char*>(pointer), alignment));
}

void* aligned_ptr(void* pointer, std::size_t alignment)
{
    return aligned_ptr(reinterpret_cast<char*>(pointer), alignment);
}

bool is_aligned(const char* pointer, std::size_t alignment)
{
    return reinterpret_cast<std::uintptr_t>(pointer) % alignment == 0;
}

bool is_aligned(const void* pointer, std::size_t alignment)
{
    return is_aligned(reinterpret_cast<const char*>(pointer), alignment);
}

bool is_aligned(char* pointer, std::size_t alignment)
{
    return is_aligned(const_cast<const char*>(pointer), alignment);
}

bool is_aligned(void* pointer, std::size_t alignment)
{
    return is_aligned(reinterpret_cast<char*>(pointer), alignment);
}

std::uintptr_t missalignment(const char* address, std::size_t alignment)
{
    return (alignment - (reinterpret_cast<std::uintptr_t>(address) % alignment)) % alignment;
}

std::uintptr_t missalignment(const void* address, std::size_t alignment)
{
    return missalignment(reinterpret_cast<const char*>(address), alignment);
}

std::uintptr_t missalignment(char* address, std::size_t alignment)
{
    return missalignment(const_cast<const char*>(address), alignment);
}

std::uintptr_t missalignment(void* address, std::size_t alignment)
{
    return missalignment(reinterpret_cast<char*>(address), alignment);
}

void* aligned_malloc(std::size_t size, std::size_t alignment, std::size_t offset)
{
    /*
     *                               block returned by malloc()
     *  <----------------------------------------------------------------------->
     * +---------------+-------------------------+------------------+------------+
     * | unused memory | offset from block begin | user extra space | user block |
     * +---------------+-------------------------+------------------+------------+
     *  <-------------> <-----------------------> <----------------> <---------->
     *   missalignment           1 byte              offset bytes   ^ size bytes
     *                                                              |
     *                                                      pointer returned by
     *                                                      cpp::aligned_malloc()
     */

    const std::size_t blockSize = offset + size + sizeof(AlignedMallocAlingOffset) + (alignment - 1);

    char* oversizedBlock = reinterpret_cast<char*>(std::malloc(blockSize));

    if(oversizedBlock)
    {
        char* alignedPtr = aligned_ptr(oversizedBlock + sizeof(AlignedMallocAlingOffset) + offset, alignment);

        if(alignedPtr + size <= oversizedBlock + blockSize)
        {
#ifndef SIPLASPLAS_UTILITY_ALIGNEDMALLOC_NO_FITBLOCK
            // Realloc the block to use the required block size only if possible
            if(alignedPtr + size != oversizedBlock + blockSize)
            {
                oversizedBlock = reinterpret_cast<char*>(std::realloc(oversizedBlock, (alignedPtr + size) - oversizedBlock));
            }
#endif
            // Store the distance to the oversizedBlock start to be able to free
            // the entire block later (See cpp::aligned_free())
            // Note it is the distance oversizedBlock to alignedPtr assuming offset == 0,
            // the offset should be retrieved to cpp::alignment_free() too.
            char* const alignedPointerWithoutOffset = alignedPtr - offset;
            const std::size_t distanceToBlock = alignedPointerWithoutOffset - oversizedBlock;

            if(distanceToBlock <= std::numeric_limits<AlignedMallocAlingOffset>::max())
            {
                write_before(alignedPtr - offset, static_cast<AlignedMallocAlingOffset>(distanceToBlock));

                SIPLASPLAS_ASSERT_EQ(reinterpret_cast<void*>(oversizedBlock), aligned_malloc_block(alignedPtr, offset))
                .onFailure([=]
                {
                    utility::log().debug("aligned_malloc() data:");
                    utility::log().debug("  size: {}", size);
                    utility::log().debug("  alignment: {}", alignment);
                    utility::log().debug("  offset: {}", offset);
                    utility::log().debug("  oversized block: {}", reinterpret_cast<void*>(oversizedBlock));
                    utility::log().debug("  aligned block:   {}", reinterpret_cast<void*>(alignedPtr));
                    utility::log().debug("  stored distance: {}", distanceToBlock);
                    utility::log().debug("  read distance:   {}", static_cast<std::size_t>(read_before<char>(alignedPtr - offset)));
                    utility::log().debug("  result diff:     {}", reinterpret_cast<char*>(aligned_malloc_block(alignedPtr, offset)) - reinterpret_cast<char*>(oversizedBlock));
                });

                return alignedPtr;
            }
            else
            {
                // The alignment offset cannot be stored
                return nullptr;
            }
        }
        else
        {
            // No enough space, alignment requirement bigger than 16
            std::free(oversizedBlock);
            return nullptr;
        }
    }
    else
    {
        return nullptr;
    }
}

void* aligned_malloc_block(void* pointer, std::size_t offset)
{
    char* pointerWithoutOffset = reinterpret_cast<char*>(pointer) - offset;

    return pointerWithoutOffset - read_before<AlignedMallocAlingOffset>(pointerWithoutOffset);
}

void aligned_free(void* pointer, std::size_t offset)
{
    if(pointer != nullptr)
    {
        std::free(aligned_malloc_block(pointer, offset));
    }
}

}

}
