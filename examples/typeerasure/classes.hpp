#ifndef SIPLASPLAS_EXAMPLES_TYPEERASURE_CLASSES_HPP
#define SIPLASPLAS_EXAMPLES_TYPEERASURE_CLASSES_HPP

#include <string>
#include <ostream>

class Foo
{
public:
    Foo(const std::string str) :
        str{str}
    {}

    void append(const std::string& tail)
    {
        str += tail;
    }

    std::string str = "hello";
};

class Bar
{
public:
    Bar(int i) :
        i{i}
    {}

    void add(int number)
    {
        i += number;
    }

    std::string toString() const
    {
        return std::to_string(i);
    }

    int i = 0;
};

class Quux : public Foo, public Bar
{
public:
    Quux(const std::string& str, int i) :
        Foo{str},
        Bar{i}
    {}
};


#include <reflection/examples/typeerasure/classes.hpp>

#endif // SIPLASPLAS_EXAMPLES_TYPEERASURE_CLASSES_HPP
