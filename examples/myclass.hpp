#ifndef SIPLASPLAS_EXAMPLES_REFLECTION_MYCLASS_HPP
#define SIPLASPLAS_EXAMPLES_REFLECTION_MYCLASS_HPP

#include <reflection/api.hpp>
#include <reflection/attributes/contract.hpp>

$(enable_reflection)
class MyClass
{
public:
    $(cpp::attributes::contract(
        [](int a, int b){ return a > 0; }, "a must be greater than zero",
        [](int r){ return true; },         "this must pass always"
    ))
    int f(int a, int b)
    {
        return a + b;
    }

    int field = 0;
};

#include <reflection/examples/myclass.hpp>

#endif // SIPLASPLAS_EXAMPLES_REFLECTION_MYCLASS_HPP
