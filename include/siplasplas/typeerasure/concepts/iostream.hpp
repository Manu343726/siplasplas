#ifndef SIPLASPLAS_TYPEERASURE_IOSTREAM_HPP
#define SIPLASPLAS_TYPEERASURE_IOSTREAM_HPP

#include "concept.hpp"

#include <siplasplas/utility/meta.hpp>
#include <ostream>
#include <istream>

namespace cpp
{

namespace concepts
{

/**
 * \ingroup concepts
 * \brief Checks whether a vaue of type T can be written to a standard stream.
 *
 * This concept trait checks if values of a given type T can be
 * written to an stream class using `operator<<(std:ostream&, T)`.
 *
 * \tparam T Type to check.
 * \returns True if the type T has a `operator<<(Stream, T)` overload declared
 * in this translation unit. False otherwise.
 *
 * ``` cpp
 *
 * #include <siplasplas/typeerasure/concepts/iostream.hpp>
 * #include <vector>
 * #include <string>
 *
 * using namespace cpp::concepts;
 *
 * static_assert(!OStreamable<std::vector<int>>(), "vector is not ostreamable by default");
 * static_assert( OStreamable<std::string>(), "string is ostreamable");
 * ```
 */
template<typename T, typename = void>
class OStreamable :  public Concept<false> {};

template<typename T>
class OStreamable<T, cpp::meta::void_t<decltype(std::declval<std::ostream&>() << std::declval<T>())>> :
    public Concept<true, noexcept(std::declval<std::ostream&>() << std::declval<T>())>
{};

/**
 * \ingroup concepts
 * \brief Checks whether a vaue of type T can be read from a standard stream.
 *
 * This concept trait checks if values of a given type T can be
 * read from a stream class using `operator>>(std:istream&, T)`.
 *
 * \tparam T Type to check.
 * \returns True if the type T has a `operator>>(std::istream&, T)` overload declared
 * in this translation unit. False otherwise.
 *
 * ``` cpp
 * #include <siplasplas/typeerasure/concepts/iostream.hpp>
 * #include <vector>
 * #include <string>
 *
 * using namespace cpp::concepts;
 *
 * static_assert(!IStreamable<std::vector<int>>(), "vector is not istreamable by default");
 * static_assert( IStreamable<std::string>(), "string is istreamable");
 * ```
 */
template<typename T, typename = void>
class IStreamable :  public Concept<false> {};

template<typename T>
class IStreamable<T, cpp::meta::void_t<decltype(std::declval<std::istream&>() >> *static_cast<T*>(nullptr))>> :
    public Concept<true, noexcept(std::declval<std::istream&>() >> *static_cast<T*>(nullptr))>
{};

}

}

#endif // SIPLASPLAS_TYPEERASURE_IOSTREAM_HPP
