#include "syncsink.hpp"

using namespace cpp;
using namespace cpp::dynamic_reflection;

bool SyncSink::pull()
{
    return false;
}

void SyncSink::invoke(const std::vector<Object>& args)
{
    _fptr.invoke(args);
}
