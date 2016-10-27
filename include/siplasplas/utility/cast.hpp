#ifndef SIPLASPLAS_UTILITY_CAST_HPP
#define SIPLASPLAS_UTILITY_CAST_HPP

namespace cpp
{

/**
 * \ingroup utility
 * \brief Removes the const qualifier from an lvalue reference
 *
 * \param value Const lvalue reference to an object
 *
 * \returns Non const lvalue reference to the given object
 */
template<typename T>
constexpr T& removeConst(const T& value)
{
    return const_cast<T&>(value);
}

/**
 * \ingroup utility
 * \brief Adds a const qualifier to a non const lvalue reference
 *
 * \param value Non const lvalue reference to an object
 *
 * \returns COnst lvalue reference to the given object
 */
template<typename T>
constexpr const T& addConst(const T& value)
{
    return value;
}

}

#endif // SIPLASPLAS_UTILITY_CAST_HPP
