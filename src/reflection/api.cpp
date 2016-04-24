#include "api.hpp"

namespace cpp
{

cpp::MetaClassData& reflection(const cpp::MetaType& type)
{
    return ::cpp::MetaClass::getClass(type);
}

}
