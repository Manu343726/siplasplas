#ifndef SIPLASPLAS_SIGNALS_ASYNCSINK_HPP
#define SIPLASPLAS_SIGNALS_ASYNCSINK_HPP

#include "sink.hpp"
#include <siplasplas/reflection/dynamic/function_pointer.hpp>
#include <readerwriterqueue/readerwriterqueue.h>

namespace cpp
{

class AsyncSink : public SignalSink
{
public:
    template<typename Caller, typename Function>
    AsyncSink(Caller& caller, Function function) :
        SignalSink{caller},
        _fptr{function}
    {}

    template<typename Caller, typename Callee, typename Function>
    AsyncSink(Caller& caller, Callee& callee, Function function) :
        SignalSink{caller, callee},
        _fptr{function}
    {}

    bool pull() override;

protected:
    void invoke(const std::vector<cpp::dynamic_reflection::Object>& args) override;
    bool invokeWithoutCallee() const override;

private:
    moodycamel::ReaderWriterQueue<std::vector<cpp::dynamic_reflection::Object>> _queue;
    cpp::dynamic_reflection::FunctionPointer _fptr;
};

}

#endif // SIPLASPLAS_SIGNALS_ASYNCSINK_HPP
