
#include "allocator/free_list.hpp"
#include <iostream>

template<typename T>
T* allocate(cpp::FreeList& freeList)
{
    T* result = reinterpret_cast<T*>(freeList.allocate(sizeof(T), 1));
    std::cout << "Allocated block @" << result << std::endl;
    std::cout << freeList.dump() << std::endl;
    return result;
}

void deallocate(cpp::FreeList& freeList, void* pointer)
{
    std::cout << "Deallocating block @" << pointer << std::endl;
    freeList.deallocate(pointer, 1);
    std::cout << freeList.dump() << std::endl;
}

int main()
{
    char buffer[32];
    cpp::FreeList freeList{std::begin(buffer), std::end(buffer), sizeof(int), alignof(int)};

    std::cout << freeList.dump() << std::endl;

    int* a = allocate<int>(freeList);
    int* b = allocate<int>(freeList);

    deallocate(freeList, a);

    int* c = allocate<int>(freeList);
    int* d = allocate<int>(freeList);
    int* e = allocate<int>(freeList);
    int* f = allocate<int>(freeList);

    deallocate(freeList, d);
    deallocate(freeList, c);
    deallocate(freeList, b);
}
