#include "datamodel.hpp"
#include <commmodel.pb.h>
#include <siplasplas/utility/constexpralgorithms.hpp>
#include <siplasplas/utility/meta.hpp>
#include <siplasplas/utility/fusion.hpp>
#include <reflection/examples/reflection/static/protoserialization/datamodel.hpp>
#include <reflection/build/examples/reflection/static/protoserialization/commmodel.pb.h>
#include <iostream>

namespace reflection = cpp::static_reflection;

template<typename Field>
struct SortBySpellingDistance
{
    template<typename Lhs, typename Rhs>
    struct apply
    {
        using type = cpp::meta::bool_<
            cpp::levenshteinDistance(
                Field::SourceInfo::spelling(),
                Lhs::SourceInfo::spelling()
            )
            <=
            cpp::levenshteinDistance(
                Field::SourceInfo::spelling(),
                Rhs::SourceInfo::spelling()
            )
        >;
    };
};

using DataModelFields = reflection::Class<NetworkSettings>::Fields;
using CommModelMethods = reflection::Class<commmodel::NetworkSettings>::Methods;

int main()
{
    std::cout << "Data model class: " << reflection::Class<NetworkSettings>::SourceInfo::fullName() << std::endl;

    cpp::foreach_type<DataModelFields>([](auto type)
    {
        using Type = cpp::meta::type_t<decltype(type)>;

        std::cout << "  Data model field: " << Type::SourceInfo::fullName() << std::endl;
    });

    std::cout << "Comm model class: " << reflection::Class<commmodel::NetworkSettings>::SourceInfo::fullName() << std::endl;

    cpp::foreach_type<CommModelMethods>([](auto type)
    {
        using Type = cpp::meta::type_t<decltype(type)>;

        std::cout << "  Comm model method: " << Type::SourceInfo::fullName() << std::endl;
    });

    std::cout << "Data model -> Comm model mapping:\n";

    cpp::foreach_type<DataModelFields>([](auto type)
    {
        using Field = cpp::meta::type_t<decltype(type)>;
        using NearestCommMethod = cpp::meta::head_t<
            cpp::meta::sort_t<
                CommModelMethods,
                SortBySpellingDistance<Field>
            >
        >;

        std::cout << "  " << Field::SourceInfo::fullName()
                  << " -> "
                  << NearestCommMethod::SourceInfo::fullName()
                  << std::endl;
    });
}
