#ifndef SIPLASPLAS_SERIALIZATION_PRETTY_PRINT_HPP
#define SIPLASPLAS_SERIALIZATION_PRETTY_PRINT_HPP

#include <siplasplas/serialization/json.hpp>
#include <siplasplas/utility/meta.hpp>
#include <iostream>

template<typename T, typename = cpp::meta::void_t<
    decltype(cpp::json(std::declval<T>()))
>>
std::ostream& operator<<(std::ostream& os, const T& object)
{
    return os << cpp::json(object);
}

template<typename T, typename = cpp::meta::void_t<
    decltype(std::declval<cpp::json>().get<T>())
>>
std::istream& operator>>(std::istream& is, T& object)
{
    cpp::json json;
    is >> json;
    object = json.get<T>();
    return is;
}


#endif // SIPLASPLAS_SERIALIZATION_PRETTY_PRINT_HPP
