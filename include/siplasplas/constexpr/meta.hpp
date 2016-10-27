#ifndef SIPLASPLAS_CONSTEXPR_META_HPP
#define SIPLASPLAS_CONSTEXPR_META_HPP

#include "arrayview.hpp"
#include "string.hpp"
#include <siplasplas/utility/meta.hpp>
#include <ostream>

namespace cpp
{

namespace constexp
{

struct DummyValueToType {};

template<typename Seq, typename TypeToValue>
class SequenceToArray;

template<template<typename...> class Seq, typename Head, typename... Tail, typename TypeToValue>
class SequenceToArray<Seq<Head, Tail...>, TypeToValue>
{
public:
    using value_type = typename TypeToValue::value_type;

    constexpr SequenceToArray() = default;

    static constexpr const value_type& get(std::size_t i)
    {
        return array[i];
    }

    static constexpr ConstArrayView<value_type> get()
    {
        return {Bounds::begin(), Bounds::size()};
    }

    struct Bounds
    {
        static constexpr std::size_t size()
        {
            return sizeof...(Tail) + 1;
        }

        static constexpr const value_type* begin()
        {
            return array;
        }

        static constexpr const value_type* end()
        {
            return array + size();
        }

        static constexpr const value_type* const cbegin()
        {
            return array;
        }

        static constexpr const value_type* const cend()
        {
            return array + size();
        }
    };

    constexpr std::size_t size() const
    {
        return Bounds::size();
    }

    constexpr const value_type& operator[](std::size_t i) const
    {
        return get(i);
    }

    constexpr const value_type* begin() const
    {
        return Bounds::begin();
    }

    constexpr const value_type* end() const
    {
        return Bounds::end();
    }

    constexpr const value_type* const cbegin() const
    {
        return Bounds::cbegin();
    }

    constexpr const value_type* const cend() const
    {
        return Bounds::cend();
    }

private:
    static constexpr value_type array[] = {
        TypeToValue::template get<Head>(),
        TypeToValue::template get<Tail>()...
    };
};

template<template<typename...> class Seq, typename Head, typename... Tail, typename TypeToValue>
constexpr typename TypeToValue::value_type SequenceToArray<Seq<Head, Tail...>, TypeToValue>::array[sizeof...(Tail) + 1];

template<template<typename...> class Seq, typename TypeToValue>
class SequenceToArray<Seq<>, TypeToValue>
{
public:
    using value_type = typename TypeToValue::value_type;

    constexpr SequenceToArray() = default;

    static constexpr const value_type& get(std::size_t i)
    {
        return *static_cast<value_type*>(nullptr);
    }

    struct Bounds
    {
        static constexpr std::size_t size()
        {
            return 0;
        }

        static constexpr const value_type* begin()
        {
            return nullptr;
        }

        static constexpr const value_type* end()
        {
            return nullptr;
        }

        static constexpr const value_type* const cbegin()
        {
            return nullptr;
        }

        static constexpr const value_type* const cend()
        {
            return nullptr;
        }
    };

    static constexpr ConstArrayView<value_type> get()
    {
        return {static_cast<value_type*>(nullptr), static_cast<std::size_t>(0)};
    }

    constexpr std::size_t size() const
    {
        return Bounds::size();
    }

    constexpr const value_type& operator[](std::size_t i) const
    {
        return get(i);
    }

    constexpr const value_type* begin() const
    {
        return Bounds::begin();
    }

    constexpr const value_type* end() const
    {
        return Bounds::end();
    }

    constexpr const value_type* const cbegin() const
    {
        return Bounds::cbegin();
    }

    constexpr const value_type* const cend() const
    {
        return Bounds::cend();
    }
};

template<typename T>
struct IntegralConstantToValue
{
    using value_type = T;

    template<typename IntegralConstant>
    static constexpr value_type get()
    {
        return IntegralConstant::value;
    }
};

template<typename T, T... Values>
using PackToArray = SequenceToArray<
    ::cpp::meta::list<std::integral_constant<T, Values>...>,
    IntegralConstantToValue<T>
>;

template<typename String>
class SequenceToString;

template<template<typename...> class Seq, char... Chars>
class SequenceToString<Seq<std::integral_constant<char, Chars>...>>
{
public:
    static constexpr const String<sizeof...(Chars) + 1>& str()
    {
        return _str;
    }

    static constexpr const char* c_str()
    {
        return _str.c_str();
    }

private:
    static constexpr String<sizeof...(Chars) + 1> _str{
        Chars..., '\0'
    };
};

template<template<typename...> class Seq, char... Chars>
constexpr String<sizeof...(Chars) + 1> SequenceToString<Seq<std::integral_constant<char, Chars>...>>::_str;

template<typename Lhs, typename LhsToValue, typename Rhs, typename RhsToValue>
constexpr bool operator==(const SequenceToArray<Lhs, LhsToValue>& lhs, const SequenceToArray<Rhs, RhsToValue>& rhs)
{
    return cpp::constexp::equal(lhs, rhs);
}

template<typename Lhs, typename LhsToValue, typename Rhs, typename RhsToValue>
constexpr bool operator!=(const SequenceToArray<Lhs, LhsToValue>& lhs, const SequenceToArray<Rhs, RhsToValue>& rhs)
{
    return !(lhs == rhs);
}

template<typename Sequence, typename ToValue>
std::ostream& operator<<(std::ostream& os, const SequenceToArray<Sequence, ToValue>& sequence)
{
    os << "{";

    for(std::size_t i = 0; i < sequence.size(); ++i)
    {
        if(i < sequence.size() - 1)
        {
            os << sequence[i] << ", ";
        }
        else
        {
            os << sequence[i];
        }
    }

    return os << "}";
}

template<char... Chars>
using PackToString = SequenceToString<
    ::cpp::meta::list<std::integral_constant<char, Chars>...>
>;

}

}

#endif // SIPLASPLAS_CONSTEXPR_META_HPP
