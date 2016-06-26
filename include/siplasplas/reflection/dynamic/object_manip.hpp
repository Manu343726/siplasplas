#ifndef SIPLASPLAS_REFLECTION_DYNAMIC_OBJECT_MANIP_HPP
#define SIPLASPLAS_REFLECTION_DYNAMIC_OBJECT_MANIP_HPP

#include "object.hpp"
#include "logger.hpp"
#include <siplasplas/utility/meta.hpp>
#include <siplasplas/utility/function_traits.hpp>
#include <siplasplas/utility/assert.hpp>
#include <vector>

namespace cpp
{

namespace dynamic_reflection
{

template<typename... Args>
std::vector<cpp::dynamic_reflection::Object> pack_to_vector(Args&&... args)
{
    return {cpp::dynamic_reflection::Object{args}...};
}

namespace
{
    template<typename... Args, std::size_t... Is>
    std::tuple<Args...> vector_to_tuple(const std::vector<cpp::dynamic_reflection::Object>& vector, meta::index_sequence<Is...>)
    {
        return {vector[Is].get<Args>()...};
    }

    template<typename... Args, std::size_t... Is>
    std::vector<cpp::dynamic_reflection::Object> tuple_to_vector(const std::tuple<Args...>& tuple, meta::index_sequence<Is...>)
    {
        return {cpp::dynamic_reflection::Object{std::get<Is>(tuple)}...};
    }

    template<typename Function, std::size_t... Is>
    auto vector_call(Function function, const std::vector<cpp::dynamic_reflection::Object>& args, meta::index_sequence<Is...>)
    {
        auto logCall = [&]
        {
            static ctti::type_id_t signatureArgTypes[] = { ctti::type_id<cpp::function_argument<Is, Function>>()..., ctti::type_id<void>() };

            ::cpp::reflection::dynamic::log().debug("Vector call to '{}' (Args count={}, Signature args count={}):", ctti::type_id<Function>().name(), args.size(), sizeof...(Is));
            for(std::size_t i = 0; i < args.size(); ++i)
            {
                ::cpp::reflection::dynamic::log().debug(
                    "  Arg({}): value '{}'. Signature type '{}'. Arg type '{}'",
                    i,
                    args[i].toString(),
                    signatureArgTypes[i].name(),
                    args[i].type().typeName()
                );
            }
        };

#ifdef SIPLASPLAS_LOG_VECTORCALL
        logCall();
#endif

        SIPLASPLAS_ASSERT_EQ(args.size(), sizeof...(Is)).onFailure([&]
        {
            logCall();
        });

        return function(args[Is].get<cpp::function_argument<Is, Function>>()...);
    }
}

template<typename... Args>
std::tuple<Args...> vector_to_tuple(const std::vector<cpp::dynamic_reflection::Object>& vector)
{
    return vector_to_tuple<Args...>(vector, meta::make_index_sequence_for<Args...>{});
}

template<typename... Args>
std::vector<cpp::dynamic_reflection::Object> tuple_to_vector(const std::tuple<Args...>& tuple)
{
    return tuple_to_vector(tuple, meta::make_index_sequence_for<Args...>{});
}

template<typename Function>
auto vector_call(Function function, const std::vector<cpp::dynamic_reflection::Object>& vector)
{
    return vector_call(function, vector, meta::to_index_sequence_t<typename cpp::function_signature<Function>::args>{});
}

template<typename Class, typename R, typename... Args>
R vector_call(R (Class::* function)(Args...) const, const Class& object, const std::vector<cpp::dynamic_reflection::Object>& vector)
{
    return vector_call([function,&object](Args... args)
    {
        return (object.*function)(args...);
    },
    vector);
}

template<typename Class, typename R, typename... Args>
R vector_call(R (Class::* function)(Args...), Class& object, const std::vector<cpp::dynamic_reflection::Object>& vector)
{
    return vector_call([function,&object](Args... args)
    {
        return (object.*function)(args...);
    },
    vector);
}

template<typename Class, typename R, typename... Args>
R vector_call(R (Class::* function)(Args...) const, const std::vector<cpp::dynamic_reflection::Object>& args)
{
    auto fargs = args;
    const auto& firstArg = args[0];
    fargs.erase(fargs.begin());
    const Class& object = firstArg.get<const Class>();

    return vector_call(function, object, fargs);
}

template<typename Class, typename R, typename... Args>
R vector_call(R (Class::* function)(Args...), const std::vector<cpp::dynamic_reflection::Object>& args)
{
    auto fargs = args;
    const Object& firstArg = args[0];
    Class& object = firstArg.get<Class>();
    fargs.erase(fargs.begin());

    return vector_call(function, object, fargs);
}

}
}

#endif // SIPLASPLAS_REFLECTION_DYNAMIC_OBJECT_MANIP_HPP
