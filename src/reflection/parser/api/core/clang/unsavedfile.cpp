#include "unsavedfile.hpp"

using namespace ::cpp::reflection::parser::api::core::clang;
using namespace ::cpp::constexp;

UnsavedFile::UnsavedFile(const ConstStringView& fileName, std::string contents) :
    _contents{std::move(contents)},
    _fileName{fileName.str()}
{}

::CXUnsavedFile UnsavedFile::cxUnsavedFile(
    const ConstStringView& fileName,
    const ConstStringView& contents
)
{
    CXUnsavedFile cxUnsavedFile;

    cxUnsavedFile.Contents = contents.c_str();
    cxUnsavedFile.Length   = contents.size();
    cxUnsavedFile.Filename = fileName.c_str();

    return cxUnsavedFile;
}

::CXUnsavedFile UnsavedFile::cxUnsavedFile() const
{
    return cxUnsavedFile(_fileName, _contents);
}

const std::string& UnsavedFile::contents() const
{
    return _contents;
}

const std::string& UnsavedFile::fileName() const
{
    return _fileName;
}

FileWriter::FileWriter(const ConstStringView& contents) :
    _stream{contents.str()}
{}

UnsavedFile FileWriter::commit(const ConstStringView& fileName) const
{
    return {fileName, _stream.str()};
}

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

std::ostream& operator<<(std::ostream& os, const UnsavedFile& unsavedFile)
{
    return os << "UnsavedFile(" << unsavedFile.fileName() << ")";
}

}

}

}

}

}

}
