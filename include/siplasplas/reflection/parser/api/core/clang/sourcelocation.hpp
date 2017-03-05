#ifndef SIPLASPLAS_REFLECTION_PARSER_API_CORE_CLANG_SOURCELOCATION_HPP
#define SIPLASPLAS_REFLECTION_PARSER_API_CORE_CLANG_SOURCELOCATION_HPP

#include "file.hpp"
#include <clang-c/Index.h>
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

// Forward-declare TranslationUnit, TranslationUnit uses
// SourceLocation for its Inclusion member class
class TranslationUnit;

/**
 * \ingroup clang
 * \brief Identifies an specific source location within a translation unit
 */
class SIPLASPLAS_REFLECTION_PARSER_API_CORE_CLANG_EXPORT SourceLocation
{
public:
    /**
     * \brief Initializes a SourceLocation object with a libclang
     * CXSourceLocation hanlde
     */
    SourceLocation(::CXSourceLocation location);

    /**
     * \brief Stores the specific data (sourcefile, line, column, etc) of
     * a source location
     */
    struct SIPLASPLAS_REFLECTION_PARSER_API_CORE_CLANG_EXPORT Location
    {
        File file;          ///< The file to which the source location points
        std::size_t line;   ///< The line to which the source location points
        std::size_t column; ///< The column to which the source location points
        std::size_t offset; ///< The offset into the buffer the source location points
    };

    /**
     * \brief Returns a null (invalid) source location
     */
    static SourceLocation null();

    /**
     * \brief Returns the source location associated with a given file, line and column
     * in a translation unit
     */
    static SourceLocation fromLineAndColumn(const core::clang::TranslationUnit& tu, const core::clang::File& file, std::size_t line, std::size_t column);

    /**
     * \brief Returns the source location associated with a given file and character offset in
     * a translation unit
     */
    static SourceLocation fromOffset(const core::clang::TranslationUnit& tu, const core::clang::File& file, std::size_t offset);

    /**
     * \brief Returns the file, line, column, and offset of the source location.
     * If the location referes into a macro expansion, returns the location of the
     * macro expansion
     */
    Location expansionLocation() const;

    /**
     * \brief Returns the file, line, column, and offset of the source location.
     * If the location refers into a macro expansion, it returns where the macro was expanded; while if
     * it referes to a macro argument, returns the location where the macro argument was written
     */
    Location location() const;

    /**
     * \brief Checks whether the source location is null (invalid)
     */
    bool isNull() const;

    /**
     * \brief Returns the underlying libclang CXSourceLocation object
     */
    ::CXSourceLocation cxSourceLocation() const;

private:
    ::CXSourceLocation _location;
};

bool operator==(const SourceLocation& lhs, const SourceLocation& rhs);
bool operator!=(const SourceLocation& lhs, const SourceLocation& rhs);
std::ostream& operator<<(std::ostream& os, const SourceLocation& sourceLocation);
std::ostream& operator<<(std::ostream& os, const SourceLocation::Location& location);

}

}

}

}

}

}

#endif // SIPLASPLAS_REFLECTION_PARSER_API_CORE_CLANG_SOURCELOCATION_HPP
