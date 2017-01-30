#ifndef SIPLASPLAS_REFLECTION_PARSER_API_CORE_MATCHERS_PROPERTIES_HPP
#define SIPLASPLAS_REFLECTION_PARSER_API_CORE_MATCHERS_PROPERTIES_HPP

#include <siplasplas/constexpr/stringview.hpp>
#include <siplasplas/utility/preprocessor.hpp>
#include <siplasplas/reflection/parser/api/core/matcher.hpp>
#include <siplasplas/reflection/parser/api/core/export.hpp>
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
 * \brief Represents the type of a libclang C++ AST introspection API function
 * See http://clang.llvm.org/doxygen/group__CINDEX__CPP.html for details
 */
using LibclangCppAstIntrospectionAPI = decltype(::clang_CXXMethod_isVirtual);

/**
 * \ingroup matchers
 * \brief Checks if the cursor follows a property
 * defined as a boolean predicate on the raw libclang cursor
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
class CursorPropertyMatcher : public MatcherInterface
{
public:
    /**
     * \brief Checks if the matcher matches on a given subtree
     * \returns True if the root cursor follows the required property.
     * False otherwise.
     */
    bool matches(const core::clang::Cursor& cursor, BoundCursors& boundCursors, AstMatchFinder& finder) override final
    {
        return Property(cursor.cxCursor());
    }
};

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
 * SIPLASPLAS_REFLECTION_PARSER_API_CORE_PROPERTY_MATCHER(
 *     isCopyConstructor, clang_CXXConstructor_isCopyConstructor
 * )
 *
 * // The resulting matcher API can be used as follows:
 *
 * // Match any class with a user-declared copy constructor:
 * matchFinder.addMatch(classDecl(hasName("Class"), has(isCopyConstructor()a));)
 * ```
 */
#define SIPLASPLAS_REFLECTION_PARSER_API_CORE_PROPERTY_MATCHER(matcher, libclang_api)               \
    ::cpp::reflection::parser::api::core::matchers::CursorPropertyMatcher<::libclang_api> matcher() \
    {                                                                                               \
        return {};                                                                                  \
    }

/**
 * \ingroup matchers
 * \brief Like SIPLASPLAS_REFLECTION_PARSER_API_CORE_PROPERTY_MATCHER, but
 * defines the matcher from the property name and its family in the libclang
 * C++ AST introspection API.
 *
 * \param family Property family (CXXConstructor, CXXField, CXXMethod, etc)
 * \param property Name of the property
 */
#define SIPLASPLAS_REFLECTION_PARSER_API_CORE_PROPERTY_MATCHER_BYFAMILY(family, property)    \
    SIPLASPLAS_REFLECTION_PARSER_API_CORE_PROPERTY_MATCHER(                                  \
        property,                                                                            \
        SIPLASPLAS_PP_CAT(SIPLASPLAS_PP_CAT(SIPLASPLAS_PP_CAT(clang_, family), _), property) \
    )


SIPLASPLAS_REFLECTION_PARSER_API_CORE_PROPERTY_MATCHER_BYFAMILY(CXXMethod, isStatic)
SIPLASPLAS_REFLECTION_PARSER_API_CORE_PROPERTY_MATCHER_BYFAMILY(CXXMethod, isConst)
SIPLASPLAS_REFLECTION_PARSER_API_CORE_PROPERTY_MATCHER_BYFAMILY(CXXMethod, isVirtual)
SIPLASPLAS_REFLECTION_PARSER_API_CORE_PROPERTY_MATCHER_BYFAMILY(CXXMethod, isPureVirtual)

}

}

}

}

}

}

#endif // SIPLASPLAS_REFLECTION_PARSER_API_CORE_MATCHERS_PROPERTIES_HPP
