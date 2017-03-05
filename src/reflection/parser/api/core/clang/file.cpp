#include "file.hpp"

using namespace cpp::reflection::parser::api::core::clang;

File::File(::CXFile file) :
    _file{file}
{}

String File::fileName() const
{
    return ::clang_getFileName(_file);
}

std::time_t File::fileTime() const
{
    return ::clang_getFileTime(_file);
}

::CXFile File::cxFile() const
{
    return _file;
}

File::UniqueId File::uniqueId() const
{
    ::CXFileUniqueID id;

    if(!::clang_getFileUniqueID(_file, &id))
    {
        return cpp::just(id);
    }
    else
    {
        return cpp::nothing();
    }
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

bool operator==(const File& lhs, const File& rhs)
{
    return ::clang_File_isEqual(lhs.cxFile(), rhs.cxFile());
}

bool operator!=(const File& lhs, const File& rhs)
{
    return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& os, const File& file)
{
    return os << file.fileName();
}

}

}

}

}

}

}
