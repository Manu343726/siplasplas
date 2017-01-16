#ifndef SIPLASPLAS_REFLECTION_PARSER_API_CORE_CLANG_UNSAVEDFILE_HPP
#define SIPLASPLAS_REFLECTION_PARSER_API_CORE_CLANG_UNSAVEDFILE_HPP

#include <clang-c/Index.h>
#include <string>
#include <sstream>
#include <siplasplas/constexpr/stringview.hpp>
#include "handle.hpp"
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
 * \brief Represents an unsaved file in memory
 */
class SIPLASPLAS_REFLECTION_PARSER_API_CORE_CLANG_EXPORT UnsavedFile
{
public:
    /**
     * \brief Initializes an unsaved file with the given path and contents.
     * The file is **not** written to disk during initialization.
     *
     * \param fileName Full path to the file
     * \param contents File text contents
     */
    UnsavedFile(const cpp::constexp::ConstStringView& fileName, std::string contents);

    /**
     * \brief Returns a libclang CXUnsavedFile object with the given data
     */
    static ::CXUnsavedFile cxUnsavedFile(const cpp::constexp::ConstStringView& fileName, const cpp::constexp::ConstStringView& content);

    /**
     * \brief Returns a libclang CXUnsavedFile object with the data of this UnsavedFile
     */
    ::CXUnsavedFile cxUnsavedFile() const;

    /**
     * \brief Returns the full path to the unsaved file on disk
     */
    const std::string& fileName() const;

    /**
     * \brief Returns the unsaved file contents in memory
     */
    const std::string& contents() const;

    friend std::ostream& operator<<(std::ostream& os, const UnsavedFile& unsavedFile);

private:
    std::string _contents;
    std::string _fileName;
};

/**
 * \ingroup clang
 * \brief Provides an interface to write UnsavedFiles on memory
 */
class FileWriter
{
public:
    FileWriter() = default;

    /**
     * \brief Initializes the writer with the given file contents
     */
    FileWriter(const cpp::constexp::ConstStringView& contents);

    /**
     * \brief Writes a value to the file contents
     */
    template<typename T>
    FileWriter& operator<<(const T& value)
    {
        _stream << value;
        return *this;
    }

    /**
     * \brief Saves the current writer contents to an UnsavedFile object. Current file contents
     * are not discarded after the operation
     *
     * \param fileName Full path to the unsaved file in memory
     */
    UnsavedFile commit(const cpp::constexp::ConstStringView& fileName) const;

private:
    std::ostringstream _stream;
};

}

}

}

}

}

}

#endif // SIPLASPLAS_REFLECTION_PARSER_API_CORE_CLANG_UNSAVEDFILE_HPP
