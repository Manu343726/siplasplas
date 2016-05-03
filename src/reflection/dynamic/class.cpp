#include "dynamic/class.hpp"

using namespace cpp::dynamic_reflection;

Class::Classes& Class::classes()
{
    static Class::Classes classes;
    return classes;
}
