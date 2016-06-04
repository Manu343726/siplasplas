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
    SignalSink(SignalEmitter* caller, SignalEmitter* callee = nullptr) :
        _caller{caller},
        _callee{callee}
    {}

    virtual ~SignalSink() = default;

    template<typename... Args>
    void operator()(Args&&... args)
    {
        if(_callee == nullptr)
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
        return _callee;
    }

    SignalEmitter* caller() const
    {
        return _caller;
    }

    virtual void pull() = 0;

protected:
    virtual void invoke(const std::vector<cpp::dynamic_reflection::Object>& args) = 0;

private:
    SignalEmitter* _caller;
    SignalEmitter* _callee = nullptr;
};

}

#endif // SIPLASPLAS_SIGNALS_SINK_HPP
