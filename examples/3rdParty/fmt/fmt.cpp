#include <ctti/type_id.hpp>
#include <fmt/ostream.h>
#include <iostream>

int main()
{
    std::cout << fmt::format("sizeof({}): {}\n", ctti::type_id<int>().name(), sizeof(int));
}
