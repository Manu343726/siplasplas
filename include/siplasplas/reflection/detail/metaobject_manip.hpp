#ifndef SIPLASPLAS_REFLECTION_DETAIL_METAOBJECT_MANIP_HPP
#define SIPLASPLAS_REFLECTION_DETAIL_METAOBJECT_MANIP_HPP

#include "metaobject.hpp"
#include <siplasplas/utility/meta.hpp>
#include <siplasplas/utility/function_traits.hpp>
#include <vector>

namespace cpp
{

template<typename... Args>
std::vector<cpp::MetaObject> pack_to_vector(Args&&... args)
{
    return {cpp::MetaObject{args}...};
}

namespace
{
    template<typename... Args, std::size_t... Is>
    std::tuple<Args...> vector_to_tuple(const std::vector<cpp::MetaObject>& vector, meta::index_sequence<Is...>)
    {
        return {vector[Is].get<Args>()...};
    }

    template<typename... Args, std::size_t... Is>
    std::vector<cpp::MetaObject> tuple_to_vector(const std::tuple<Args...>& tuple, meta::index_sequence<Is...>)
    {
        return {cpp::MetaObject{std::get<Is>(tuple)}...};
    }

    template<typename Function, std::size_t... Is>
    auto vector_call(Function function, const std::vector<cpp::MetaObject>& args, meta::index_sequence<Is...>)
    {
        return function(args[Is].get<cpp::function_argument<Is, Function>>()...);
    }
}

template<typename... Args>
std::tuple<Args...> vector_to_tuple(const std::vector<cpp::MetaObject>& vector)
{
    return vector_to_tuple<Args...>(vector, meta::make_index_sequence_for<Args...>{});
}

template<typename... Args>
std::vector<cpp::MetaObject> tuple_to_vector(const std::tuple<Args...>& tuple)
{
    return tuple_to_vector(tuple, meta::make_index_sequence_for<Args...>{});
}

template<typename Function>
auto vector_call(Function function, const std::vector<cpp::MetaObject>& vector)
{
    return vector_call(function, vector, meta::to_index_sequence_t<typename cpp::function_signature<Function>::args>{});
}

template<typename Class, typename R, typename... Args>
R vector_call(R (Class::* function)(Args...) const, const Class& object, const std::vector<cpp::MetaObject>& vector)
{
    return vector_call([function,&object](Args... args)
    {
        return (object.*function)(args...);
    },
    vector);
}

template<typename Class, typename R, typename... Args>
R vector_call(R (Class::* function)(Args...), Class& object, const std::vector<cpp::MetaObject>& vector)
{
    return vector_call([function,&object](Args... args)
    {
        return (object.*function)(args...);
    },
    vector);
}

template<typename Class, typename R, typename... Args>
R vector_call(R (Class::* function)(Args...) const, const std::vector<cpp::MetaObject>& args)
{
    auto fargs = args;
    fargs.erase(fargs.begin());
    const Class& object = args[0].get<Class>();

    return vector_call(function, object, fargs);
}

template<typename Class, typename R, typename... Args>
R vector_call(R (Class::* function)(Args...), const std::vector<cpp::MetaObject>& args)
{
    auto fargs = args;
    fargs.erase(fargs.begin());
    Class& object = args[0].get<Class>();

    return vector_call(function, object, fargs);
}


}

#endif // SIPLASPLAS_REFLECTION_DETAIL_METAOBJECT_MANIP_HPP
