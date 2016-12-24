#include <iostream>
#include <backward/backward.hpp>

void print_backtrace(std::size_t frames = 32)
{
    using namespace backward;
    StackTrace stackTrace;
    Printer printer;

    stackTrace.load_here(frames);
    printer.object = true;
    printer.color = true;
    printer.print(stackTrace, stdout);
}

int main()
{
    print_backtrace();
}
