#ifndef SIPLASPLAS_SIGNALS_ASYNCSINK_HPP
#define SIPLASPLAS_SIGNALS_ASYNCSINK_HPP

#include "sink.hpp"
#include <readerwriterqueue/readerwriterqueue.h>

namespace cpp
{

class AsyncSink : public SignalSink
{
public:
    template<typename Function>
    AsyncSink(SignalEmitter& caller, Function function) :
        SignalSink{&caller, nullptr},
        _fptr{function}
    {}

    template<typename Function>
    AsyncSink(SignalEmitter& caller, SignalEmitter& callee, Function function) :
        SignalSink{&caller, &callee},
        _fptr{function}
    {}

    void pull() override
    {
        std::vector<cpp::dynamic_reflection::Object> args;

        while(_queue.try_dequeue(args))
        {
            _fptr.invoke(args);
        }
    }

protected:
    void invoke(const std::vector<cpp::dynamic_reflection::Object>& args) override
    {
        _queue.enqueue(args);
    }

private:
    moodycamel::ReaderWriterQueue<std::vector<cpp::dynamic_reflection::Object>> _queue;
    cpp::dynamic_reflection::FunctionPointer _fptr;
};

}

#endif // SIPLASPLAS_SIGNALS_ASYNCSINK_HPP
