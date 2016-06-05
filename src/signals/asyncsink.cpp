#include "asyncsink.hpp"

using namespace cpp;
using namespace cpp::dynamic_reflection;

bool AsyncSink::pull()
{
    std::vector<Object> args;
    bool result = false;

    while(_queue.try_dequeue(args))
    {
        _fptr.invoke(args);
        result = true;
    }

    return result;
}

void AsyncSink::invoke(const std::vector<Object>& args)
{
    _queue.enqueue(args);
}
