#ifndef SIPLASPLAS_SIGNALS_SINK_HPP
#define SIPLASPLAS_SIGNALS_SINK_HPP

#include <siplasplas/reflection/dynamic/object_manip.hpp>
#include <type_traits>

namespace cpp
{

class SignalEmitter;

class SignalSink
{
public:
    template<typename Caller, typename Callee>
    SignalSink(Caller& caller, Callee& callee) :
        _caller{&caller},
        _callee{&callee}
    {}

    template<typename Caller>
    SignalSink(Caller& caller) :
        _caller{&caller},
        _callee{}
    {}

    virtual ~SignalSink() = default;

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

    SignalEmitter* callee() const
    {
        return _callee.get<SignalEmitter*>();
    }

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
