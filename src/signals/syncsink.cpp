#include "syncsink.hpp"

using namespace cpp;
using namespace cpp::typeerasure;

bool SyncSink::pull()
{
    return false;
}

void SyncSink::invoke(std::vector<SimpleAny32>&& args)
{
    _fptr.invoke(std::move(args));
}

bool SyncSink::invokeWithoutCallee() const
{
    return _fptr.kind() != FunctionKind::MEMBER_FUNCTION &&
           _fptr.kind() != FunctionKind::CONST_MEMBER_FUNCTION;
}
