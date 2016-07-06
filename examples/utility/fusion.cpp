
#include <siplasplas/utility/fusion.hpp>
#include <ctti/type_id.hpp>
#include <iostream>

int main()
{
    cpp::foreach_type<int, char, bool>([&](auto type)
    {
        using Type = cpp::meta::type_t<decltype(type)>;

        std::cout << "sizeof(" << ctti::type_id<Type>().name() << "): " << sizeof(Type) << std::endl;
    });
}
