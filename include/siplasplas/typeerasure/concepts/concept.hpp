#ifndef SIPLASPLAS_TYPEERASURE_CONCEPTS_CONCEPT_HPP
#define SIPLASPLAS_TYPEERASURE_CONCEPTS_CONCEPT_HPP

#include <type_traits>

namespace cpp
{

namespace concepts
{

/**
 * \ingroup concepts
 * \brief Represents a concept
 *
 * siplasplas-typeerasure represents concepts as boolean
 * type traits, where `Concept::value == true` means the concept
 * is satisfied, and false otherwise.
 *
 * Concepts also keep track of the `noexcept`-ness of the feature
 * they check, to optimize the corresponding type erased feature (See cpp::features namespace).
 *
 * \tparam Result Result of the concept check. True if the concept is satisfied, false otherwise.
 * \tparam NoExcept Whether the checked feature is noexcept. False by dfault (By default the
 * `noexcept`-ness of the feature is not checked).
 */
template<bool Result, bool NoExcept = false>
class Concept : public std::integral_constant<bool, Result>
{
public:
    static constexpr bool no_except = NoExcept;
};

}

}

#endif // SIPLASPLAS_TYPEERASURE_CONCEPTS_CONCEPT_HPP
