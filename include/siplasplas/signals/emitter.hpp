#ifndef SIPLASPLAS_SIGNALS_EMITTER_HPP
#define SIPLASPLAS_SIGNALS_EMITTER_HPP

#include <siplasplas/reflection/dynamic/object_manip.hpp>
#include <siplasplas/reflection/dynamic/function_pointer.hpp>
#include <siplasplas/utility/hash.hpp>
#include <siplasplas/reflection/api.hpp>

namespace cpp
{

class SignalEmitter;

class Slot
{
public:
    template<typename Sink>
    Slot(Sink sink) :
        _callee{nullptr},
        _sink{sink}
    {}

    template<typename Sink>
    Slot(SignalEmitter& callee, Sink sink) :
        _callee{&callee},
        _sink{sink}
    {}

    template<typename... Args>
    void invoke(Args&&... args)
    {
        if(_callee == nullptr)
        {
            _sink(std::forward<Args>(args)...);
        }
        else
        {
            _sink(*_callee, std::forward<Args>(args)...);
        }

    }

    SignalEmitter* callee() const
    {
        return _callee;
    }

private:
    SignalEmitter* _callee = nullptr;
    cpp::dynamic_reflection::FunctionPointer _sink;
};

class SignalEmitter
{
public:
    template<typename Sink, typename R, typename Class, typename... Args>
    static void connect(SignalEmitter& caller, R(Class::*source)(Args...), Sink sink)
    {
        auto fptr = reinterpret_cast<void*>(source);

        caller._signals[fptr].push_back(Slot{sink});
    }

    template<typename Sink, typename R, typename Class, typename... Args>
    static void connect(SignalEmitter& caller, R(Class::*source)(Args...), SignalEmitter& callee, Sink sink)
    {
        auto fptr = reinterpret_cast<void*>(source);

        caller._signals[fptr].emplace_back(callee, sink);
        callee.registerCaller(caller);
    }

    template<typename Class, typename R, typename... FArgs, typename... Args>
    static void emit(Class& emitter, R(Class::*function)(FArgs...), Args&&... args)
    {
        emitter.invoke(function, std::forward<Args>(args)...);
    }


    ~SignalEmitter()
    {
        for(auto* caller : _callers)
        {
            caller->disconnectCallee(this);
        }
    }

protected:
    template<typename Function, typename... Args>
    void invoke(Function function, Args&&... args)
    {
        auto fptr = reinterpret_cast<void*>(function);
        auto it = _signals.find(fptr);

        if(it != _signals.end())
        {
            auto& connections = it->second;

            for(auto& connection : connections)
            {
                connection.invoke(
                    std::forward<Args>(args)...
                );
            }
        }
    }

private:
    std::vector<SignalEmitter*> _callers;
    cpp::HashTable<void*, std::vector<Slot>> _signals;

    void disconnectCallee(SignalEmitter* callee)
    {
        for(auto& keyValue : _signals)
        {
            auto& slots = keyValue.second;

            slots.erase(std::remove_if(slots.begin(), slots.end(), [callee](const Slot& slot)
            {
                return slot.callee() == callee;
            }), slots.end());
        }
    }

    void registerCaller(SignalEmitter& caller)
    {
        if(std::find(_callers.begin(), _callers.end(), &caller) == _callers.end())
        {
            _callers.push_back(&caller);
        }
    }
};

template<typename Class>
auto emit(const Class& object)
{
    return ::cpp::static_reflection::Class<Class>::fakeObject(
        object,
        [](const Class& object, auto method, auto&&... args)
        {
            SignalEmitter::emit(object, method, std::forward<decltype(args)>(args)...);
        }
    );
}

template<typename Class>
auto emit(Class& object)
{
    return ::cpp::static_reflection::Class<Class>::fakeObject(
        object,
        [](Class& object, auto method, auto&&... args)
        {
            SignalEmitter::emit(object, method, std::forward<decltype(args)>(args)...);
        }
    );
}


}

#endif // SIPLASPLAS_SIGNALS_EMITTER_HPP
