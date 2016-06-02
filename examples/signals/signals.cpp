#include "foobar.hpp"

using namespace cpp;

int main()
{
    Foo foo;
    SignalEmitter::connect(foo, &Foo::signal, [](int i)
    {
        std::cout << "Hello from lambda! (i=" << i << ")\n";
    });
    SignalEmitter::connect(foo, &Foo::signal2, [](const std::string& str, int integer)
    {
        std::cout << "signal2: '" << str << "', " << integer << "\n";
    });
    SignalEmitter::connect(foo, &Foo::signal2, foo, &Foo::slot);

    {
        Bar bar;

        SignalEmitter::connect(foo, &Foo::signal, bar, &Bar::slot);
        emit(foo).signal(42);
    }

    emit(foo).signal(3141592);
    emit(foo).signal2(std::string("hello"), 42);
}
