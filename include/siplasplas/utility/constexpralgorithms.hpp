#ifndef SIPLASPLAS_UTILITY_CONSTEXPRALGORITHMS_HPP
#define SIPLASPLAS_UTILITY_CONSTEXPRALGORITHMS_HPP

#include <iterator>

namespace cpp
{

/**
 * \ingroup utility
 * \defgroup constexpr-algorithms Constexpr counterparts of some
 * standard algorithms
 */

/**
 * \ingroup constexpr-algorithms
 * \brief Constexpr functor that compares two values for equality
 *
 * The functor invokes `operator==` on the given values. The functor
 * is constexpr only if the found `operator==` is constexpr.
 */
struct Equal
{
    constexpr Equal() = default;

    template<typename T, typename U>
    constexpr bool operator()(const T& lhs, const U& rhs) const
    {
        return lhs == rhs;
    }
};

/**
 * \ingroup constexpr-algorithms
 * \brief Constexpr functor that compares two values for inequality
 *
 * The functor invokes `operator!=` on the given values. The functor
 * is constexpr only if the found `operator!=` is constexpr.
 */
struct NotEqual
{
    constexpr NotEqual() = default;

    template<typename T, typename U>
    constexpr bool operator()(const T& lhs, const U& rhs) const
    {
        return lhs != rhs;
    }
};

/**
 * \ingroup constexpr-algorithms
 * \brief Returns the distance between two iterators
 */
template<typename Begin, typename End>
constexpr auto distance(Begin begin, End end)
{
    return end - begin;
}

/**
 * \ingroup constexpr-algorithms
 * \brief Compares if two sequences are equal
 *
 * \param begin1 Iterator to the beginning of the first sequence
 * \param end1 Iterator to the end of the first sequence
 * \param begin2 Iterator to the beginning of the second sequence
 * \param end2 Iterator to the end of the second sequence
 * \param compare Comparison predicate of signature `bool(Type1, Type2)`
 * The behavior is undefined if the predicate modifies the elements passed to
 * it
 *
 * This function performs a constexpr equality comparison of two sequences. The
 * constexpr-ness of the function is not guaranteed and depends on the given comparison
 * predicate and the iterators.
 *
 * \returns True if the two sequences are of the same length and their elements are equal.
 * False otherwise
 */
template<
    typename Begin1, typename End1,
    typename Begin2, typename End2,
    typename Compare
>
constexpr bool equal(
    Begin1 begin1, End1 end1,
    Begin2 begin2, End2 end2,
    Compare compare
)
{
    return (begin1 == end1 || begin2 == end2 ||
            ::cpp::distance(begin1, end1) != ::cpp::distance(begin2, end2))
    ?
        true
    :
        compare(*begin1++, *begin2++) &&
        equal(begin1, end1, begin2, end2, compare);
}

/**
 * \ingroup constexpr-algorithms
 * \brief Compares if two sequences are equal
 *
 * \param begin1 Iterator to the beginning of the first sequence
 * \param end1 Iterator to the end of the first sequence
 * \param begin2 Iterator to the beginning of the second sequence
 * \param end2 Iterator to the end of the second sequence
 *
 * This function performs a constexpr equality comparison of two sequences. The
 * constexpr-ness of the function is not guaranteed and depends on the given iterators.
 * The function uses cpp::Equal constexpr functor for comparison
 *
 * \returns True if the two sequences are of the same length and their elements are equal.
 * False otherwise
 */
template<
    typename Begin1, typename End1,
    typename Begin2, typename End2
>
constexpr bool equal(
    Begin1 begin1, End1 end1,
    Begin2 begin2, End2 end2
)
{
    return equal(
        begin1, end1,
        begin2, end2,
        ::cpp::Equal()
    );
}

}

#endif // SIPLASPLAS_UTILITY_CONSTEXPRALGORITHMS_HPP
