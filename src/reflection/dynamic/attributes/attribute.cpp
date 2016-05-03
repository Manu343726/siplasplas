#include "dynamic/attributes/attribute.hpp"

using namespace cpp::dynamic_reflection::attributes;


std::vector<cpp::dynamic_reflection::Object> Attribute::processArguments(const std::vector<cpp::dynamic_reflection::Object>& args)
{
    return args;
}

cpp::dynamic_reflection::Object Attribute::processReturnValue(const cpp::dynamic_reflection::Object& returnValue)
{
    return returnValue;
}
