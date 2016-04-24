#ifndef SIPLASPLAS_UTILITY_LEXICAL_CAST_HPP
#define SIPLASPLAS_UTILITY_LEXICAL_CAST_HPP

#include <sstream>

namespace cpp
{

template<typename T>
std::string lexical_cast(const T& value)
{
    std::ostringstream os;

    os << value;

    return os.str();
}

template<typename T>
T lexical_cast(const std::string& value)
{
    static_assert(std::is_default_constructible<T>::value,
        "T must be default constructible");

    std::istringstream is{value};
    T object;

    is >> object;

    return object;
}

}

#endif // SIPLASPLAS_UTILITY_LEXICAL_CAST_HPP
