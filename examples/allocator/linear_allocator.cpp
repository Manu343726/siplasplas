
#include <vector>
#include <iostream>

#include <siplasplas/allocator/linear_allocator.hpp>
#include <siplasplas/allocator/stl_allocator.hpp>

template<typename T>
using Vector = std::vector<
    T, 
    cpp::STLAllocator<
        T, 
        cpp::LinearAllocator
    >
>;

int main()
{
    char buffer[1024];
    Vector<int> vector{cpp::LinearAllocator{&buffer[0], &buffer[sizeof(buffer)]}};

    for(int i : {0, 1, 2, 3, 4, 5, 6, 7, 8})
    {
        vector.push_back(i);
        std::cout << vector.get_allocator().dump();

        for(int i : vector)
            std::cout << i << " ";

        std::cout << std::endl;
    }
}
