#include "lifo_allocator.hpp"

#include <siplasplas/utility/exception.hpp>
#include <siplasplas/utility/memory_manip.hpp>

using namespace cpp;

LifoAllocator::LifoAllocator(void* begin, void* end) :
    TrackTopAllocator{begin, end}
{}

void* LifoAllocator::allocate(std::size_t size, std::size_t alignment, std::size_t offset)
{
    assert(offset == 0 && "Cannot do offsetting with this allocator, it stores metadata before user block-address");

    offset = sizeof(offset_t);

    char* user_ptr = detail::aligned_ptr(top() + offset, alignment);
    char* block_end = user_ptr + size;

    if (block_end <= end())
    {
        // Store padding length between current top and returned (user) ptr
        detail::write_before(user_ptr, static_cast<offset_t>(user_ptr - top()));

        set_top(block_end);

        return user_ptr;
    }
    else
    {
        return nullptr;
    }
}

void LifoAllocator::deallocate(void* pointer, std::size_t size)
{
    char* pointer_ = reinterpret_cast<char*>(pointer);

    if (pointer_ < begin() || pointer_ + size > end())
    {
        cpp::Throw<std::runtime_error>(
            "Cannot deallocate, allocated buffer does not belong to storage\n"
            "{}\n"
            "Pointer: {}\n"
            "Block size: {} bytes\n"
            "Block end: {}\n",
            dump(),
            (void*)pointer_,
            size,
            (void*)(pointer_ + size)
        );
    }

    // Restore the top to its position prior to allocation
    char* old_top = pointer_ - detail::read_before<offset_t>(pointer);

    if (pointer_ + size != top())
    {
        cpp::Throw<std::runtime_error>(
            "Cannot deallocate in non-LIFO order!\n"
            "{}\n"
            "Pointer: {}\n"
            "Block size: {} bytes\n"
            "Old top: {}\n"
            "Block end: {}\n",
            dump(),
            (void*)pointer_,
            size,
            (void*)old_top,
            (void*)(pointer_ + size)
        );
    }

    set_top(old_top);
}

std::string LifoAllocator::dump() const
{
    std::ostringstream os;

    os << "LIFO allocator dump:" << std::endl;
    os << "======================" << std::endl;
    os << " - Memory set begin: " << (void*)begin() << std::endl;
    os << " - Memory set end: " << (void*)end() << std::endl;
    os << " - Top: " << (void*)top() << std::endl;

    return os.str();
}
