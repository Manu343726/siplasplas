#include "foobar.hpp"

using namespace cpp;

int main()
{
    Foo foo;
    SignalEmitter::connect(foo, &Foo::signal, [](int i)
    {
        std::cout << "Hello from lambda! (i=" << i << ")\n";
    });

    {
        Bar bar;

        SignalEmitter::connect(foo, &Foo::signal, bar, &Bar::slot);
        emit(foo).signal(42);
    }

    emit(foo).signal(3141592);
}
