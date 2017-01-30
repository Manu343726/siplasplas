#ifndef SIPLASPLAS_TEST_REFLECTION_PARSER_API_CORE_TESTUTILS_FILES_SIMPLECLASSES_HPP
#define SIPLASPLAS_TEST_REFLECTION_PARSER_API_CORE_TESTUTILS_FILES_SIMPLECLASSES_HPP

namespace FooNamespace
{

class Foo
{
public:
    void foo();
};

namespace BarNamespace
{

class Bar
{
public:
    void bar();

    class Quux
    {
        class Bar {};
    };
};

namespace QuuxNamespace
{

}

}

}

#endif // SIPLASPLAS_TEST_REFLECTION_PARSER_API_CORE_TESTUTILS_FILES_SIMPLECLASSES_HPP
