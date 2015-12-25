
#include <list>
#include <stack>
#include <iostream>

#include "allocator/lifo_allocator.hpp"
#include "allocator/stl_allocator.hpp"

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
    using std::stack<T, List<T>>::stack;

    typename List<T>::allocator_type get_allocator() const
    {
        return std::stack<T, List<T>>::c.get_allocator();
    }
};

int main()
{
    char buffer[1024];
    cpp::LifoAllocator alloc{&buffer[0], &buffer[sizeof(buffer)]};
    List<int> list{cpp::make_stl_allocator<int>(alloc)};

    Stack<int> stack{list};

    for(int i : {0, 1, 2, 3, 4, 5, 6, 7, 8})
    {
        std::cout << "Pushing '" << i << "'" << std::endl;
        std::cout << stack.get_allocator().dump();

        stack.push(i);
    }

    while(!stack.empty())
    {
        std::cout << "Popping '" << stack.top() << "'" << std::endl;
        std::cout << stack.get_allocator().dump();
        stack.pop();
    }

    std::cout << "Final allocator state:" << std::endl
              << stack.get_allocator().dump();
}
