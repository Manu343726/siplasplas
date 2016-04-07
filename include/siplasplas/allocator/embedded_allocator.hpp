#ifndef SIPLASPLAS_ALLOCATOR_EMBEDDED_ALLOCATOR_HPP
#define SIPLASPLAS_ALLOCATOR_EMBEDDED_ALLOCATOR_HPP

#include <siplasplas/utility/memory_manip.hpp>
#include <siplasplas/allocator/export.hpp>
#include <string>

namespace cpp
{
    class SIPLASPLAS_ALLOCATOR_EXPORT EmbeddedAllocator
    {
    public:
        EmbeddedAllocator(void* begin, void* end, std::size_t metadata_length = 0);

        char* begin() const;
        char* end() const;
        char* metadata_begin() const;
        char* metadata_end() const;

        std::size_t storage_size() const;
        std::size_t metadata_length() const;

        template<typename T>
        detail::RawReaderWriter<T> metadata(std::size_t begin)
        {
            return{ metadata_begin() + begin };
        }

        template<typename T>
        T metadata(std::size_t begin) const
        {
            return detail::read_at<T>(metadata_begin() + begin);
        }

        bool belongs_to_storage(void* pointer) const;

        std::string dump() const;

    private:
        char* _begin;
    };
}

#endif // SIPLASPLAS_ALLOCATOR_EMBEDDED_ALLOCATOR_HPP
