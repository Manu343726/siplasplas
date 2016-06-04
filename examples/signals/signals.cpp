#include "foobar.hpp"

#include <future>

using namespace cpp;

void producer(Foo& foo)
{
    for(std::size_t i = 0; true; ++i) 
    {
        emit(foo).signal(i);
    }
}

void consumer(Bar& bar)
{
    while(true)
    {
        bar.poll();
    }
}


int main()
{
    Foo foo;
    Bar bar;

    SignalEmitter::connect_async(foo, &Foo::signal, bar, &Bar::slot);

    auto producerTask = std::async(std::launch::async, producer, std::ref(foo));
    auto consumerTask = std::async(std::launch::async, consumer, std::ref(bar));

    producerTask.get();
    consumerTask.get();
}

