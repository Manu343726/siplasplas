
#include <siplasplas/allocator/linear_allocator.hpp>
#include <siplasplas/allocator/canary_allocator.hpp>
#include <siplasplas/allocator/stl_allocator.hpp>
#include <siplasplas/utility/printers.hpp>

#include <vector>
#include <iostream>

template<typename T>
using Allocator = cpp::STLAllocator<
    T, 
    cpp::CanaryAllocator<cpp::LinearAllocator>
>;

int main()
{
    char buffer[128] = {};
    std::vector<int, Allocator<int>> v{Allocator<int>{std::begin(buffer), std::end(buffer)}};
    v.reserve(5);

    std::cout << "sizeof(void*): " << sizeof(void*) << " bytes" << std::endl;
    std::cout << "sizeof(std::size_t): " << sizeof(std::size_t) << " bytes" << std::endl;

    for(int i : {1, 2, 3, 4, 5})
    {
        v.push_back(i);
        std::cout << cpp::print_memory(std::begin(buffer), std::end(buffer), 8, '|') << std::endl;
    }

    std::cout << "Element size: " << sizeof(decltype(v)::value_type) << std::endl;

    for(const auto& i : v)
    {
        std::cout << "Elem " << i <<" @" << &i << std::endl;
        std::cout << cpp::print_memory(reinterpret_cast<const char*>(&i) - 4,
                                       reinterpret_cast<const char*>(&i + 1) + 4)
                  << std::endl;
    }
}
