#ifndef SIPLASPLAS_SIGNALS_SYNCSINK_HPP
#define SIPLASPLAS_SIGNALS_SYNCSINK_HPP

#include "sink.hpp"
#include <siplasplas/reflection/dynamic/function_pointer.hpp>

namespace cpp
{

class SyncSink : public SignalSink
{
public:
    template<typename Caller, typename Function>
    SyncSink(Caller& caller, Function function) :
        SignalSink{caller},
        _fptr{function}
    {}

    template<typename Caller, typename Callee, typename Function>
    SyncSink(Caller& caller, Callee& callee, Function function) :
        SignalSink{caller, callee},
        _fptr{function}
    {}

    bool pull() override;

protected:
    void invoke(const std::vector<cpp::dynamic_reflection::Object>& args) override;
    bool invokeWithoutCallee() const override;

private:
    cpp::dynamic_reflection::FunctionPointer _fptr;
};

}

#endif // SIPLASPLAS_SIGNALS_SYNCSINK_HPP
