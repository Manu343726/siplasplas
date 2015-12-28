#ifndef SIPLASPLAS_ALLOCATOR_LIFO_ALLOCATOR_HPP
#define SIPLASPLAS_ALLOCATOR_LIFO_ALLOCATOR_HPP

#include <memory>
#include <string>
#include <sstream>
#include "allocator/allocator_utils.hpp"
#include "allocator/intrusive_allocator.hpp"
#include "utility/throw.hpp"

namespace cpp
{
	class LifoAllocator : protected IntrusiveAllocator
	{
	public:
		using offset_t = std::uint8_t;

		LifoAllocator(void* begin, void* end) :
			IntrusiveAllocator{begin, end}
		{}

		void* allocate(std::size_t size, std::size_t alignment, std::size_t offset = 0)
		{
			if (offset < sizeof(offset_t))
			{
				offset = sizeof(offset_t);
			}

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

		void deallocate(void* pointer, std::size_t size)
		{
			char* pointer_ = reinterpret_cast<char*>(pointer);

			if (pointer_ < begin() || pointer_ + size > end())
			{
				cpp::Throw<std::runtime_error>()
					<< "Cannot deallocate, allocated buffer does not belong to storage" << std::endl
					<< dump()
					<< "Pointer: " << (void*)pointer_ << std::endl
					<< "Block size: " << size << " bytes" << std::endl
					<< "Block end: " << (void*)(pointer_ + size);
			}

			// Restore the top to its position prior to allocation 
			char* old_top = pointer_ - detail::read_before<offset_t>(pointer);

			if (pointer_ + size != top())
			{
				cpp::Throw<std::runtime_error>()
					<< "Cannot deallocate in non-LIFO order!\n"
					<< dump()
					<< "Pointer: " << (void*)pointer_ << std::endl
					<< "Block size: " << size << " bytes" << std::endl
					<< "Old top: " << (void*)old_top << std::endl
					<< "Block end: " << (void*)(pointer_ + size);
			}

			set_top(old_top);
		}

		std::string dump() const
		{
			std::ostringstream os;

			os << "LIFO allocator dump:" << std::endl;
			os << "======================" << std::endl;
			os << " - Memory set begin: " << (void*)begin() << std::endl;
			os << " - Memory set end: " << (void*)end() << std::endl;
			os << " - Top: " << (void*)top() << std::endl;

			return os.str();
		}
	};
}

#endif // SIPLASPLAS_ALLOCATOR_LIFO_ALLOCATOR_HPP
