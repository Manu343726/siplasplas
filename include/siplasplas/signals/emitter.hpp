#ifndef SIPLASPLAS_SIGNALS_EMITTER_HPP
#define SIPLASPLAS_SIGNALS_EMITTER_HPP

#include <siplasplas/reflection/dynamic/object_manip.hpp>

namespace cpp
{

class Emitter
{
public:
    using Argument = cpp::dynamic_reflection::Object;

    template<typename... Args>
    void emit(const Args&... args)
    {
        auto args_vector = cpp::pack_to_vector(args...);

        for(auto& handler : handlers)
        {
            handler.invoke(*this, args_vector);
        }
    }

    void invoke(Emitter& emitter, const 

private:
    std::vector<SignalHandler> _handlers;
};

}

#endif // SIPLASPLAS_SIGNALS_EMITTER_HPP
