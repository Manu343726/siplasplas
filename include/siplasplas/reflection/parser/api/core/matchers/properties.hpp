#ifndef SIPLASPLAS_REFLECTION_PARSER_API_CORE_MATCHERS_PROPERTIES_HPP
#define SIPLASPLAS_REFLECTION_PARSER_API_CORE_MATCHERS_PROPERTIES_HPP

#include <siplasplas/constexpr/stringview.hpp>
#include <siplasplas/utility/preprocessor.hpp>
#include <siplasplas/reflection/parser/api/core/matcher.hpp>
#include <siplasplas/reflection/parser/api/core/export.hpp>
#include <siplasplas/utility/function_traits.hpp>
#include <siplasplas/utility/memberfunctor.hpp>
#include <clang-c/Index.h>

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
 * \brief Checks a property of a cursor implemented as a libclang API function
 *
 * \tparam ResultType
 * \tparam ExpectedResult Expected result value. The matcher matches if the
 * current cursor applied to the function returns this value.
 * \tparam ResultType(*Api)(::CXCursor) Function pointer to a libclang API function
 */
template<typename ResultType, ResultType ExpectedResult, ResultType(*Api)(::CXCursor)>
class CursorLibclangPropertyMatcher : public MatcherInterface
{
public:
    bool matches(const core::clang::Cursor& cursor, BoundCursors& boundCursors, AstMatchFinder& finder) override final
    {
        return Api(cursor.cxCursor()) == ExpectedResult;
    }
};

/**
 * \ingroup matchers
 * \brief Represents the type of a libclang C++ AST introspection API function
 * See http://clang.llvm.org/doxygen/group__CINDEX__CPP.html for details
 */
using LibclangCppAstIntrospectionAPI = decltype(::clang_CXXMethod_isVirtual);

/**
 * \ingroup matchers
 * \brief Checks if the cursor follows a property
 * defined in the libclang C++ AST introspection API
 *
 * Since siplasplas-reflection-parser-api-core-clang C++ API
 * does not implement all the C++ AST introspection properties
 * of libclang, this template is used to call libclang directly
 * to query the properties of a cursor pointing to a C++ entity.
 *
 * See http://clang.llvm.org/doxygen/group__CINDEX__CPP.html for
 * details.
 *
 * \tparam Property Function pointer to a libclang C+++ AST
 * introspection function
 */
template<LibclangCppAstIntrospectionAPI Property>
using AstIntrospectionPropertyMatcher = CursorLibclangPropertyMatcher<unsigned int, true, Property>;

/**
 * \ingroup matchers
 * \brief Implements a matcher that matches if a given property
 * is true on the current cursor
 *
 * \tparam Predicate Predicate callable
 */
template<typename Predicate>
class PropertyMatcher
    : public MatcherInterface
    , public cpp::MemberFunctor<Predicate>
{
public:
    using cpp::MemberFunctor<Predicate>::MemberFunctor;

    bool matches(const core::clang::Cursor& cursor, BoundCursors& boundCursors, AstMatchFinder& finder) override final
    {
        return cpp::MemberFunctor<Predicate>::invoke(cursor);
    }
};

/**
 * \ingroup matchers
 * \brief Checks if a given predicate is true for
 * the current cursor
 */
template<typename Predicate>
PropertyMatcher<std::decay_t<Predicate>> isTrue(Predicate&& predicate)
{
    return { std::forward<Predicate>(predicate) };
}

/**
 * \ingroup matchers
 * \brief Defines a matcher API to check if a property of a cursor represented by
 * a given libclang API function equals an expected value.
 *
 * This macro is used to define common cursor properties directly from the corresponding
 * libclang function. For example:
 *
 * ``` cpp
 * // Defines a matcher to match cursors pointing to members with public access specifier:
 * SIPLASPLAS_REFLECTION_PARSER_API_CORE_LIBCLANG_PROPERTY_MATCHER(isPublic, ::clang_getCXXAccessSpecifier, ::CX_CXXPublic);
 * ```
 *
 * \param matcher Name of the matcher
 * \param libclang_api Pointer to a libclang API function. Must have signature `decltype(expectedValue)(::CXCursor)`.
 * \param expectedValue Value that the libclang function must return when invoked with the current cursor. If the returned
 * value matches this, the matcher matches.
 */
