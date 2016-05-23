#include <siplasplas/utility/dynamiclibrary.hpp>
#include <iostream>

extern "C" void f()
{
    std::cout << "f()!" << std::endl;
}

int main()
{
    auto thisBinary = cpp::DynamicLibrary::load("");
    auto main = thisBinary.getSymbol("f");

    auto mainptr = main.get<void(*)()>();
    mainptr();

    std::cout << thisBinary.path() << std::endl;
    std::cout << main.name() << std::endl;
}
