#include "asyncsink.hpp"

using namespace cpp;
using namespace cpp::dynamic_reflection;

AsyncSink::~AsyncSink() = default;

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

bool AsyncSink::invokeWithoutCallee() const
{
    return _fptr.kind() != FunctionKind::MEMBER_FUNCTION &&
           _fptr.kind() != FunctionKind::CONST_MEMBER_FUNCTION;
}
