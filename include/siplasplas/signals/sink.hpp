#ifndef SIPLASPLAS_SIGNALS_SINK_HPP
#define SIPLASPLAS_SIGNALS_SINK_HPP

#include <siplasplas/reflection/dynamic/object_manip.hpp>
#include <type_traits>

namespace cpp
{

class SignalEmitter;

/**
 * \ingroup signals
 *
 *
 * \brief Interface to the signals sink API
 *
 * A signal sink represents the destination of an emitted signal, that is,
 * the action that should be invoked and how. Sinks keep track of the objects
 * participating in a signal interchange (The source SignalEmitter and the destination
 * object, if any). The interface provides two member functions to be overrided:
 *
 *  - **SignalSink::invoke()**: Implements the behavior of the sink, that is, what
 *  is done when the source signal is emitted.
 *
 *  - **SignalSink::pull()**: Implements the pull behavior of the sink, what should
 *  be done if the user of the connection explicitly asks for incoming signals.
 *
 * Also the user must specify whether the sink should be invoked with ot without
 * a callee object (See SignalSink::operator()() template). **Callee (if any) is always
 * the first argument of type erased invoke() function**.
 *
 * When an user invokes the signal, he/she does through the SignalSInk::invoke()
 * template, which perfect forwards the signal arguments to the type erased protected
 * functions above.
 *
 */
class SignalSink
{
public:

    /**
     * \brief Constructs a sink given the connection caller and the callee
     *
     * This constructor is used by child classes to initialize connection participants.
     * Note the SignalSink is pure virtual.
     *
     * \param caller Reference to the caller object. Caller must inherit SignalEmitter.
     * \param callee Reference to the callee object. Caller must inherit SignalEmitter.
     */
    template<typename Caller, typename Callee>
    SignalSink(Caller& caller, Callee& callee) :
        _caller{&caller},
        _callee{&callee}
    {}

    /**
     * \brief Constructs a sink given the connection caller object.
     *
     * This constructor is used by child classes to initialize connection caller. This
     * constructor is used by connections with no associated callee (Such as connections invoking
     * a lambda or a free function).
     * Note the SignalSink is pure virtual.
     *
     * \param caller Reference to the caller object. Caller must inherit SignalEmitter.
     */
    template<typename Caller>
    SignalSink(Caller& caller) :
        _caller{&caller},
        _callee{}
    {}

    virtual ~SignalSink() = default;

    /**
     * \brief Invokes the sink with the given arguments.
     *
     * This function invokes the sink by type erasing the call arguments
     * and passing them to SignalSink::invoke(). If neccesary (See SignalSink::invokeWithoutCallee())
     * it also passes a reference to the callee object first.
     *
     * \param args call arguments.
     */
    template<typename... Args>
    void operator()(Args&&... args)
    {
        if(invokeWithoutCallee())
        {
            invoke(
                cpp::dynamic_reflection::pack_to_vector(
                    std::forward<Args>(args)...
                )
            );
        }
        else
        {
            invoke(
                cpp::dynamic_reflection::pack_to_vector(
                    _callee, std::forward<Args>(args)...
                )
            );
        }
    }

    /**
     * \brief Gives the callee of the connection.
     *
     * \return A pointer to the callee object if the connection
     * has an associated callee. Throws in other case.
     */
    SignalEmitter* callee() const
    {
        return _callee.get<SignalEmitter*>();
    }

    /**
     * \brief Gives the caller of the connection.
     *
     * \return A pointer to the caller object.
     */
    SignalEmitter* caller() const
    {
        return _caller.get<SignalEmitter*>();
    }

    virtual bool pull() = 0;

protected:
    virtual void invoke(const std::vector<cpp::dynamic_reflection::Object>& args) = 0;
    virtual bool invokeWithoutCallee() const = 0;

private:
    cpp::dynamic_reflection::Object _caller, _callee;
};

}

#endif // SIPLASPLAS_SIGNALS_SINK_HPP
