
#include <siplasplas/utility/meta.hpp>
#include <siplasplas/utility/function_traits.hpp>
#include <siplasplas/utility/tuple.hpp>
#include <ctti/type_id.hpp>

#include <iostream>

namespace meta = cpp::meta;

int main()
{
    auto lambda = [](int,int){};
    using signature = cpp::function_signature<decltype(lambda)>;

    std::cout << ctti::type_id<decltype(&decltype(lambda)::operator())>().name() << std::endl;
}
