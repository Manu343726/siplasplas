
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
    char buffer[128];
    cpp::FreeList freeList{std::begin(buffer), std::end(buffer), sizeof(int), alignof(int)};

    std::cout << freeList.dump() << std::endl;

    int* a = allocate<int>(freeList);
	*a = 1;
    int* b = allocate<int>(freeList);
	*b = 2;
    deallocate(freeList, a);

    int* c = allocate<int>(freeList);
	*c = 3;
    int* d = allocate<int>(freeList);
	*d = 4;
    int* e = allocate<int>(freeList);
	*e = 5;
    int* f = allocate<int>(freeList);

    deallocate(freeList, d);
    deallocate(freeList, c);
    deallocate(freeList, b);
    deallocate(freeList, f);
    deallocate(freeList, e);
}
