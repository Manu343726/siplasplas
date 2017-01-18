#include "string.hpp"

using namespace ::cpp::reflection::parser::api::core::clang;
using namespace ::cpp::constexp;

const char* String::c_str() const
{
    return ::clang_getCString(rawHandle());
}

ConstStringView String::str() const
{
    return {c_str()};
}

bool operator==(const ::CXString& lhs, const ::CXString rhs)
{
    return lhs.data == rhs.data;
}

bool operator!=(const ::CXString& lhs, const ::CXString& rhs)
{
    return !(lhs == rhs);
}

namespace
{

const ::CXString& nullCXString()
{
    static ::CXString null = { nullptr, 0 };
    return null;
}

}

const ::CXString& NullHandle<::CXString>::value = nullCXString();

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

std::ostream& operator<<(std::ostream& os, const String& str)
{
    return os << str.str();
}

bool operator==(const String& lhs, const cpp::constexp::ConstStringView& rhs)
{
    return lhs.str() == rhs;
}

bool operator==(const cpp::constexp::ConstStringView& lhs, const String& rhs)
{
    return rhs == lhs;
}

bool operator==(const std::string& lhs, const String& rhs)
{
    return cpp::constexp::ConstStringView(lhs) == rhs.str();
}

bool operator==(const String& lhs, const std::string& rhs)
{
    return rhs == lhs;
}

bool operator!=(const String& lhs, const cpp::constexp::ConstStringView& rhs)
{
    return !(lhs == rhs);
}

bool operator!=(const cpp::constexp::ConstStringView& lhs, const String& rhs)
{
    return !(lhs == rhs);
}

bool operator!=(const std::string& lhs, const String& rhs)
{
    return !(lhs == rhs);
}

bool operator!=(const String& lhs, const std::string& rhs)
{
    return !(lhs == rhs);
}

}

}

}

}

}

}
