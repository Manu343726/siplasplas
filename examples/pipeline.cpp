#include <siplasplas/pipeline/pipeline.hpp>
#include <iostream>

using namespace cpp::pipeline;

template<typename Source, typename Function, typename = decltype(std::declval<Source>()(std::declval<Push<int>>()))>
Stage<int> operator|(const Source& source, const Function& function)
{
    return{ source, function };
}

template<typename Function>
Stage<int> operator|(Stage<int>& previous, const Function& function)
{
    return{ previous, function };
}

using IO = StageIO<int>;

void mul2(IO& io)
{
    for (int i : io)
    {
        std::cout << "mul2(" << i << ")\n";
        std::cout << "Source: " << &io.source() << std::endl;
        std::cout << "Sink: " << &io.sink() << std::endl;
        io(i * 2);
    }
}

void addOne(IO& io)
{
    for (int i : io)
    {
        std::cout << "addone(" << i << ")\n";
        std::cout << "Source: " << &io.source() << std::endl;
        std::cout << "Sink: " << &io.sink() << std::endl;
        io(i + 1);
    }
}

auto pipe = echo({ 1,2,3,4 }) | addOne | mul2;

int main()
{
    for (int i : pipe)
    {
        std::cout << i << std::endl;
    }
}