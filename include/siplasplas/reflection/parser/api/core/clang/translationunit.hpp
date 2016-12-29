#ifndef SIPLASPLAS_REFLECTION_PARSER_API_CORE_CLANG_TRANSLATIONUNIT_HPP
#define SIPLASPLAS_REFLECTION_PARSER_API_CORE_CLANG_TRANSLATIONUNIT_HPP

#include <siplasplas/reflection/parser/api/core/clang/export.hpp>
#include <clang-c/Index.h>
#include <siplasplas/constexpr/stringview.hpp>
#include <range/v3/all.hpp>
#include "handle.hpp"
#include "string.hpp"
#include "diagnostic.hpp"
#include "cursor.hpp"

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

namespace clang
{

/**
 * \ingroup clang
 * \brief Unique handle to a libclang CXTranslationUnit pointer
 */
using CXTranslationUnit = core::clang::UniqueHandle<
    ::CXTranslationUnit,
    ::clang_disposeTranslationUnit
>;

/**
 * \ingroup clang
 * \brief Gives access to a parsed translation unit data
 */
class SIPLASPLAS_REFLECTION_PARSER_API_CORE_CLANG_EXPORT TranslationUnit : public CXTranslationUnit
{
public:
    using CXTranslationUnit::CXTranslationUnit;

    /**
     * \brief Returns the original translation unit source file name
     */
    core::clang::String spelling() const;

    /**
     * \brief Returns the set of diagnostics associated with this translation unit
     *
     * \returns A lazy range of Diagnostic objects
     */
    auto diagnostics() const
    {
        return ranges::view::ints(
            0, static_cast<int>(::clang_getNumDiagnostics(rawHandle()))
        ) | ranges::view::transform(
            [this](int i)
            {
                return Diagnostic{::clang_getDiagnostic(rawHandle(), i)};
            }
        );
    }

    /**
     * \brief Returns the number of compile diagnostics
     * generated when parsing this translation unit
     */
    std::size_t numberOfDiagnostics() const;

    /**
     * \brief Returns whether the translation unit has no associated
     * compile diagnostics
     *
     * \returns True if the translation unit has no diagnostics, false
     * otherwise
     */
    bool noDiagnostics() const;

    /**
     * \brief Returns the cursor that represents this translation unit
     */
    core::clang::Cursor cursor() const;
};

}

}

}

}

}

}

#endif // SIPLASPLAS_REFLECTION_PARSER_API_CORE_CLANG_TRANSLATIONUNIT_HPP
