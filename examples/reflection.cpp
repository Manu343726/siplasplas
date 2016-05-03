
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

    cpp::dynamic_reflection::Class::get<MyClass>();

    auto json = cpp::serialize(myObject);

    std::cout << std::setw(2) << json << std::endl;
    auto fromJson = cpp::deserialize(json).get<MyClass>();
    assert(fromJson.field == 42);
    std::cout << std::setw(4) << cpp::serialize(myObject) << std::endl;

    cpp::foreach<cpp::static_reflection::Class<MyClass>::Methods>([](auto method)
    {
        using Method = cpp::meta::type_t<decltype(method)>;

        std::cout << "Method \"" << Method::spelling() << "\"" << std::endl
                  << " - Displayname: " << Method::displayName() << std::endl
                  << " - Source file: " << Method::file() << std::endl
                  << " - Line: " << Method::line() << std::endl
                  << " - Params: " << cpp::printTypeList<cpp::function_arguments<typename Method::type>>() << std::endl
                  << " - Type: " << ctti::type_id<typename Method::type>().name() << std::endl << std::endl;
    });

    cpp::foreach<cpp::static_reflection::Class<MyClass>::Fields>([](auto field)
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
}
