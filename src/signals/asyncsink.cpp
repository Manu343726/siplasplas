#include "asyncsink.hpp"

using namespace cpp;
using namespace cpp::typeerasure;

AsyncSink::~AsyncSink() = default;

bool AsyncSink::pull()
{
    std::vector<SimpleAny32> args;
    bool result = false;

    while(_queue.try_dequeue(args))
    {
        _fptr.invoke(std::move(args));
        result = true;
    }

    return result;
}

void AsyncSink::invoke(std::vector<SimpleAny32>&& args)
{
    _queue.enqueue(std::move(args));
}

bool AsyncSink::invokeWithoutCallee() const
{
    return _fptr.kind() != FunctionKind::MEMBER_FUNCTION &&
           _fptr.kind() != FunctionKind::CONST_MEMBER_FUNCTION;
}
