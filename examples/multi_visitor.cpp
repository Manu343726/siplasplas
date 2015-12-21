
#include <iostream>
#include <string>

#include "variant/variant.hpp"
#include "variant/multi_visitor.hpp"

using Variant = cpp::Variant<
    bool, float, std::string
>;

auto visitor = cpp::multi_visitor<void>(
    [](const auto& a, const auto& b, const auto& c)
    {
        std::cout << "Three parameters:\n"
                  << "a: " << a << " (" << ctti::type_id(a).name() << ')' << std::endl
                  << "b: " << b << " (" << ctti::type_id(b).name() << ')' << std::endl
                  << "c: " << c << " (" << ctti::type_id(c).name() << ')' << std::endl;
    }
);

using namespace std::string_literals;

template<typename...>
using void_t = void;

template<typename Lhs, typename Rhs, typename = void>
struct comparer
{
    bool operator()(const Lhs& lhs, const Rhs& rhs)
    {
        return false;
    }
};

template<typename Lhs, typename Rhs>
struct comparer<Lhs, Rhs, void_t<decltype(std::declval<Lhs>() == std::declval<Rhs>())>>
{
    bool operator()(const Lhs& lhs, const Rhs& rhs)
    {
        return lhs == rhs;
    }
};

namespace cpp
{
	template<typename... Ts>
	bool operator==(const cpp::Variant<Ts...>& lhs, const cpp::Variant<Ts...>& rhs)
	{
		return cpp::multi_visitor<bool>(
			[](const auto& lhs, const auto& rhs)            
			{
				return comparer<decltype(lhs), decltype(rhs)>()(lhs, rhs);
			}
		)(lhs, rhs);
	}
}

int main()
{
    Variant a{true}, b{1.3f}, c{"hola!"s};
    bool equal = a == a;
    bool not_equal = a != b;

    std::cout << std::boolalpha;
    std::cout << equal  << std::endl;
    std::cout << not_equal << std::endl;

    visitor(a, b, c);
}
