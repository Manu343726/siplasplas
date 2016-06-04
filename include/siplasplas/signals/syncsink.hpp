#ifndef SIPLASPLAS_SIGNALS_SYNCSINK_HPP
#define SIPLASPLAS_SIGNALS_SYNCSINK_HPP

#include "sink.hpp"

namespace cpp
{

class SyncSink : public SignalSink
{
public:
    template<typename Function>
    SyncSink(SignalEmitter& caller, Function function) :
        SignalSink{&caller, nullptr},
        _fptr{function}
    {}

    template<typename Function>
    SyncSink(SignalEmitter& caller, SignalEmitter& callee, Function function) :
        SignalSink{&caller, &callee},
        _fptr{function}
    {}

    void pull() override
    {}

protected:
    void invoke(const std::vector<cpp::dynamic_reflection::Object>& args) override
    {
        _fptr.invoke(args);
    }

private:
    cpp::dynamic_reflection::FunctionPointer _fptr;
};

}

#endif // SIPLASPLAS_SIGNALS_SYNCSINK_HPP
