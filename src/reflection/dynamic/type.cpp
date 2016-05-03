#include "dynamic/type.hpp"

using namespace cpp::dynamic_reflection;

Type::Types& Type::types()
{
    static Type::Types types;
    return types;
}
