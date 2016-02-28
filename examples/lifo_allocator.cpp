
#include <list>
#include <stack>
#include <iostream>

#include <siplasplas/allocator/lifo_allocator.hpp>
#include <siplasplas/allocator/linear_allocator.hpp>
#include <siplasplas/allocator/stl_allocator.hpp>

template<typename T>
using List = std::list<
    T, 
    cpp::STLAllocator<
        T, 
        cpp::LifoAllocator
    >
>;

template<typename T>
class Stack : public std::stack<T, List<T>>
{
public:
	using StackBase = std::stack<T, List<T>>;
	using StackBase::StackBase;

    typename List<T>::allocator_type get_allocator() const
    {
        return StackBase::c.get_allocator();
    }

	typename List<T>::const_iterator begin() const
	{
		return StackBase::c.begin();
	}

	typename List<T>::const_iterator end() const
	{
		return StackBase::c.end();
	}
};

int main()
{
	char buffer[1024];

    cpp::LifoAllocator alloc{std::begin(buffer), std::end(buffer)};
	{
		List<int> stack{ cpp::make_stl_allocator<int>(alloc) };

		for (int i : {1, 2, 3, 4})
		{
			std::cout << "Pushing '" << i << "'" << std::endl;

			stack.push_back(i);

			for (auto it = stack.begin(); it != stack.end(); ++it)
				std::cout << *it << " ";
			std::cout << std::endl;
		}

		for (auto it = stack.begin(); it != stack.end(); ++it)
			std::cout << *it << " ";
		std::cout << std::endl;

		while (!stack.empty())
		{
			std::cout << "Popping '" << stack.back() << "'" << std::endl;
			stack.pop_back();
		}
	}

	std::cout << alloc.dump();
}
