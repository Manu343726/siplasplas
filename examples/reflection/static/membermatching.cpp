#include <astexamples.hpp>
#include <siplasplas/utility/meta.hpp>
#include <siplasplas/constexpr/arrayview.hpp>
#include <siplasplas/constexpr/algorithm.hpp>
#include <siplasplas/reflection/static/api.hpp>
#include <siplasplas/utility/fusion.hpp>
#include <iostream>

namespace reflection = cpp::static_reflection;


constexpr cpp::constexp::ConstArrayView<char> method = "foobar";

struct Compare
{
    template<typename Lhs, typename Rhs>
    struct apply
    {
        using type = cpp::meta::bool_<
            cpp::constexp::levenshteinDistance(method, Lhs::SourceInfo::spelling()) >=
            cpp::constexp::levenshteinDistance(method, Rhs::SourceInfo::spelling())
        >;
    };
};

using Methods = typename reflection::Class<ClassWithMultipleMembers>::Methods;
using SortedMethods = cpp::meta::sort_t<
    Methods,
    Compare
>;

int main()
{
    std::cout << "Class: " << reflection::Class<ClassWithMultipleMembers>::SourceInfo::fullName()
        << std::endl;
    std::cout << "Number of methods: " << Methods::size << std::endl;
    std::cout << "Methods:\n";

    cpp::foreach_type<Methods>([](auto type)
    {
        using Type = cpp::meta::type_t<decltype(type)>;

        std::cout << "  " << Type::SourceInfo::fullName() << std::endl;
    });

    std::cout << "Methods (Sorted by distance to \"" << method << "\"):\n";

    cpp::foreach_type<SortedMethods>([](auto type)
    {
        using Type = cpp::meta::type_t<decltype(type)>;

        std::cout << "  " << Type::SourceInfo::fullName() << std::endl;
    });
}