#define SIPLASPLAS_REFLECTION_PARSER_API_CORE_LIBCLANG_PROPERTY_MATCHER(matcher, libclang_api, expectedValue)                                     \
    ::cpp::reflection::parser::api::core::matchers::CursorLibclangPropertyMatcher<decltype(expectedValue), expectedValue, libclang_api> matcher() \
    {                                                                                                                                             \
        return {};                                                                                                                                \
    }

/**
 * \ingroup matchers
 * \brief Defines a matcher API to check if a cursor follows
 * a certain property represented as a libclang C++ AST introspection
 * API function. See http://clang.llvm.org/doxygen/group__CINDEX__CPP.html
 *
 * \param matcher Name of the matcher function API
 * \param libclang_api name of the libclang API function
 *
 * The macro generates a function `CursorPropertyMatcher<libclang_api> matcher();`
 * For example:
 *
 * ``` cpp
 * SIPLASPLAS_REFLECTION_PARSER_API_CORE_CXXINTROSPECTION_MATCHER(
 *     isCopyConstructor, clang_CXXConstructor_isCopyConstructor
 * )
 *
 * // The resulting matcher API can be used as follows:
 *
 * // Match any class with a user-declared copy constructor:
 * matchFinder.addMatch(classDecl(hasName("Class"), has(isCopyConstructor()a));)
 * ```
 */
#define SIPLASPLAS_REFLECTION_PARSER_API_CORE_CXXINTROSPECTION_MATCHER(matcher, libclang_api)                 \
    ::cpp::reflection::parser::api::core::matchers::AstIntrospectionPropertyMatcher<::libclang_api> matcher() \
    {                                                                                                         \
        return {};                                                                                            \
    }

/**
 * \ingroup matchers
 * \brief Like SIPLASPLAS_REFLECTION_PARSER_API_CORE_CXXINTROSPECTION_MATCHER, but
 * defines the matcher from the property name and its family in the libclang
 * C++ AST introspection API.
 *
 * \param family Property family (CXXConstructor, CXXField, CXXMethod, etc)
 * \param property Name of the property
 */
#define SIPLASPLAS_REFLECTION_PARSER_API_CORE_CXXINTROSPECTION_MATCHER_BYFAMILY(family, property) \
    SIPLASPLAS_REFLECTION_PARSER_API_CORE_CXXINTROSPECTION_MATCHER(                               \
        property,                                                                                 \
        SIPLASPLAS_PP_CAT(SIPLASPLAS_PP_CAT(SIPLASPLAS_PP_CAT(clang_, family), _), property)      \
    )

SIPLASPLAS_REFLECTION_PARSER_API_CORE_LIBCLANG_PROPERTY_MATCHER(isPublic   , ::clang_getCXXAccessSpecifier, ::CX_CXXPublic)
SIPLASPLAS_REFLECTION_PARSER_API_CORE_LIBCLANG_PROPERTY_MATCHER(isProtected, ::clang_getCXXAccessSpecifier, ::CX_CXXProtected)
SIPLASPLAS_REFLECTION_PARSER_API_CORE_LIBCLANG_PROPERTY_MATCHER(isPrivate  , ::clang_getCXXAccessSpecifier, ::CX_CXXPrivate)

SIPLASPLAS_REFLECTION_PARSER_API_CORE_CXXINTROSPECTION_MATCHER_BYFAMILY(CXXMethod, isStatic)
SIPLASPLAS_REFLECTION_PARSER_API_CORE_CXXINTROSPECTION_MATCHER_BYFAMILY(CXXMethod, isConst)
SIPLASPLAS_REFLECTION_PARSER_API_CORE_CXXINTROSPECTION_MATCHER_BYFAMILY(CXXMethod, isVirtual)
SIPLASPLAS_REFLECTION_PARSER_API_CORE_CXXINTROSPECTION_MATCHER_BYFAMILY(CXXMethod, isPureVirtual)

}

}

}

}

}

}

#endif // SIPLASPLAS_REFLECTION_PARSER_API_CORE_MATCHERS_PROPERTIES_HPP
