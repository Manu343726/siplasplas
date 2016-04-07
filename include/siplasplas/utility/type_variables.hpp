#ifndef SIPLASPLAS_UTILITY_TYPE_VARIABLES_HPP
#define SIPLASPLAS_UTILITY_TYPE_VARIABLES_HPP

#include "meta.hpp"

namespace cpp
{
    template<typename T>
    using type = meta::identity<T>;

#ifndef _MSC_VER
    template<typename T>
    constexpr type<T> type_var = type<T>{};
#endif
}

#endif // SIPLASPLAS_UTILITY_TYPE_VARIABLES_HPP
