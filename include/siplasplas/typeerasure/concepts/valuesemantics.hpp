#ifndef SIPLASPLAS_TYPEERASURE_CONCEPTS_VALUESEMANTICS_HPP
#define SIPLASPLAS_TYPEERASURE_CONCEPTS_VALUESEMANTICS_HPP

#include "concept.hpp"
#include <type_traits>

/**
 * \ingroup concepts
 * \defgroup value-semantics
 */

namespace cpp
{

namespace concepts
{

/**
 * \ingroup value-semantics
 * \brief Checks if a type satisfies the [default constructible concept](http://en.cppreference.com/w/cpp/concept/DefaultConstructible)
 *
 *  - `::value`: true if T meets Default Constructible concept. False otherwise
 *  - `::no_except`: true if T is also [no throw default constructible](http://en.cppreference.com/w/cpp/types/is_default_constructible). False otherwise
 *
 *  \tparam T Type to be tested
 */
template<typename T>
class DefaultConstructible : public Concept<
    std::is_default_constructible<T>::value,
    std::is_nothrow_default_constructible<T>::value
> {};


/**
 * \ingroup value-semantics
 * \brief Checks if a type satisfies the [constructible concept](http://en.cppreference.com/w/cpp/concept/Constructible)
 * with the given constructor arguments
 *
 *  - `::value`: true if T meets the Constructible concept. False otherwise
 *  - `::no_except`: true if T is also [no throw constructible](http://en.cppreference.com/w/cpp/types/is_constructible). False otherwise
 *
 *  \tparam T Type to be tested
 *  \tparam Args Constructor arguments types
 */
template<typename T, typename... Args>
class Constructible : public Concept<
    std::is_constructible<T, Args...>::value,
    std::is_nothrow_constructible<T, Args...>::value
> {};


/**
 * \ingroup value-semantics
 * \brief Checks if a type satisfies the [copy constructible concept](http://en.cppreference.com/w/cpp/concept/CopyConstructible)
 *
 *  - `::value`: true if T meets Copy Constructible concept. False otherwise
 *  - `::no_except`: true if T is also [no throw copy constructible](http://en.cppreference.com/w/cpp/types/is_copy_constructible). False otherwise
 *
 *  \tparam T Type to be tested
 */
template<typename T>
class CopyConstructible : public Concept<
    std::is_copy_constructible<T>::value,
    std::is_nothrow_copy_constructible<T>::value
> {};


/**
 * \ingroup value-semantics
 * \brief Checks if a type satisfies the [move constructible concept](http://en.cppreference.com/w/cpp/concept/MoveConstructible)
 *
 *  - `::value`: true if T meets Move  Constructible concept. False otherwise
 *  - `::no_except`: true if T is also [no throw move constructible](http://en.cppreference.com/w/cpp/types/is_move_constructible). False otherwise
 *
 *  \tparam T Type to be tested
 */
template<typename T>
class MoveConstructible : public Concept<
    std::is_move_constructible<T>::value,
    std::is_nothrow_move_constructible<T>::value
> {};


/**
 * \ingroup value-semantics
 * \brief Checks if a type satisfies the [assignable concept](http://en.cppreference.com/w/cpp/concept/Assignable)
 *
 *  - `::value`: true if T meets Assignable from U concept. False otherwise
 *  - `::no_except`: true if T is also [no throw assignable](http://en.cppreference.com/w/cpp/types/is_nothrow_assignable). False otherwise
 *
 *  \tparam T Type of the left side (asigned to) of the asignment operation
 *  \tparam U Type of the right side (asigned from) of the asignment operation
 */
template<typename T, typename U>
class Assignable : public Concept<
    std::is_assignable<T, U>::value,
    std::is_nothrow_assignable<T, U>::value
> {};


/**
 * \ingroup value-semantics
 * \brief Checks if a type satisfies the [copy assignable concept](http://en.cppreference.com/w/cpp/concept/CopyAssignable)
 *
 *  - `::value`: true if T meets Copy Assignable concept. False otherwise
 *  - `::no_except`: true if T is also [no throw copy assignable](http://en.cppreference.com/w/cpp/types/is_copy_assignable). False otherwise
 *
 *  \tparam T Type to be tested
 */
template<typename T>
class CopyAssignable : public Concept<
    std::is_copy_assignable<T>::value,
    std::is_nothrow_copy_assignable<T>::value
> {};


/**
 * \ingroup value-semantics
 * \brief Checks if a type satisfies the [move assignable concept](http://en.cppreference.com/w/cpp/concept/MoveAssignable)
 *
 *  - `::value`: true if T meets Default Constructible concept. False otherwise
 *  - `::no_except`: true if T is also [no throw copy constructible](http://en.cppreference.com/w/cpp/types/is_copy_constructible). False otherwise
 *
 *  \tparam T Type to be tested
 */
template<typename T>
class MoveAssignable : public Concept<
    std::is_move_assignable<T>::value,
    std::is_nothrow_move_assignable<T>::value
> {};


/**
 * \ingroup value-semantics
 * \brief Checks if a type satisfies the [destructible concept](http://en.cppreference.com/w/cpp/concept/Destructible)
 *
 *  - `::value`: true if T meets the Destructible  concept. False otherwise
 *  - `::no_except`: true if T is also [no throw destructible](http://en.cppreference.com/w/cpp/types/is_nothrow_destructible). False otherwise
 *
 *  \tparam T Type to be tested
 */
template<typename T>
class Destructible : public Concept<
    std::is_destructible<T>::value,
    std::is_nothrow_destructible<T>::value
> {};

}

}

#endif // SIPLASPLAS_TYPEERASURE_CONCEPTS_VALUESEMANTICS_HPP
