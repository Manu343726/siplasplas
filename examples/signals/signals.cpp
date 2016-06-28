#include "foobar.hpp"

#include <future>
#include <chrono>

using namespace cpp;

void producer(Foo& foo)
{
    std::size_t i = 0;
    auto start = std::chrono::system_clock::now();

    while(std::chrono::system_clock::now() - start < std::chrono::milliseconds(SIPLASPLAS_EXAMPLES_SIGNALS_PRODUCER_MS))
    {
        emit(foo).signal(i++);
    }
}

void consumer(Bar& bar)
{
    auto start = std::chrono::system_clock::now();

    while(std::chrono::system_clock::now() - start < std::chrono::milliseconds(SIPLASPLAS_EXAMPLES_SIGNALS_CONSUMER_MS))
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

