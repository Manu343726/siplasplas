#ifndef SIPLASPLAS_UTILITY_LEXICAL_CAST_HPP
#define SIPLASPLAS_UTILITY_LEXICAL_CAST_HPP

#include <sstream>

namespace cpp
{

/**
 * \ingroup utility
 * \brief Returns an string representation of a value.
 *
 * This function invokes `operator<<(std::ostream&, const T&)` to get
 * an string representation of a given value.
 *
 * It's a simplified version of the [Boost.Lexical_Cast library](http://www.boost.org/doc/libs/1_55_0/doc/html/boost_lexical_cast.html).
 *
 * \tparam T value type. Must have a valid `operator<<(std::ostream&, T)` overload.
 * \param value Value which is translated to a string.
 *
 * @return A string representation of the given value.
 */
template<typename T>
std::string lexical_cast(const T& value)
{
    std::ostringstream os;

    os << value;

    return os.str();
}

/**
 * \ingroup utility
 * \brief Returns an string representation of a value.
 *
 * This function invokes `operator>>(std::istream&, T&)` to assign a T value
 * from an string representation of it.
 *
 * It's a simplified version of the [Boost.Lexical_Cast library](http://www.boost.org/doc/libs/1_55_0/doc/html/boost_lexical_cast.html).
 *
 * \tparam T Required output value type. Must have a valid `operator>>(std::istream&, T&)` overload.
 * \param value String representation of the value.
 *
 * @return A value of type \p T equivalent to the \p value string representation argument.
 */
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
