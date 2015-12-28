#ifndef SIPLASPLAS_ALLOCATOR_INTRUSIVE_ALLOCATOR_HPP
#define SIPLASPLAS_ALLOCATOR_INTRUSIVE_ALLOCATOR_HPP

#include "allocator/allocator_utils.hpp"

namespace cpp
{
	/*
	 * The Standard Library allocator model does not play very well with stateful allocators. Actually 
	 * allocator instances are meaningless (Usually optimized out through EBO) since certain operations
	 * of the allocator model do not care too much about allocator state. See rebind for example.
	 *
	 * This class aims to model STL allocators as "accessors" to memory pools that have their own allocation
	 * policy encoded in place. Specifically, given a pool of N bytes it represents a simple growing or fixed 
	 * size storage as a pointer to the end of the storage and a pointer to the end of the currently committed 
	 * memory (top of the storage). These two pointers are stored in the first bytes of the given pool and then
	 * read and modified by the different allocator instances (See the diagram bellow). 
	 *
	 */

	class IntrusiveAllocator
	{
	protected:
		IntrusiveAllocator(void* begin, void* end) :
			_begin{ reinterpret_cast<char*>(begin) }
		{
			/*
			* Pool layout:
			*
			*    metadata         storage
			*  <---------> <------------------>
			* +-----+-----+--------------------+
			* | end | top |         ...        |
			* +-----+-----+--------------------+
			*             ^            ^       ^
			*             |            |       |
			*           begin         top    end
			*
			*             <------------>
			*              storage used
			*
			*/

			detail::write_at(begin, end);
			detail::write_at(reinterpret_cast<char*>(begin) + sizeof(void*), reinterpret_cast<char*>(begin) + sizeof(void*) * 2);
		}

		const char* begin() const
		{
			return _begin + sizeof(void*) + sizeof(void*);
		}

		char* end() const
		{
			return detail::read_at<char*>(_begin);
		}

		char* top() const
		{
			return detail::read_at<char*>(_begin + sizeof(void*));
		}

		void set_top(void* pointer)
		{
			detail::write_at(_begin + sizeof(void*), pointer);
		}

		void commit(std::size_t bytes)
		{
			detail::write_at(_begin + sizeof(void*), top() + bytes);
		}

		void decommit(std::size_t bytes)
		{
			detail::write_at(_begin + sizeof(void*), top() - bytes);
		}

		std::size_t bytes() const
		{
			return top() - begin();
		}

		std::size_t bytes_left() const
		{
			return end() - top();
		}

		bool full() const
		{
			return bytes_left() == 0;
		}

	private:
		char* _begin;
	};
}

#endif // SIPLASPLAS_ALLOCATOR_INTRUSIVE_ALLOCATOR_HPP
