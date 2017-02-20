#ifndef SIPLASPLAS_UTILITY_ANYOF_HPP
#define SIPLASPLAS_UTILITY_ANYOF_HPP

#include "memberfunctor.hpp"
#include <algorithm>

namespace cpp
{

namespace detail
{

template<typename Predicate, typename Begin, typename End>
class AnyOf : private cpp::MemberFunctor<Predicate>
{
public:
    AnyOf(Predicate predicate, Begin begin, End end) :
        cpp::MemberFunctor<Predicate>{predicate},
        _begin{begin},
        _end{end}
    {}

    template<typename T>
    friend bool operator==(const T& value, const AnyOf& range)
    {
        return std::any_of(range._begin, range._end, [&](const auto& current)
        {
            return range.invoke(value, current);
        });
    }

    template<typename T>
    friend bool operator!=(const T& value, const AnyOf& range)
    {
        return !(value == range);
    }

private:
    Begin _begin;
    End _end;
};

}

/**
 * \ingroup utility
 * \brief Returns a range that can be compared to a value, so it returns true if
 * the value meets any of the predicates represented in the range
 *
 * ``` cpp
 * if(42 == anyOf({1, 2, 3, 4}[](int a, int b){
 *        return a % 10 == b;
 *   })
 * {
 *     ...
 * }
 * ```
 *
 * \param range Set of values to compare against
 * \param predicate A binary predicate function. Used to compare an user value
 * with all the elements in the range
 *
 * \returns An implementation defined type comparable to a single value. The equality
 * comparison between the returned object and a value returns true if the value is
 * equal to any of the values in the range (Equality represented by the given predicate).
 */
template<typename Range, typename Predicate>
auto anyOf(const Range& range, Predicate predicate)
{
    return ::cpp::detail::AnyOf<Predicate, decltype(std::begin(range)), decltype(std::end(range))>{
        predicate, std::begin(range), std::end(range)
    };
}

/**
 * \ingroup utility
 * \brief Equivalent to the other anyOf() overload, using equality operator as
 * comparison predicate
 */
template<typename Range>
auto anyOf(const Range& range)
{
    return anyOf(range, [](const auto& target, const auto& current)
    {
        return target == current;
    });
}

template<typename T>
auto anyOf(const std::initializer_list<T>& initList)
{
    return anyOf(initList, [](const auto& target, const auto& current)
    {
        return target == current;
    });
}

}

#endif // SIPLASPLAS_UTILITY_ANYOF_HPP
