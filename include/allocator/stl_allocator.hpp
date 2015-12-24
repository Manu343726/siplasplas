#ifndef SIPLASPLAS_ALLOCATOR_STL_ALLOCATOR_HPP
#define SIPLASPLAS_ALLOCATOR_STL_ALLOCATOR_HPP

#include <memory>

namespace cpp
{
    template<typename T, typename Allocator>
    struct STLAllocator : public Allocator
    {
        using value_type = T;
        using pointer = T*;

        STLAllocator() = default;

        STLAllocator(char* begin, char* end) :
            Allocator{begin, end}
        {}

        STLAllocator(Allocator&& allocator) :
            Allocator{std::move(allocator)}
        {}

        const Allocator& raw_allocator() const
        {
            return *static_cast<const Allocator*>(this);
        }

        Allocator& raw_allocator()
        {
            return *static_cast<Allocator*>(this);
        }

        pointer allocate(std::size_t count)
        {
            char* user_ptr = Allocator::allocate(sizeof(value_type)*count, alignof(value_type));

            if(user_ptr)
            {
                return reinterpret_cast<pointer>(user_ptr);
            }
            else
            {
                throw std::bad_alloc{};
            }
        }

        void deallocate(pointer ptr, std::size_t count)
        {
            Allocator::deallocate(reinterpret_cast<char*>(ptr), sizeof(value_type)*count);
        }

        template<typename... Args>
        void construct(pointer ptr, Args&&... args)
        {
            new(ptr) value_type{std::forward<Args>(args)...};
        }

        void destroy(pointer ptr)
        {
            ptr->~value_type();
        }

        template<typename U>
        struct rebind
        {
            using other = STLAllocator<U, Allocator>;
        };

        friend bool operator==(const STLAllocator& lhs, const STLAllocator& rhs)
        {
            return lhs.raw_allocator() == rhs.raw_allocator(); 
        }
        
        friend bool operator!=(const STLAllocator& lhs, const STLAllocator& rhs)
        {
            return !(lhs == rhs);
        }
    };
}

#endif // SIPLASPLAS_ALLOCATOR_STL_ALLOCATOR_HPP

