
#include <iostream>
#include <string>
#include <cstddef>
#include <cassert>

// change
#include "reflection/detail/type_info.hpp"
#include "reflection/detail/any.hpp"
#include "reflection/detail/metatype.hpp"
#include "reflection/detail/metaobject.hpp"
#include "reflection/field.hpp"
#include "reflection/metaclass.hpp"
#include "reflection/function.hpp"

using namespace std::string_literals;

// Force 'std::string' as name. GCC uses inline namespaces as part of
// std:: namespace for stdlibc++ ABI versioning
CPP_REFLECTION_CUSTOM_TYPENAME_FOR(std::string, "std::string")

struct MyClass
{
    struct EnableReflection {};

    int field = 0;
    std::string field2;
    char field3;
    std::size_t field4;

    int f(int i) const
    {
        return i;
    }

    std::string appendInt(int i, std::string str) const
    {
        return str + std::to_string(i) + std::to_string(field);
    }
};

class MyOtherClass
{
    struct EnableReflection;
public:
    int a, b, c, d, e, f, g;
    
    int f1(){ return 1; }
    int f2() const { return 2; }
    int f3(int, char){ return 3; }
};

int main()
{
    MyClass myObject;

    MyClass::reflection().field("field").get(myObject) = 12;
    assert(myObject.field == 12);

    for(const auto& keyValue : MyClass::reflection().fields())
    {
        const auto& field = keyValue.second;

        std::cout << " -- " << field.name() << ": Type " << field.type().type().name()
                            << ", sizeof " << field.type().type().sizeOf()
                            << ", alignment " << field.type().type().alignment()
                  << std::endl;
    }

    for (const auto& keyValue : MyClass::reflection().functions())
    {
        const auto& function = keyValue.second;

        std::cout << " -- " << function.name() << ": Return type " << function.returnType().type().name()
            << ", parameter types [";
        
        for (const auto& type : function.parameterTypes())
            std::cout << "'" << type.type().name() << "', ";

        std::cout << "], const: " << std::boolalpha << function.isConst() << std::endl;
    }

    cpp::MetaObject result = MyClass::reflection().function("f")(myObject)(1);

    std::cout << myObject.field << std::endl;
}
