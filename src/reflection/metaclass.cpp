#include "metaclass.hpp"

using namespace cpp;

MetaClass::MetaClassRegistry& MetaClass::metaClasses()
{
    static MetaClass::MetaClassRegistry classes;
    return classes;
}
