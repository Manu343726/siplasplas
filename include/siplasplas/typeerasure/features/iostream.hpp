#ifndef SIPLASPLAS_TYPEERASURE_FEATURES_IOSTREAM_HPP
#define SIPLASPLAS_TYPEERASURE_FEATURES_IOSTREAM_HPP

#include <siplasplas/utility/staticif.hpp>
#include <siplasplas/utility/exception.hpp>
#include <siplasplas/typeerasure/concepts/iostream.hpp>

namespace cpp
{

namespace features
{

class OStreamable
{
public:
    template<typename T>
    static std::ostream& apply(std::ostream& os, const T& value)
    {
        return cpp::staticIf<cpp::concepts::OStreamable<T>::value>([&](auto identity) -> std::ostream&
        {
            return identity(os) << value;
        }).Else([](auto) -> std::ostream&
        {
            throw cpp::exception<std::runtime_error>(
                "{} is not streamable!",
                ctti::type_id<T>().name()
            );
        });
    }
};

}

}

#endif // SIPLASPLAS_TYPEERASURE_FEATURES_IOSTREAM_HPP
