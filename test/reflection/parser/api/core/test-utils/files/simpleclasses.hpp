#ifndef SIPLASPLAS_TEST_REFLECTION_PARSER_API_CORE_TESTUTILS_FILES_SIMPLECLASSES_HPP
#define SIPLASPLAS_TEST_REFLECTION_PARSER_API_CORE_TESTUTILS_FILES_SIMPLECLASSES_HPP

class Foo
{
public:
    void method();
    void methodConst() const;

    static void foo(const char* str);

    int i;
    static constexpr const char* FOO = "foo";

private:
    int _i;
    void doSomething();
};

template<typename T>
class FooTemplate
{
public:
    void method(const char* str, int i) const;
    int j;
};

#endif // SIPLASPLAS_TEST_REFLECTION_PARSER_API_CORE_TESTUTILS_FILES_SIMPLECLASSES_HPP
