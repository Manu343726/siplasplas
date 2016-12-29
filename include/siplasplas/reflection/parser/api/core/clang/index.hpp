#ifndef SIPLASPLAS_REFLECTION_PARSER_API_CORE_CLANG_INDEX_HPP
#define SIPLASPLAS_REFLECTION_PARSER_API_CODE_CLANG_INDEX_HPP

#include <siplasplas/constexpr/stringview.hpp>
#include "handle.hpp"
#include "unsavedfile.hpp"
#include "translationunit.hpp"
#include "compileoptions.hpp"
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
 * \brief Unique handle to a libclang CXIndex pointer
 */
using CXIndex = core::clang::UniqueHandle<
    ::CXIndex,
    ::clang_disposeIndex
>;

/**
 * \ingroup clang
 * \brief Compilation index
 */
class SIPLASPLAS_REFLECTION_PARSER_API_CORE_CLANG_EXPORT Index : public core::clang::CXIndex
{
public:
    Index();

    /**
     * \brief Parses the given source file and the translation unit
     * corresponding to that file
     *
     * \param fileName Source file name as passed to the compiler
     * \param compileOptions Set of compile options to parse the translation unit with
     *
     * \returns A null translation unit if an error ocurred during parsing. A non-null
     * translation unit on success. Note a translation unit parsed succesfully may contain
     * diagnostics such as compilation errors and warnings.
     */
    TranslationUnit parse(const cpp::constexp::ConstStringView& fileName, const CompileOptions& compileOptions);

    /**
     * \brief Parses the given unsaved files and the translation unit associated with them
     *
     * \param unsavedFiles Set of unsaved files to parse. The files must exist in the filesystem
     * \param compileOptions Set of compile options to parse the translation unit with
     *
     * \returns A null translation unit if an error ocurred during parsing. A non-null
     * translation unit on success. Note a translation unit parsed succesfully may contain
     * diagnostics such as compilation errors and warnings.
     */
    TranslationUnit parse(const cpp::constexp::ConstArrayView<core::clang::UnsavedFile>& unsavedFiles, const CompileOptions& compileOptions);
};

}

}

}

}

}

}

#endif // SIPLASPLAS_REFLECTION_PARSER_API_CODE_CLANG_INDEX_HPP
