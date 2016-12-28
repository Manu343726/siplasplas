#include "error.hpp"

using namespace ::cpp::reflection::parser::api::core::clang;
using namespace ::cpp::constexp;

namespace
{

const char* errorCodeString(const Error::Code errorCode)
{
    static constexpr const char* strings[] = {
        "cpp::reflection::parser::api::core::clang::Error::Code::Success",
        "cpp::reflection::parser::api::core::clang::Error::Code::Failure",
        "cpp::reflection::parser::api::core::clang::Error::Code::Crashed",
        "cpp::reflection::parser::api::core::clang::Error::Code::InvalidArguments",
        "cpp::reflection::parser::api::core::clang::Error::Code::ASTReadError",
    };

    return strings[static_cast<int>(errorCode)];
}

const char* errorCodeDetails(const Error::Code errorCode)
{
    static constexpr const char* strings[] = {
        "No error",
        "A generic error code, no further details are given",
        "libclang crashed when performing the requested operation",
        "The function detected that the arguments violate the function contract",
        "An AST deserialization error has occurred"
    };

    return strings[static_cast<int>(errorCode)];
}

}

Error::Error(::CXErrorCode errorCode) :
    _cxErrorCode{errorCode}
{}

bool Error::success() const
{
    return errorCode() == Code::Success;
}

ConstStringView Error::diagnostic() const
{
    return errorCodeString(errorCode());
}

ConstStringView Error::details() const
{
    return errorCodeDetails(errorCode());
}

Error::Code Error::errorCode() const
{
    return static_cast<Error::Code>(cxErrorCode());
}

::CXErrorCode Error::cxErrorCode() const
{
    return _cxErrorCode;
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

std::ostream& operator<<(std::ostream& os, const Error::Code errorCode)
{
    return os << errorCodeString(errorCode);
}

std::ostream& operator<<(std::ostream& os, const Error error)
{
    return os << "cpp::reflection::api::core::clang::Error(" << error.errorCode() << ")";
}

}

}

}

}

}

}
