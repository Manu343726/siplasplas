#ifndef SIPLASPLAS_CONSTEXPR_ALGORITHM_HPP
#define SIPLASPLAS_CONSTEXPR_ALGORITHM_HPP

#include <cstdint>
#include <type_traits>

namespace cpp
{

namespace constexp
{

/**
 * \ingroup constexpr
 * \defgroup algorithm
 * \brief Constexpr counterparts of some standard algorithms
 */

/**
 * \ingroup algorithm
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
 * \ingroup algorithm
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
 * \ingroup algorithm
 * \brief Returns an iterator pointing to the beginning of a sequence
 * \returns An iterator pointing to the first element of a sequence. Invokes
 * `sequence.begin()`.
 */
template<typename Sequence>
constexpr auto begin(const Sequence& sequence)
{
    return sequence.begin();
}

/**
 * \ingroup algorithm
 * \brief Returns an iterator pointing to the beginning of an array
 * \returns An iterator pointing to the first element of the array
 * Returns &array[0].
 */
template<typename T, std::size_t N>
constexpr auto begin(const T (&array)[N])
{
    return &array[0];
}

/**
 * \ingroup algorithm
 * \brief Returns an iterator pointing to the end of a sequence
 * \returns An iterator pointing to the past the end element of a sequence. Invokes
 * `sequence.end()`.
 */
template<typename Sequence>
constexpr auto end(const Sequence& sequence)
{
    return sequence.end();
}

/**
 * \ingroup algorithm
 * \brief Returns an iterator pointing to the end of an array
 * \returns An iterator pointing to the past the end element of the array
 * Returns &array[N].
 */
template<typename T, std::size_t N>
constexpr auto end(const T (&array)[N])
{
    return &array[N];
}

/**
 * \ingroup algorithm
 * \brief Returns the distance between two iterators
 */
template<typename Begin, typename End>
constexpr auto distance(Begin begin, End end)
{
    return end - begin;
}

/**
 * \ingroup algorithm
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
    return (begin1 == end1 || begin2 == end2) ?
        ((::cpp::constexp::distance(begin1, end1) !=
          ::cpp::constexp::distance(begin2, end2)) ?
            false
        :
            true
        )
    :
        compare(*begin1++, *begin2++) &&
        ::cpp::constexp::equal(begin1, end1, begin2, end2, compare);
}

/**
 * \ingroup algorithm
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
        ::cpp::constexp::Equal()
    );
}

/**
 * \ingroup algorithm
 * \brief Compares if two sequences are equal
 *
 * \param lhs First sequence
 * \param rhs Second sequence
 *
 * \returns True if the two sequences are of the same length and their elements are equal.
 * False otherwise
 */
template<typename Lhs, typename Rhs>
constexpr bool equal(const Lhs& lhs, const Rhs& rhs)
{
    return ::cpp::constexp::equal(
        ::cpp::constexp::begin(lhs), ::cpp::constexp::end(lhs),
        ::cpp::constexp::begin(rhs), ::cpp::constexp::end(rhs)
    );
}

/**
 * \ingroup algorithm
 * \brief Returns the greatest value of the two given
 * \returns lhs if lhs is bigger or equal than rhs. rhs otherwise.
 */
template<typename T>
constexpr const T& max(const T& lhs, const T& rhs)
{
    return lhs >= rhs ?
        lhs
    :
        rhs;
}

/**
 * \ingroup algorithm
 * \brief Returns the greatest value of the two given
 * \returns lhs if lhs is bigger or equal than rhs. rhs otherwise.
 */
template<typename T, typename U>
constexpr std::common_type_t<T, U> max(const T& lhs, const U& rhs)
{
    return static_cast<std::common_type_t<T, U>>(lhs) >=
           static_cast<std::common_type_t<T, U>>(rhs) ?
        lhs
    :
        rhs;
}

/**
 * \ingroup algorithm
 * \brief Returns the greatest value of the set of values given
 */
template<typename First, typename Second, typename Third, typename... Tail>
constexpr decltype(auto) max(const First& first, const Second& second, const Third& third,
                   const Tail&... tail)
{
    return ::cpp::constexp::max(
        first,
        ::cpp::constexp::max(
            second,
            third,
            tail...
        )
    );
}



/**
 * \ingroup algorithm
 * \brief Returns the smallest value of the two given
 * \returns lhs if lhs is less or equal than rhs. rhs otherwise.
 */
template<typename T>
constexpr const T& min(const T& lhs, const T& rhs)
{
    return lhs <= rhs ?
        lhs
    :
        rhs;
}

/**
 * \ingroup algorithm
 * \brief Returns the smallest value of the two given
 * \returns lhs if lhs is less or equal than rhs. rhs otherwise.
 */
template<typename T, typename U>
constexpr std::common_type_t<T, U> min(const T& lhs, const U& rhs)
{
    // The cast is needed to prevemt signed vs unsigned comparison
    // warnings
    return static_cast<std::common_type_t<T, U>>(lhs) <=
           static_cast<std::common_type_t<T, U>>(rhs) ?
        lhs
    :
        rhs;
}

/**
 * \ingroup algorithm
 * \brief Returns the smallest value of the set of values given
 */
template<typename First, typename Second, typename Third, typename... Tail>
constexpr decltype(auto) min(const First& first, const Second& second, const Third& third,
                   const Tail&... tail)
{
    return ::cpp::constexp::min(
        first,
        ::cpp::constexp::min(
            second,
            third,
            tail...
        )
    );
}


/**
 * \ingroup algorithm
 * \brief Computes the distance between two sequences using the
 * Levenshtein distance algorithm
 */
template<
    typename Begin1, typename End1,
    typename Begin2, typename End2
>
constexpr std::size_t levenshteinDistance(
    Begin1 begin1, End1 end1,
    Begin2 begin2, End2 end2
)
{
    return (begin1 >= end1) ?
        ::cpp::constexp::distance(begin2, end2)
    :(
           (begin2 >= end2) ?
        ::cpp::constexp::distance(begin1, end1)
    :(
        ::cpp::constexp::min(
            ::cpp::constexp::levenshteinDistance(begin1 + 1, end1, begin2, end2) + 1,
            ::cpp::constexp::levenshteinDistance(begin1, end1, begin2 + 1, end2) + 1,
            ::cpp::constexp::levenshteinDistance(begin1 + 1, end1, begin2 + 1, end2) +
            ((*begin1 == *begin2) ? 0 : 1)
        )
    ));
}

/**
 * \ingroup algorithm
 * \brief Computes the distance between two sequences using the
 * Levenshtein distance algorithm
 */
template<typename Lhs, typename Rhs>
constexpr std::size_t levenshteinDistance(const Lhs& lhs, const Rhs& rhs)
{
    return ::cpp::constexp::levenshteinDistance(
        ::cpp::constexp::begin(lhs),
        ::cpp::constexp::end(lhs),
        ::cpp::constexp::begin(rhs),
        ::cpp::constexp::end(rhs)
    );
}

}

}

#endif // SIPLASPLAS_CONSTEXPR_ALGORITHM_HPP
