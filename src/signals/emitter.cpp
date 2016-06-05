#include "emitter.hpp"

using namespace cpp;

SignalEmitter::~SignalEmitter()
{
    for(auto& sink : _incomingConnections)
    {
        sink->caller()->disconnectCallee(this);
    }
}

void SignalEmitter::poll()
{
    for(auto& sink : _incomingConnections)
    {
        sink->pull();
    }
}

void SignalEmitter::disconnectCallee(SignalEmitter* callee)
{
    std::lock_guard<std::mutex> guard{_lockConnections};

    for(auto& keyValue : _connections)
    {
        auto& sinks = keyValue.second;

        sinks.erase(std::remove_if(sinks.begin(), sinks.end(), [callee](const std::shared_ptr<SignalSink>& sink)
                    {
                    return sink->callee() == callee;
                    }), sinks.end());
    }
}

void SignalEmitter::registerIncommingConnection(const std::shared_ptr<SignalSink>& sink)
{
    std::lock_guard<std::mutex> guard{_lockIncommingConnections};
    _incomingConnections.insert(sink);
}

