#include "attributes/attribute.hpp"

using namespace cpp::attributes;


std::vector<cpp::MetaObject> Attribute::processArguments(const std::vector<cpp::MetaObject>& args)
{
    return args;
}

cpp::MetaObject Attribute::processReturnValue(const cpp::MetaObject& returnValue)
{
    return returnValue;
}
