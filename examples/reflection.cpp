
#include <iostream>
#include <string>
#include <cstddef>
#include <cassert>

#include "myclass.hpp"
#include <siplasplas/serialization/serialization.hpp>
#include <siplasplas/utility/fusion.hpp>

using namespace std::string_literals;

int freePlainFunction(int a, int b)
{
    return a + b;
}

int main()
{
    MyClass myObject;
    myObject.field = 42;
    auto json = cpp::serialize(myObject);

    std::cout << std::setw(2) << json << std::endl;
    auto fromJson = cpp::deserialize(json).get<MyClass>();
    assert(fromJson.field == 42);
    std::cout << std::setw(4) << cpp::serialize(myObject) << std::endl;

    cpp::reflection<MyClass>().field("field").get(myObject) = 12;
    assert(myObject.field == 12);

    cpp::foreach<cpp::Reflection<MyClass>::Methods>([](auto method)
    {
        using Method = cpp::meta::type_t<decltype(method)>;

        std::cout << "Method \"" << Method::spelling() << "\"" << std::endl
                  << " - Displayname: " << Method::displayName() << std::endl
                  << " - Source file: " << Method::file() << std::endl
                  << " - Line: " << Method::line() << std::endl
                  << " - Params: " << cpp::printTypeList<cpp::function_arguments<typename Method::type>>() << std::endl
                  << " - Type: " << ctti::type_id<typename Method::type>().name() << std::endl << std::endl;
    });

    cpp::foreach<cpp::Reflection<MyClass>::Fields>([](auto field)
    {
        using Field = cpp::meta::type_t<decltype(field)>;

        std::cout << "Field \"" << Field::spelling() << "\"" << std::endl
                  << " - Displayname: " << Field::displayName() << std::endl
                  << " - Source file: " << Field::file() << std::endl
                  << " - Line: " << Field::line() << std::endl
                  << " - Type: " << ctti::type_id<typename Field::type>().name() << std::endl
                  << " - Value type: " << ctti::type_id<typename Field::value_type>().name() << std::endl
                  << " - Class: " << ctti::type_id<typename Field::class_type>().name() << std::endl << std::endl;


    });


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
