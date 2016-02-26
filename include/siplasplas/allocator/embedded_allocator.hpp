#ifndef SIPLASPLAS_ALLOCATOR_EMBEDDED_ALLOCATOR_HPP
#define SIPLASPLAS_ALLOCATOR_EMBEDDED_ALLOCATOR_HPP

#include "siplasplas/allocator/allocator_utils.hpp"

#include <sstream>

namespace cpp
{
    class EmbeddedAllocator
    {
    public:
        EmbeddedAllocator(void* begin, void* end, std::size_t metadata_length = 0) :
            _begin{reinterpret_cast<char*>(begin)}
        {
            detail::write_at(_begin, end);
            detail::write_at(_begin + sizeof(void*), metadata_length);
        }

        char* end() const
        {
            return detail::read_at<char*>(_begin);
        }

        std::size_t storage_size() const
        {
            return end() - begin();
        }

        std::size_t metadata_length() const
        {
            return detail::read_at<std::size_t>(_begin + sizeof(void*));
        }

        char* metadata_begin() const
        {
            return _begin + sizeof(void*) + sizeof(std::size_t);
        }

        char* metadata_end() const
        {
            return metadata_begin() + metadata_length();
        }

        template<typename T>
        detail::RawReaderWriter<T> metadata(std::size_t begin)
        {
            return { metadata_begin() + begin };
        }

        template<typename T>
        T metadata(std::size_t begin) const
        {
            return detail::read_at<T>(metadata_begin() + begin);
        }

        char* begin() const
        {
            return metadata_end();
        }

        bool belongs_to_storage(void* pointer) const
        {
            return begin() <= pointer && pointer < end();
        }

        std::string dump() const
        {
            std::ostringstream os;

            os << "embedded allocator dump:" << std::endl;
            os << "=========================" << std::endl;
            os << " - Storage begin: " << (void*)begin() << std::endl;
            os << " - Storage set end: " << (void*)end() << std::endl;
            os << " - Metadata length: " << metadata_length() << std::endl;

            return os.str();
        }
    private:
        char* _begin;
    };
}

#endif // SIPLASPLAS_ALLOCATOR_EMBEDDED_ALLOCATOR_HPP
