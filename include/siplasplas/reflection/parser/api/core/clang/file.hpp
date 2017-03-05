#ifndef SIPLASPLAS_REFLECTION_PARSER_API_CORE_CLANG_FILE_HPP
#define SIPLASPLAS_REFLECTION_PARSER_API_CORE_CLANG_FILE_HPP

#include "string.hpp"
#include "handle.hpp"
#include <clang-c/Index.h>
#include <siplasplas/variant/optional.hpp>
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
 * \brief Represents a sourcefile in a particular translation unit
 */
class SIPLASPLAS_REFLECTION_PARSER_API_CORE_CLANG_EXPORT File
{
public:
    /**
     * \brief Uniquely identifies a file pointied by different
     * File objects across an index
     */
    using UniqueId = cpp::Optional<::CXFileUniqueID>;

    /**
     * \brief Initialices a File with the corresponging libclang
     * CXFile handle
     */
    File(::CXFile file);

    /**
     * \brief Returns the full path to the file
     */
    core::clang::String fileName() const;

    /**
     * \brief Returns the last modification time of the file
     */
    std::time_t fileTime() const;

    /**
     * \brief Returns the underlying libclang CXFile handle
     */
    ::CXFile cxFile() const;

    /**
     * \brief Returns the underlying file unique ID
     */
    UniqueId uniqueId() const;

private:
    ::CXFile _file;
};

bool operator==(const File& lhs, const File& rhs);
bool operator!=(const File& lhs, const File& rhs);
std::ostream& operator<<(std::ostream& os, const File& file);

}

}

}

}

}

}

#endif // SIPLASPLAS_REFLECTION_PARSER_API_CORE_CLANG_FILE_HPP
