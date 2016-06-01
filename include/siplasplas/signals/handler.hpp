#ifndef SIPLASPLAS_SIGNALS_HANDLER_HPP
#define SIPLASPLAS_SIGNALS_HANDLER_HPP

#include <siplasplas/reflection/dynamic/object_manip.hpp>
#include <siplasplas/reflection/dynamic/function.hpp>

namespace cpp
{

class Emitter;

class Handler
{
public:
    void invoke(Emitter& emitter, const std::vector<cpp::dynamic_reflection::Object>& args)
    {
        
    }

private:
    std::shared_ptr<cpp::dynamic_reflection::Function> _function;
};

}

#endif // SIPLASPLAS_SIGNALS_HANDLER_HPP
