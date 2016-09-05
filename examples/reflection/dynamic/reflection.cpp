
#include <iostream>
#include <string>
#include <cstddef>
#include <cassert>

#include "../myclass.hpp"
#include <siplasplas/utility/fusion.hpp>

using namespace std::string_literals;
using namespace cpp::dynamic_reflection;

int freePlainFunction(int a, int b)
{
    return a + b;
}

template<typename Class>
void loadClass(Runtime& runtime)
{
    auto class_ = cpp::drfl::Class::create(
        SourceInfo::fromStaticSourceInfo<cpp::srfl::Class<Class>>(),
        Type::get<Class>()
    );

    runtime.addEntity(class_);

    cpp::foreach_type<typename cpp::srfl::Class<Class>::Fields>([&](auto field)
    {
        using Field = cpp::meta::type_t<decltype(field)>;

        runtime.addEntity(
            cpp::drfl::Field::create(
                SourceInfo::fromStaticSourceInfo<Field>(),
                Field::get()
            )
        );
    });

    cpp::foreach_type<typename cpp::srfl::Class<Class>::Methods>([&](auto function)
    {
        using Function = cpp::meta::type_t<decltype(function)>;

        runtime.addEntity(
            cpp::drfl::Function::create(
                SourceInfo::fromStaticSourceInfo<Function>(),
                Function::get()
            )
        );
    });

}

int main()
{
    Runtime runtime{"reflection example"};

    loadClass<MyClass>(runtime);

    auto& field = runtime.namespace_().class_("MyClass").field_("field");
    auto& function = runtime.namespace_().class_("MyClass").function_("f");
    MyClass myObject, myOtherObject;

    field.get(myObject) = 3;
    field.get(myOtherObject) = 4;
    assert(myObject.field == 3);
    assert(myOtherObject.field == 4);

    field.get(myObject) = field.get(myOtherObject);
    assert(myObject.field == 4);

    std::cout << "Result of invoking f: "
              << function(myObject, 1, 2).get<int>()
              << std::endl;
}
