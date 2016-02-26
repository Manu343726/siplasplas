#include "siplasplas/allocator/detail/track_top_allocator.hpp"

using namespace cpp;

TrackTopAllocator::TrackTopAllocator(void* begin, void* end) :
    EmbeddedAllocator{begin, end, sizeof(void*)}
{
    set_top(EmbeddedAllocator::begin());
}

char* TrackTopAllocator::top() const
{
    return metadata<char*>(0);
}

void TrackTopAllocator::set_top(void* pointer)
{
    metadata<void*>(0) = pointer;
}

void TrackTopAllocator::commit(std::size_t bytes)
{
    set_top(top() + bytes);
}

void TrackTopAllocator::decommit(std::size_t bytes)
{
    set_top(top() - bytes);
}

std::size_t TrackTopAllocator::bytes() const
{
    return top() - begin();
}

std::size_t TrackTopAllocator::bytes_left() const
{
    return end() - top();
}

bool TrackTopAllocator::belongs_to_storage(void* pointer) const
{
    return begin() <= pointer && pointer < top();
}

bool TrackTopAllocator::full() const
{
    return bytes_left() == 0;
}

std::string TrackTopAllocator::dump() const
{
    std::ostringstream os;

    os << "Track top allocator dump:" << std::endl;
    os << "=========================" << std::endl;
    os << " - Memory set begin: " << (void*)begin() << std::endl;
    os << " - Memory set end: " << (void*)end() << std::endl;
    os << " - Top: " << (void*)top() << std::endl;

    return os.str();
}

