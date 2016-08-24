#include "classes.hpp"
#include <siplasplas/reflection/static/api.hpp>
#include <siplasplas/typeerasure/any.hpp>
#include <siplasplas/utility/fusion.hpp>
#include <iostream>

using namespace std::string_literals;

template<typename Class, typename... Args>
cpp::Any32 createObject(Args&&... args)
{
    auto any = cpp::Any32::create<Class>(std::forward<Args>(args)...);

    cpp::foreach_type<typename cpp::srfl::Class<Class>::Fields>([&](auto field)
    {
        using Field = cpp::meta::type_t<decltype(field)>;

        any[Field::SourceInfo::spelling()] = Field::get();
    });

    cpp::foreach_type<typename cpp::srfl::Class<Class>::Methods>([&](auto function)
    {
        using Function = cpp::meta::type_t<decltype(function)>;

        any(Function::SourceInfo::spelling()) = Function::get();
    });

    return any;
}

int main()
{
    std::vector<cpp::Any32> objects = {
        createObject<Foo>("hello"),
        createObject<Bar>(42),
        createObject<Quux>("hello", 42)
    };

    // Invoke Foo::append() method:
    objects[0]("append")(", world!"s);

    // Print Foo::str field:
    std::cout << objects[0]["str"].get<std::string>() << std::endl;

    // Equivalent to <Bar object>.i = 42:
    objects[1]["i"] = 42;

    // Invoke Bar::toString() method, which prints i:
    std::cout << objects[1]("toString")().get<std::string>() << std::endl;


    // Arbitrary functions can be assigned as methods too:
    objects[1]("print") = [](const Bar& bar, std::ostream& os)
    {
        os << "Bar(i = " << bar.i << ")";
    };

    objects[1]("print")(std::cout);
}

