#ifndef SIPLASPLAS_EXAMPLES_SIGNALS_FOOBAR_HPP
#define SIPLASPLAS_EXAMPLES_SIGNALS_FOOBAR_HPP

#include <siplasplas/signals/emitter.hpp>
#include <iostream>

class Foo : public cpp::SignalEmitter
{
public:
    void signal(int i){}
    void signal2(const std::string& str, int integer){}

    void slot(const std::string& str, int integer)
    {
        std::cout << __PRETTY_FUNCTION__ << ": '" << str << "', " << integer << "\n";
    }
};

class Bar : public cpp::SignalEmitter
{
public:
    void slot(int i)
    {
        std::cout << "Hello from Bar! (i=" << i << ")\n";
    }
};

#include <reflection/examples/signals/foobar.hpp>

#endif // SIPLASPLAS_EXAMPLES_SIGNALS_FOOBAR_HPP
