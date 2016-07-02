#ifndef SIPLASPLAS_SIGNALS_ASYNCSINK_HPP
#define SIPLASPLAS_SIGNALS_ASYNCSINK_HPP

#include "sink.hpp"
#include <siplasplas/reflection/dynamic/function_pointer.hpp>
#include <readerwriterqueue/readerwriterqueue.h>
#include <siplasplas/signals/export.hpp>

namespace cpp
{

/**
 * \ingroup signals
 *
 *
 * \brief Implements an asynchronous signal sink suited for communication
 * between different threads.
 *
 * The AsyncSink class implements a sink that by default enqueues signal emissions
 * in a thread safe queue. The destination function is invoked only when the user asks
 * for incomming signals by pulling the sink. See AsyncSInk::pull().
 */
class SIPLASPLAS_SIGNALS_EXPORT AsyncSink : public SignalSink
{
public:
    /**
     * \brief Creates an asynchronous sink given a caller and a
     * destination function.
     *
     * See SignalEmitter::connect_async()
     *
     * \param caller Caller object.
     * \param function Function to be invoked when the signal is handled.
     */
    template<typename Caller, typename Function>
    AsyncSink(Caller& caller, Function function) :
        SignalSink{caller},
        _fptr{function}
    {}

    /**
     * \brief Creates an asynchronous sink given a caller, a callee, and a
     * destination function.
     *
     * See SignalEmitter::connect_async()
     *
     * \param caller Caller object.
     * \param callee Callee object.
     * \param function Function to be invoked when the signal is handled.
     */
    template<typename Caller, typename Callee, typename Function>
    AsyncSink(Caller& caller, Callee& callee, Function function) :
        SignalSink{caller, callee},
        _fptr{function}
    {}

    /**
     * \brief Checks whether there are queued signal emissions waiting to
     * be handled and invokes the destination function for each of them.
     *
     * The AsyncSink::pull() method fetches pending signals emissions (That is,
     * argument sets from previous signal emissions from the caller object) and
     * invokes the destination function using that arguments, once for each arguments set
     * in the queue. This function continues invoking the destination function until
     * the queue is empty.
     * The destination function is executed from the same thread pull() was invoked.
     *
     * @return true if at lest one pending signal emission was processed, false if the queue was
     * empty.
     */
    bool pull() override;

    virtual ~AsyncSink();

protected:
    void invoke(const std::vector<cpp::dynamic_reflection::Object>& args) override;
    bool invokeWithoutCallee() const override;

private:
    moodycamel::ReaderWriterQueue<std::vector<cpp::dynamic_reflection::Object>> _queue;
    cpp::dynamic_reflection::FunctionPointer _fptr;
};

}

#endif // SIPLASPLAS_SIGNALS_ASYNCSINK_HPP
