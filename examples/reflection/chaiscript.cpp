
#include <chaiscript/chaiscript.hpp>
#include <chaiscript/chaiscript_stdlib.hpp>
#include <siplasplas/utility/fusion.hpp>
#include "myclass.hpp"

int main()
{
    chaiscript::ChaiScript chai{chaiscript::Std_Lib::library()};

    cpp::foreach<cpp::srfl::Class<MyClass>::Methods>([&](auto method)
    {
        using Method = cpp::meta::type_t<decltype(method)>;

        chai.add(chaiscript::fun(Method::get()), Method::SourceInfo::spelling());
    });
}
