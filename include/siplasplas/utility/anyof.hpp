#ifndef SIPLASPLAS_UTILITY_ANYOF_HPP
#define SIPLASPLAS_UTILITY_ANYOF_HPP

#include "memberfunctor.hpp"
#include <algorithm>

namespace cpp
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

private:
    Begin _begin;
    End _end;
};

template<typename Range, typename Predicate>
auto anyOf(const Range& range, Predicate predicate)
{
    return AnyOf<Predicate, decltype(std::begin(range)), decltype(std::end(range))>{
        predicate, std::begin(range), std::end(range)
    };
}

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
