#include "memory_manip.hpp"

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

}

}
