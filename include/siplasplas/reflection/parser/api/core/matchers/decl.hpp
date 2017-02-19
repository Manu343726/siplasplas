#ifndef SIPLASPLAS_REFLECTION_PARSER_API_CORE_MATCHERS_DECL_HPP
#define SIPLASPLAS_REFLECTION_PARSER_API_CORE_MATCHERS_DECL_HPP

#include "kind.hpp"
#include "properties.hpp"
#include "boolean.hpp"

namespace cpp
{

namespace reflection
{

namespace parser
{

namespace api
{

namespace core
{

namespace matchers
{

/**
 * \ingroup matchers
 * \brief Matches a namespace declaration
 *
 * \param innerMatchers Set of inner matchers
 *
 * ``` cpp
 * // A namespace with a class member:
 * namespaceDecl(has(classDecl()))
 * ```
 */
SIPLASPLAS_REFLECTION_PARSER_API_CORE_MATCHERS_KINDMATCHER(namespaceDecl, Namespace)

/**
 * \ingroup matchers
 * \brief Matches a class declaration
 *
 * \param innerMatchers Set of inner matchers
 *
 * ``` cpp
 * // A class with a class member:
 * classDecl(has(classDecl()))
 * ```
 */
SIPLASPLAS_REFLECTION_PARSER_API_CORE_MATCHERS_KINDMATCHER(classDecl, ClassDecl)

/**
 * \ingroup matchers
 * \brief Matches a class template declaration
 *
 * \param innerMatchers Set of inner matchers
 *
 * ``` cpp
 * // A class with a class template member:
 * classDecl(has(classTemplate()))
 * ```
 */
SIPLASPLAS_REFLECTION_PARSER_API_CORE_MATCHERS_KINDMATCHER(classTemplate, ClassTemplate)

/**
 * \ingroup matchers
 * \brief Matches a C++ member function declaration
 *
 * \param innerMatchers Set of inner matchers
 *
 * ``` cpp
 * // A class with at a method:
 * classDecl(has(cxxMethod()))
 * ```
 */
SIPLASPLAS_REFLECTION_PARSER_API_CORE_MATCHERS_KINDMATCHER(cxxMethod, CXXMethod)

/**
 * \ingroup matchers
 * \brief Matches a member variable declaration from a C struct,
 * union, or C++ class
 *
 * \param innerMatchers Set of inner matchers
 *
 * ``` cpp
 * // A class with at a field:
 * classDecl(has(fieldDecl()))
 * ```
 */
SIPLASPLAS_REFLECTION_PARSER_API_CORE_MATCHERS_KINDMATCHER(fieldDecl, FieldDecl)

/**
 * \ingroup matchers
 * \brief Matches a C++ static member function declaration
 *
 * \param innerMatchers Set of inner matchers
 *
 * ``` cpp
 * // A class with at a static method:
 * classDecl(has(cxxStaticMethod()))
 * ```
 */
SIPLASPLAS_REFLECTION_PARSER_API_CORE_MATCHERS_KINDMATCHER_EX(cxxStaticMethod, CXXMethod, isStatic())

/**
 * \ingroup matchers
 * \brief Matches a C++ non-static member function declaration
 *
 * \param innerMatchers Set of inner matchers
 *
 * ``` cpp
 * // A class with at a non-static method:
 * classDecl(has(cxxStaticMethod()))
 * ```
 */
SIPLASPLAS_REFLECTION_PARSER_API_CORE_MATCHERS_KINDMATCHER_EX(cxxNonStaticMethod, CXXMethod, unless(isStatic()))

}

}

}

}

}

}

#endif // SIPLASPLAS_REFLECTION_PARSER_API_CORE_MATCHERS_DECL_HPP
