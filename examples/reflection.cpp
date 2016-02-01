
#include <iostream>
#include <string>
#include <cstddef>
#include <cassert>

#include "myclass.hpp"

using namespace std::string_literals;

int freePlainFunction(int a, int b)
{
    return a + b;
}

int main()
{
    MyClass myObject;

    cpp::reflection<MyClass>().field("field").get(myObject) = 12;
    assert(myObject.field == 12);

    for(const auto& keyValue : cpp::reflection<MyClass>().fields())
    {
        const auto& field = keyValue.second;

        std::cout << " -- " << field.name() << ": Type " << field.type().type().name()
                            << ", sizeof " << field.type().type().sizeOf()
                            << ", alignment " << field.type().type().alignment()
                  << std::endl;
    }

    for (const auto& keyValue : cpp::reflection<MyClass>().functions())
    {
        const auto& function = keyValue.second;

        std::cout << " -- " << function.name() << ": Return type " << function.returnType().type().name()
            << ", parameter types [";
        
        for (const auto& type : function.parameterTypes())
            std::cout << "'" << type.type().name() << "', ";

        std::cout << "], const: " << std::boolalpha << function.isConst() << std::endl;
    }

    cpp::MetaObject result = cpp::reflection<MyClass>().function("f")(myObject)(1, 2);

    std::cout << myObject.field << std::endl;

    std::cout << freePlainFunction(
            cpp::reflection(myObject).function("f")(1, 10),
            cpp::reflection(myObject).field("field")
    ) << std::endl;
}
