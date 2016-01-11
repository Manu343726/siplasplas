
#include <iostream>
#include <string>

#include "reflection/detail/type_info.hpp"
#include "reflection/detail/any.hpp"
#include "reflection/detail/metatype.hpp"
#include "reflection/detail/metaobject.hpp"

using namespace std::string_literals;

// Force 'std::string' as name. GCC uses inline namespaces as part of
// std:: namespace for stdlibc++ ABI versioning
CPP_REFLECTION_CUSTOM_TYPENAME_FOR(std::string, "std::string");

int main()
{
    static_assert(cpp::TypeInfo::get<int>()(cpp::TypeTrait::is_integral), "???");

    cpp::MetaObject::registerMetaObject<int>();
    cpp::MetaObject::registerMetaObject<std::string>();

    auto intType = cpp::MetaType::get("int");

    cpp::MetaObject integer{ intType };
    cpp::MetaObject string{ cpp::MetaType::get("std::string") };
    string = "hola caracola"s;
    
    integer = 2;
    std::cout << integer.get<int>() << std::endl;

    integer = string;
    std::cout << integer.get<std::string>() << std::endl;
}
