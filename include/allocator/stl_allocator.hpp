#ifndef SIPLASPLAS_ALLOCATOR_STL_ALLOCATOR_HPP
#define SIPLASPLAS_ALLOCATOR_STL_ALLOCATOR_HPP

#include <memory>

#include <ctti/type_id.hpp>
#include <sstream>

namespace cpp
{
    template<typename T, typename Allocator>
    struct STLAllocator : public Allocator
    {
        using value_type = T;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;

		using propagate_on_container_move_assignment = std::true_type;

        STLAllocator() = default;
		STLAllocator(const STLAllocator&) = default;
		STLAllocator(STLAllocator&&) = default;

        STLAllocator(char* begin, char* end, std::size_t fence = 0, unsigned char cannary = 0xff) :
            Allocator{begin, end}
        {}

        STLAllocator(Allocator&& allocator, std::size_t fence = 0, unsigned char cannary = 0xff) :
            Allocator{std::move(allocator)}
        {}

        STLAllocator(const Allocator& allocator, std::size_t fence = 0, unsigned char cannary = 0xff) :
            Allocator{allocator}
        {}

		// Rebind-aware ctors

		template<typename U>
		STLAllocator(const STLAllocator<U, Allocator>& alloc) :
			Allocator{ static_cast<const Allocator&>(alloc) }
		{}

		template<typename U>
		STLAllocator(STLAllocator<U, Allocator>&& alloc) :
			Allocator{ std::move( static_cast<const Allocator&>(alloc) ) }
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
			const std::size_t blockSize = sizeof(value_type)*count;
            char* user_ptr = reinterpret_cast<char*>(Allocator::allocate(blockSize, alignof(value_type)));

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
            Allocator::deallocate(ptr, sizeof(value_type)*count);

			std::cout << dump();
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

		std::string dump() const
		{
			std::ostringstream os;
			 
			os << "STLAllocator adapter for " << ctti::type_id<Allocator>().name() << ": \n"
				  " - Type: " << ctti::type_id<T>().name() << " (" << sizeof(T) << " bytes, alignment: " << alignof(T) << ")\n"
			   << Allocator::dump();

			return os.str();
		}
    };

    template<typename T, typename Allocator>
    cpp::STLAllocator<T, std::decay_t<Allocator>> make_stl_allocator(Allocator&& alloc)
    {
        return { std::forward<Allocator>(alloc) };        
    }
}

#endif // SIPLASPLAS_ALLOCATOR_STL_ALLOCATOR_HPP

