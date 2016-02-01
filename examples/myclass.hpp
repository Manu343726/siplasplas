#ifndef SIPLASPLAS_EXAMPLES_REFLECTION_MYCLASS_HPP
#define SIPLASPLAS_EXAMPLES_REFLECTION_MYCLASS_HPP

#include "reflection/api.hpp"

class MyClass : public cpp::Reflectable<MyClass>
{
public:
	int f(int a, int b)
	{
		return a + b;
	}

	int field = 0;
};

#include "myclass.hpp.rfl"

#endif // SIPLASPLAS_EXAMPLES_REFLECTION_MYCLASS_HPP