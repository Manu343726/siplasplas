
#include "variant/variant.hpp"
#include <iostream>
#include <vector>
#include <unordered_map>

int main()
{
    // So ctti is still constexpr...
    static_assert(ctti::type_id(1) != ctti::type_id('c'), "???");

    using map_t = std::unordered_map<int, cpp::Variant<bool, int, std::string>>;
    using nested_variant_t = cpp::Variant<int, char, map_t>;
    using variant_t = cpp::Variant<map_t, std::vector<int>, std::string, int, float>;

    variant_t v = std::string{"mmm palmera de chocolateeee"};

    auto visitor = cpp::visitor<void>(
            [&](const std::string& str)
            {
                std::cout << "Hello from string visitor! " << str << std::endl;
            },
            [&](int i)
            {
                std::cout << "Hello from int visitor! " << i << std::endl;
            },
            [&](const std::vector<int>& v)
            {
                std::cout << "Hello from vector<int> visitor! ";

                for(int i : v)
                    std::cout << i << " ";
                std::cout << std::endl;
            },
            [&](map_t& map)
            {
                std::cout << "A variant that has a map from int to variant<int,bool,string>" << std::endl;

                map[0] = 1;
                map[1] = true;
                map[2] = std::string{"hello!"};

                auto map_visitor = cpp::visitor<void>(
                        [&](int i) { std::cout << i << std::endl; },
                        [&](bool b) { std::cout << std::boolalpha << b << std::endl; },
                        [&](const std::string& str) { std::cout << str << std::endl; }
                );

                for(const auto& keyValue : map)
                {
                    std::cout << "Key " << keyValue.first << ": ";
                    keyValue.second.visit(map_visitor);
                }
            },
            [&](const auto& value)
            {
                std::cout << "Hey! a generic visitor! (Actually operating on " << ctti::type_id(value).name() << ")" << std::endl;
            }
    );

    v.visit(visitor);

    v = 1;

    v.visit(visitor);

    v = std::vector<int>{1,2,3,4,5,6,7,8};

    v.visit(visitor);

    v = map_t{10};

    v.visit(visitor);

    v = 12.0f;

    v.visit(visitor);
}
