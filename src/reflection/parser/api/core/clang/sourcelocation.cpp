#include "sourcelocation.hpp"
#include "translationunit.hpp"

using namespace cpp::reflection::parser::api::core::clang;

SourceLocation::SourceLocation(::CXSourceLocation location) :
    _location{location}
{}

SourceLocation SourceLocation::null()
{
    return { ::clang_getNullLocation() };
}

SourceLocation SourceLocation::fromLineAndColumn(const TranslationUnit& tu, const File& file, std::size_t line, std::size_t column)
{
    return { ::clang_getLocation(tu.rawHandle(), file.cxFile(), line, column) };
}

SourceLocation SourceLocation::fromOffset(const TranslationUnit& tu, const File& file, std::size_t offset)
{
    return { ::clang_getLocationForOffset(tu.rawHandle(), file.cxFile(), offset) };
}

SourceLocation::Location SourceLocation::expansionLocation() const
{
    ::CXFile file;
    unsigned line, column, offset;

    ::clang_getExpansionLocation(cxSourceLocation(), &file, &line, &column, &offset);

    return { file, line, column, offset };
}

SourceLocation::Location SourceLocation::location() const
{
    ::CXFile file;
    unsigned line, column, offset;

    ::clang_getFileLocation(cxSourceLocation(), &file, &line, &column, &offset);

    return { file, line, column, offset };
}

bool SourceLocation::isNull() const
{
    return *this != null();
}

::CXSourceLocation SourceLocation::cxSourceLocation() const
{
    return _location;
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

bool operator==(const SourceLocation& lhs, const SourceLocation& rhs)
{
    return ::clang_equalLocations(lhs.cxSourceLocation(), rhs.cxSourceLocation());
}

bool operator!=(const SourceLocation& lhs, const SourceLocation& rhs)
{
    return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& os, const SourceLocation::Location& location)
{
    return os << location.file << ":" << location.line << "," << location.column;
}

std::ostream& operator<<(std::ostream& os, const SourceLocation& sourceLocation)
{
    return os << sourceLocation.location();
}

}

}

}

}

}

}
