#ifndef SIPLASPLAS_SIGNALS_SYNCSINK_HPP
#define SIPLASPLAS_SIGNALS_SYNCSINK_HPP

#include "sink.hpp"
#include <siplasplas/typeerasure/function.hpp>
#include <siplasplas/signals/export.hpp>

namespace cpp
{

/**
 * \ingroup signals
 *
 *
 * \brief Implements a direct connection to the destination function
 *
 * Synchronous sinks have no special policy for function invocation but
 * just invoke the function directly when a signal arrives. See SignalEmitter::connect().
 */
class SIPLASPLAS_SIGNALS_EXPORT SyncSink : public SignalSink
{
public:
    /**
     * \brief Creates a synchronous sink given a caller and a
     * destination function.
     *
     * See SignalEmitter::connect_async()
     *
     * \param caller Caller object.
     * \param function Function to be invoked when the signal is handled.
     */
    template<typename Caller, typename Function>
    SyncSink(Caller& caller, Function function) :
        SignalSink{caller},
        _fptr{function}
    {}

    /**
     * \brief Creates a synchronous sink given a caller, a callee, and a
     * destination function.
     *
     * See SignalEmitter::connect_async()
     *
     * \param caller Caller object.
     * \param callee Callee object.
     * \param function Function to be invoked when the signal is handled.
     */
    template<typename Caller, typename Callee, typename Function>
    SyncSink(Caller& caller, Callee& callee, Function function) :
        SignalSink{caller, callee},
        _fptr{function}
    {}

    /**
     * \brief Does nothing.
     *
     * Synchronous sinks directly invoke the destination function when
     * SignalSink::operator() and SyncSink::invoke() are invoked, so pull()
     * has no work to do.
     *
     * @return Always returns true.
     */
    bool pull() override;

    virtual ~SyncSink() = default;

protected:
    void invoke(std::vector<cpp::SimpleAny32>&& args) override;
    bool invokeWithoutCallee() const override;

private:
    cpp::typeerasure::Function32 _fptr;
};

}

#endif // SIPLASPLAS_SIGNALS_SYNCSINK_HPP
