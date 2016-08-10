#ifndef SIPLASPLAS_TYPEERASURE_CONCEPTS_VALUESEMANTICS_HPP
#define SIPLASPLAS_TYPEERASURE_CONCEPTS_VALUESEMANTICS_HPP

#include "concept.hpp"
#include <type_traits>

namespace cpp
{

namespace concepts
{

template<typename T>
class DefaultConstructible : public Concept<
    std::is_default_constructible<T>::value,
    std::is_nothrow_default_constructible<T>::value
> {};


template<typename T, typename... Args>
class Constructible : public Concept<
    std::is_constructible<T, Args...>::value,
    std::is_nothrow_constructible<T, Args...>::value
> {};


template<typename T>
class CopyConstructible : public Concept<
    std::is_copy_constructible<T>::value,
    std::is_nothrow_copy_constructible<T>::value
> {};


template<typename T>
class MoveConstructible : public Concept<
    std::is_move_constructible<T>::value,
    std::is_nothrow_move_constructible<T>::value
> {};


template<typename T, typename U>
class Assignable : public Concept<
    std::is_assignable<T, U>::value,
    std::is_nothrow_assignable<T, U>::value
> {};


template<typename T>
class CopyAssignable : public Concept<
    std::is_copy_assignable<T>::value,
    std::is_nothrow_copy_assignable<T>::value
> {};


template<typename T>
class MoveAssignable : public Concept<
    std::is_move_assignable<T>::value,
    std::is_nothrow_move_assignable<T>::value
> {};


template<typename T>
class Destructible : public Concept<
    std::is_destructible<T>::value,
    std::is_nothrow_destructible<T>::value
> {};

}

}

#endif // SIPLASPLAS_TYPEERASURE_CONCEPTS_VALUESEMANTICS_HPP
