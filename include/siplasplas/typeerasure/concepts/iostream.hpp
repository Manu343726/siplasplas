#ifndef SIPLASPLAS_TYPEERASURE_IOSTREAM_HPP
#define SIPLASPLAS_TYPEERASURE_IOSTREAM_HPP

#include <siplasplas/utility/meta.hpp>
#include <ostream>
#include <istream>

namespace cpp
{

namespace concepts
{

/**
 * \ingroup concepts
 * \brief Checks whether a vaue of type T can be written to an stream.
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
 * static_assert(!OStreamable<std::vector<int>>(), "vector is not streamable by default");
 * static_assert( OStreamable<std::string>(), "string is streamable");
 * ```
 */
template<typename T, typename = void>
class OStreamable :  public std::false_type {};

template<typename T>
class OStreamable<T, cpp::meta::void_t<decltype(std::declval<std::ostream&>() << std::declval<T>())>> : public std::true_type {};
}

}

#endif // SIPLASPLAS_TYPEERASURE_IOSTREAM_HPP
