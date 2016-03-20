#ifndef SIPLASPLAS_EXAMPLES_REFLECTION_MYCLASS_HPP
#define SIPLASPLAS_EXAMPLES_REFLECTION_MYCLASS_HPP

#include <reflection/api.hpp>

class MyClass : public cpp::Reflectable<MyClass>
{
public:
    SIPLASPLAS_ANNOTATION(
        ::UnknownAttributeClass("goodbye world", [1,2,3,4])
    )
    int f(int a, int b)
    {
        return a + b;
    }

    int field = 0;
};

#include <reflection/examples/myclass.hpp>

#endif // SIPLASPLAS_EXAMPLES_REFLECTION_MYCLASS_HPP
