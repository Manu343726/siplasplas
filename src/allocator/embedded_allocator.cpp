#include "embedded_allocator.hpp"

#include <sstream>

using namespace cpp;

EmbeddedAllocator::EmbeddedAllocator(void* begin, void* end, std::size_t metadata_length) :
    _begin{reinterpret_cast<char*>(begin)}
{
    detail::write_at(_begin, end);
    detail::write_at(_begin + sizeof(void*), metadata_length);
}

char* EmbeddedAllocator::end() const
{
    return detail::read_at<char*>(_begin);
}

std::size_t EmbeddedAllocator::storage_size() const
{
    return end() - begin();
}

std::size_t EmbeddedAllocator::metadata_length() const
{
    return detail::read_at<std::size_t>(_begin + sizeof(void*));
}

char* EmbeddedAllocator::metadata_begin() const
{
    return _begin + sizeof(void*) + sizeof(std::size_t);
}

char* EmbeddedAllocator::metadata_end() const
{
    return metadata_begin() + metadata_length();
}

char* EmbeddedAllocator::begin() const
{
    return metadata_end();
}

bool EmbeddedAllocator::belongs_to_storage(void* pointer) const
{
    return begin() <= pointer && pointer < end();
}

std::string EmbeddedAllocator::dump() const
{
    std::ostringstream os;

    os << "embedded allocator dump:" << std::endl;
    os << "=========================" << std::endl;
    os << " - Storage begin: " << (void*)begin() << std::endl;
    os << " - Storage set end: " << (void*)end() << std::endl;
    os << " - Metadata length: " << metadata_length() << std::endl;

    return os.str();
}

