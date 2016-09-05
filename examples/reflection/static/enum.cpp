#include "../myenum.hpp"

int main()
{
    static_assert(cpp::static_reflection::Enum<MyEnum>::fromString("FOO") == MyEnum::FOO, "???");
}
