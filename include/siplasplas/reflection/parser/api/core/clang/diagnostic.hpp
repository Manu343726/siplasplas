#ifndef SIPLASPLAS_REFLECTION_PARSER_CORE_CLANG_DIAGNOSTIC_HPP
#define SIPLASPLAS_REFLECTION_PARSER_CORE_CLANG_DIAGNOSTIC_HPP

#include <clang-c/Index.h>
#include "handle.hpp"
#include "string.hpp"
#include <siplasplas/reflection/parser/api/core/clang/export.hpp>


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
 * \brief Unique handle to a libclang CXDiagnostic pointer
 */
using CXDiagnostic = core::clang::UniqueHandle<
    ::CXDiagnostic,
    ::clang_disposeDiagnostic
>;

/**
 * \ingroup clang
 * \brief A single diagnostic, containing the diagnostic's severity, text, etc
 */
class SIPLASPLAS_REFLECTION_PARSER_API_CORE_CLANG_EXPORT Diagnostic : public core::clang::CXDiagnostic
{
public:
    using core::clang::CXDiagnostic::CXDiagnostic;

    /**
     * \brief Options to control the display of diagnostics
     */
    enum class DisplayOption
    {
	/**
	 * \brief Display the source-location information where the diagnostic was located.
         *
         * When set, diagnostics will be prefixed by the file, line, and (optionally) column
	 * to which the diagnostic refers. For example,
         *
 	 * ```
         * test.c:28: warning: extra tokens at end of #endif directive
         * ```
         *
         * This option corresponds to the clang flag `-fshow-source-location`.
	 */
        DisplaySourceLocation = ::CXDiagnostic_DisplaySourceLocation,

	/**
	* \brief If displaying the source-location information of the diagnostic,
	* also include the column number.
        *
        * This option corresponds to the clang flag `-fshow-column`.
	*/
        DisplayColumn         = ::CXDiagnostic_DisplayColumn,

	/**
	* \brief If displaying the source-location information of the diagnostic,
        * also include information about source ranges in a machine-parsable format.
	*
	* This option corresponds to the clang flag `-fdiagnostics-print-source-range-info`.
	*/
        DisplaySourceRanges   = ::CXDiagnostic_DisplaySourceRanges,

	/**
        * \brief Display the option name associated with this diagnostic, if any.
        *
        * The option name displayed (e.g., `-Wconversion`) will be placed in
	* brackets after the diagnostic text. This option corresponds to the
	* clang flag `-fdiagnostics-show-option`.
	*/
        DisplayOption         = ::CXDiagnostic_DisplayOption,

	/**
	* \brief Display the category number associated with this diagnostic, if any.
        *
        * The category number is displayed within brackets after the diagnostic text.
        * This option corresponds to the clang flag `-fdiagnostics-show-category=id`.
	*/
        DisplayCategoryId     = ::CXDiagnostic_DisplayCategoryId,

	/**
	* \brief Display the category name associated with this diagnostic, if any.
        *
        * The category name is displayed within brackets after the diagnostic text.
	* This option corresponds to the clang flag `-fdiagnostics-show-category=name`.
	*/
        DisplayCategoryName   = ::CXDiagnostic_DisplayCategoryName
    };

    /**
    * \brief Returns the category number of the diagnostic
    */
    int category() const;

    /**
     * \brief Returns the name of the category of this diagnostic
     */
    core::clang::String categoryText() const;

    /**
     * \brief Returns a string representation of the diagnostic ready to display. This function
     * formats the string with the default display settings
     */
    core::clang::String display() const;

    /**
     * \brief Returns a string representation of the diagnostic ready to display, formatted with
     * the given options
     *
     * \param displayOptions Set of display options
     */
    core::clang::String display(const cpp::constexp::ConstArrayView<DisplayOption>& displayOptions) const;
};

}

}

}

}

}

}

#endif // SIPLASPLAS_REFLECTION_PARSER_CORE_CLANG_DIAGNOSTIC_HPP
