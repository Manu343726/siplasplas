
#include <iostream>
#include <string>
#include <cstddef>
#include <cassert>

#include "reflection/detail/type_info.hpp"
#include "reflection/detail/any.hpp"
#include "reflection/detail/metatype.hpp"
#include "reflection/detail/metaobject.hpp"
#include "reflection/field.hpp"
#include "reflection/metaclass.hpp"

using namespace std::string_literals;

// Force 'std::string' as name. GCC uses inline namespaces as part of
// std:: namespace for stdlibc++ ABI versioning
CPP_REFLECTION_CUSTOM_TYPENAME_FOR(std::string, "std::string");

int main()
{
    struct MyClass : public cpp::MetaClassFor<MyClass>
    {
        int field = 0;
        std::string field2;
        char field3;
        std::size_t field4;
    };

    cpp::MetaClass::registerClass<MyClass>({
        SIPLASPLAS_REFLECTION_FIELD(MyClass, field),
        SIPLASPLAS_REFLECTION_FIELD(MyClass, field2),
        SIPLASPLAS_REFLECTION_FIELD(MyClass, field3),
        SIPLASPLAS_REFLECTION_FIELD(MyClass, field4)
    });

    MyClass myObject;

    MyClass::reflection().field("field").get(myObject) = 12;

    assert(myObject.field == 12);

    for(const auto& keyValue : MyClass::reflection().fields())
    {
        const auto& field = keyValue.second;

        std::cout << field.name() << ": Type " << field.type().type().name()
                                  << ", sizeof " << field.type().type().sizeOf()
                                  << ", alignment " << field.type().type().alignment()
                  << std::endl;
    }
}
