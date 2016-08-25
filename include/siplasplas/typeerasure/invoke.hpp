#ifndef SIPLASPLAS_TYPEERASURE_INVOKE_HPP
#define SIPLASPLAS_TYPEERASURE_INVOKE_HPP

#include "simpleany.hpp"
#include "anyarg.hpp"
#include <siplasplas/utility/invoke.hpp>
#include <siplasplas/utility/meta.hpp>
#include <siplasplas/utility/function_traits.hpp>
#include <vector>

namespace cpp
{

namespace typeerasure
{

namespace detail
{

template<typename Callable, typename Storage, std::size_t... Is>
void logInvoke(::cpp::meta::index_sequence<Is...>, const std::vector<cpp::SimpleAny<Storage>>& args)
{
    static constexpr ctti::type_id_t paramsTypes[] = { ctti::type_id<::cpp::function_argument<Is, std::decay_t<Callable>>>()..., ctti::type_id<void>() };

    typeerasure::log().debug("About to do type-erased invoke (vector args):");
    typeerasure::log().debug("  Function: {}", ctti::type_id<std::decay_t<Callable>>().name());

    for(std::size_t i = 0; i < sizeof...(Is); ++i)
    {
        typeerasure::log().debug("  #{}: Param {}, Arg {}", i, paramsTypes[i].name(), args[i].typeInfo().typeName());
    }
}

template<typename Callable, typename... Storages, std::size_t... Is>
void logInvoke(::cpp::meta::index_sequence<Is...>, const cpp::SimpleAny<Storages>&... args)
{
    static constexpr ctti::type_id_t paramsTypes[] = { ctti::type_id<::cpp::function_argument<Is, std::decay_t<Callable>>>()..., ctti::type_id<void>() };
    TypeInfo argsTypes[] = { args.typeInfo()... , TypeInfo::get<int>()};

    typeerasure::log().debug("About to do type-erased invoke (variadic args):");
    typeerasure::log().debug("  Function: {}", ctti::type_id<std::decay_t<Callable>>().name());

    for(std::size_t i = 0; i < sizeof...(Is); ++i)
    {
        typeerasure::log().debug("  #{}: Param {}, Arg {}", i, paramsTypes[i].name(), argsTypes[i].typeName());
    }
}

template<typename Callable, typename... Storages, std::size_t... Is>
decltype(auto) invoke(Callable&& callable, ::cpp::meta::index_sequence<Is...>, const ::cpp::SimpleAny<Storages>&... args)
{
    logInvoke<Callable>(::cpp::meta::index_sequence<Is...>(), args...);

    return ::cpp::invoke(
        std::forward<Callable>(callable),
        args.template get<
            ::cpp::function_argument<
                Is,
                std::decay_t<Callable>
            >
        >()...
    );
}

template<typename Callable, typename... Storages, std::size_t... Is>
decltype(auto) invoke(Callable&& callable, ::cpp::meta::index_sequence<Is...>, ::cpp::SimpleAny<Storages>&... args)
{
    logInvoke<Callable>(::cpp::meta::index_sequence<Is...>(), args...);

    return ::cpp::invoke(
        std::forward<Callable>(callable),
        args.template get<
            ::cpp::function_argument<
                Is,
                std::decay_t<Callable>
            >
        >()...
    );
}

template<typename Callable, typename... Storages, std::size_t... Is>
decltype(auto) invoke(Callable&& callable, ::cpp::meta::index_sequence<Is...>, ::cpp::SimpleAny<Storages>&&... args)
{
    logInvoke<Callable>(::cpp::meta::index_sequence<Is...>(), args...);

    return ::cpp::invoke(
        std::forward<Callable>(callable),
        args.template get<
            ::cpp::function_argument<
                Is,
                std::decay_t<Callable>
            >
        >()...
    );
}

template<typename Callable, typename Storage, std::size_t... Is>
decltype(auto) invoke(Callable&& callable, ::cpp::meta::index_sequence<Is...>, const std::vector<::cpp::SimpleAny<Storage>>& args)
{
    logInvoke<Callable>(::cpp::meta::index_sequence<Is...>(), args);

    return ::cpp::invoke(
        std::forward<Callable>(callable),
        args[Is].template get<
            ::cpp::function_argument<
                Is,
                std::decay_t<Callable>
            >
        >()...
    );
}

template<typename Callable, typename Storage, std::size_t... Is>
decltype(auto) invoke(Callable&& callable, ::cpp::meta::index_sequence<Is...>, std::vector<::cpp::SimpleAny<Storage>>& args)
{
    logInvoke<Callable>(::cpp::meta::index_sequence<Is...>(), args);

    return ::cpp::invoke(
        std::forward<Callable>(callable),
        args[Is].template get<
            ::cpp::function_argument<
                Is,
                std::decay_t<Callable>
            >
        >()...
    );
}

template<typename Callable, typename Storage, std::size_t... Is>
decltype(auto) invoke(Callable&& callable, ::cpp::meta::index_sequence<Is...>, std::vector<::cpp::SimpleAny<Storage>>&& args)
{
    logInvoke<Callable>(::cpp::meta::index_sequence<Is...>(), args);

    return ::cpp::invoke(
        std::forward<Callable>(callable),
        args[Is].template get<
            ::cpp::function_argument<
                Is,
                std::decay_t<Callable>
            >
        >()...
    );
}

template<typename Callable, std::size_t... Is>
decltype(auto) invoke(Callable&& callable, ::cpp::meta::index_sequence<Is...>, const std::vector<AnyArg>& args)
{
    return ::cpp::invoke(
        std::forward<Callable>(callable),
        args[Is].get<
            ::cpp::function_argument<
                Is,
                std::decay_t<Callable>
            >
        >()...
    );
}

template<typename Callable, std::size_t... Is>
decltype(auto) invoke(Callable&& callable, ::cpp::meta::index_sequence<Is...>, std::vector<AnyArg>& args)
{
    return ::cpp::invoke(
        std::forward<Callable>(callable),
        args[Is].get<
            ::cpp::function_argument<
                Is,
                std::decay_t<Callable>
            >
        >()...
    );
}

template<typename Callable, std::size_t... Is>
decltype(auto) invoke(Callable&& callable, ::cpp::meta::index_sequence<Is...>, std::vector<::cpp::AnyArg>&& args)
{
    return ::cpp::invoke(
        std::forward<Callable>(callable),
        args[Is].get<
            ::cpp::function_argument<
                Is,
                std::decay_t<Callable>
            >
        >()...
    );
}

template<typename Callable, std::size_t... Is, typename Iterator, typename = typename std::iterator_traits<Iterator>::value_type>
decltype(auto) invoke(Callable&& callable, ::cpp::meta::index_sequence<Is...>, Iterator argsBegin)
{
    return ::cpp::invoke(
        std::forward<Callable>(callable),
        (argsBegin + Is)->template get<
            ::cpp::function_argument<
                Is,
                std::decay_t<Callable>
            >
        >()...
    );
}

}

/**
 * \ingroup type-erasure
 * \brief Invokes a callable object with the given type-erased arguments
 *
 * \param callable Callable entity to be invoked
 * \param args Invocation arguments. If \p callable is a pointer to member function,
 * the first argument is interpreted as the caller object
 *
 * See cpp::invoke() for detailed behavior
 */
template<typename Callable, typename... Storages>
decltype(auto) invoke(Callable&& callable, const ::cpp::SimpleAny<Storages>&... args)
{
    return ::cpp::typeerasure::detail::invoke(std::forward<Callable>(callable), ::cpp::meta::make_index_sequence<::cpp::function_arguments<std::decay_t<Callable>>::size>(), args...);
}

/**
 * \ingroup type-erasure
 * \brief Invokes a callable object with the given type-erased arguments
 *
 * \param callable Callable entity to be invoked
 * \param args Invocation arguments. If \p callable is a pointer to member function,
 * the first argument is interpreted as the caller object
 *
 * See cpp::invoke() for detailed behavior
 */
template<typename Callable, typename... Storages>
decltype(auto) invoke(Callable&& callable, ::cpp::SimpleAny<Storages>&... args)
{
    return ::cpp::typeerasure::detail::invoke(std::forward<Callable>(callable), ::cpp::meta::make_index_sequence<::cpp::function_arguments<std::decay_t<Callable>>::size>(), args...);
}

/**
 * \ingroup type-erasure
 * \brief Invokes a callable object with the given type-erased arguments
 *
 * \param callable Callable entity to be invoked
 * \param args Invocation arguments. If \p callable is a pointer to member function,
 * the first argument is interpreted as the caller object
 *
 * See cpp::invoke() for detailed behavior
 */
template<typename Callable, typename... Storages>
decltype(auto) invoke(Callable&& callable, ::cpp::SimpleAny<Storages>&&... args)
{
    return ::cpp::typeerasure::detail::invoke(std::forward<Callable>(callable), ::cpp::meta::make_index_sequence<::cpp::function_arguments<std::decay_t<Callable>>::size>(), std::move(args)...);
}

/**
 * \ingroup type-erasure
 * \brief Invokes a callable object with the given type-erased arguments
 *
 * \param callable Callable entity to be invoked
 * \param args Invocation arguments. If \p callable is a pointer to member function,
 * the first argument is interpreted as the caller object
 *
 * See cpp::invoke() for detailed behavior
 */
template<typename Callable, typename Storage>
decltype(auto) invoke(Callable&& callable, const std::vector<::cpp::SimpleAny<Storage>>& args)
{
    return ::cpp::typeerasure::detail::invoke(std::forward<Callable>(callable), ::cpp::meta::make_index_sequence<::cpp::function_arguments<std::decay_t<Callable>>::size>(), args);
}

/**
 * \ingroup type-erasure
 * \brief Invokes a callable object with the given type-erased arguments
 *
 * \param callable Callable entity to be invoked
 * \param args Invocation arguments. If \p callable is a pointer to member function,
 * the first argument is interpreted as the caller object
 *
 * See cpp::invoke() for detailed behavior
 */
template<typename Callable, typename Storage>
decltype(auto) invoke(Callable&& callable, std::vector<::cpp::SimpleAny<Storage>>& args)
{
    return ::cpp::typeerasure::detail::invoke(std::forward<Callable>(callable), ::cpp::meta::make_index_sequence<::cpp::function_arguments<std::decay_t<Callable>>::size>(), args);
}

/**
 * \ingroup type-erasure
 * \brief Invokes a callable object with the given type-erased arguments
 *
 * \param callable Callable entity to be invoked
 * \param args Invocation arguments. If \p callable is a pointer to member function,
 * the first argument is interpreted as the caller object
 *
 * See cpp::invoke() for detailed behavior
 */
template<typename Callable, typename Storage>
decltype(auto) invoke(Callable&& callable, std::vector<::cpp::SimpleAny<Storage>>&& args)
{
    return ::cpp::typeerasure::detail::invoke(std::forward<Callable>(callable), ::cpp::meta::make_index_sequence<::cpp::function_arguments<std::decay_t<Callable>>::size>(), std::move(args));
}

/**
 * \ingroup type-erasure
 * \brief Invokes a callable object with the given type-erased arguments
 *
 * This overload using cpp::AnyArg objects ensures that reference parameters
 * of the callable get references to the original non-type-erased arguments
 *
 * \param callable Callable entity to be invoked
 * \param args Invocation arguments. If \p callable is a pointer to member function,
 * the first argument is interpreted as the caller object
 *
 * See cpp::invoke() for detailed behavior
 */
template<typename Callable>
decltype(auto) invoke(Callable&& callable, const std::vector<::cpp::AnyArg>& args)
{
    return ::cpp::typeerasure::detail::invoke(std::forward<Callable>(callable), ::cpp::meta::make_index_sequence<::cpp::function_arguments<std::decay_t<Callable>>::size>(), args);
}

/**
 * \ingroup type-erasure
 * \brief Invokes a callable object with the given type-erased arguments
 *
 * This overload using cpp::AnyArg objects ensures that reference parameters
 * of the callable get references to the original non-type-erased arguments
 *
 * \param callable Callable entity to be invoked
 * \param args Invocation arguments. If \p callable is a pointer to member function,
 * the first argument is interpreted as the caller object
 *
 * See cpp::invoke() for detailed behavior
 */
template<typename Callable>
decltype(auto) invoke(Callable&& callable, std::vector<::cpp::AnyArg>& args)
{
    return ::cpp::typeerasure::detail::invoke(std::forward<Callable>(callable), ::cpp::meta::make_index_sequence<::cpp::function_arguments<std::decay_t<Callable>>::size>(), args);
}

/**
 * \ingroup type-erasure
 * \brief Invokes a callable object with the given type-erased arguments
 *
 * This overload using cpp::AnyArg objects ensures that reference parameters
 * of the callable get references to the original non-type-erased arguments
 *
 * \param callable Callable entity to be invoked
 * \param args Invocation arguments. If \p callable is a pointer to member function,
 * the first argument is interpreted as the caller object
 *
 * See cpp::invoke() for detailed behavior
 */
template<typename Callable>
decltype(auto) invoke(Callable&& callable, std::vector<::cpp::AnyArg>&& args)
{
    return ::cpp::typeerasure::detail::invoke(std::forward<Callable>(callable), ::cpp::meta::make_index_sequence<::cpp::function_arguments<std::decay_t<Callable>>::size>(), std::move(args));
}

/**
 * \ingroup type-erasure
 * \brief Invokes a callable object with the given type-erased arguments
 *
 * This overload takes an iterator to a sequence of type-erased arguments.
 *
 * \tparam Iterator Must follow the [`RandomAccessIterator`](http://en.cppreference.com/w/cpp/concept/RandomAccessIterator) concept
 * \param callable Callable entity to be invoked
 * \param argsBegin Iterator pointing to the beginning of the args sequence. If \p callable is a pointer to member function,
 * the first argument is interpreted as the caller object
 *
 * See cpp::invoke() for detailed behavior
 */
template<typename Callable, typename Iterator, typename = typename std::iterator_traits<Iterator>::value_type>
decltype(auto) invoke(Callable&& callable, Iterator argsBegin)
{
    return ::cpp::typeerasure::detail::invoke(std::forward<Callable>(callable), ::cpp::meta::make_index_sequence<::cpp::function_arguments<std::decay_t<Callable>>::size>(), argsBegin);
}

}

}
#endif // SIPLASPLAS_TYPEERASURE_INVOKE_HPP
