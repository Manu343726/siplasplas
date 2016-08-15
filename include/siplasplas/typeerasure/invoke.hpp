#ifndef SIPLASPLAS_TYPEERASURE_INVOKE_HPP
#define SIPLASPLAS_TYPEERASURE_INVOKE_HPP

#include "simpleany.hpp"
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

template<typename Callable, typename... Storages, std::size_t... Is>
auto invoke(Callable&& callable, ::cpp::meta::index_sequence<Is...>, const ::cpp::SimpleAny<Storages>&... args)
{
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
auto invoke(Callable&& callable, ::cpp::meta::index_sequence<Is...>, ::cpp::SimpleAny<Storages>&... args)
{
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
auto invoke(Callable&& callable, ::cpp::meta::index_sequence<Is...>, ::cpp::SimpleAny<Storages>&&... args)
{
    return ::cpp::invoke(
        std::forward<Callable>(callable),
        std::move(args.template get<
            ::cpp::function_argument<
                Is,
                std::decay_t<Callable>
            >
        >())...
    );
}

template<typename Callable, typename Storage, std::size_t... Is>
auto invoke(Callable&& callable, ::cpp::meta::index_sequence<Is...>, const std::vector<::cpp::SimpleAny<Storage>>& args)
{
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
auto invoke(Callable&& callable, ::cpp::meta::index_sequence<Is...>, std::vector<::cpp::SimpleAny<Storage>>& args)
{
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
auto invoke(Callable&& callable, ::cpp::meta::index_sequence<Is...>, std::vector<::cpp::SimpleAny<Storage>>&& args)
{
    return ::cpp::invoke(
        std::forward<Callable>(callable),
        std::move(
            args[Is].template get<
                ::cpp::function_argument<
                    Is,
                    std::decay_t<Callable>
                >
            >()
        )...
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
auto invoke(Callable&& callable, const ::cpp::SimpleAny<Storages>&... args)
{
    return detail::invoke(std::forward<Callable>(callable), ::cpp::meta::make_index_sequence<::cpp::function_arguments<std::decay_t<Callable>>::size>(), args...);
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
auto invoke(Callable&& callable, ::cpp::SimpleAny<Storages>&... args)
{
    return detail::invoke(std::forward<Callable>(callable), ::cpp::meta::make_index_sequence<::cpp::function_arguments<std::decay_t<Callable>>::size>(), args...);
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
auto invoke(Callable&& callable, ::cpp::SimpleAny<Storages>&&... args)
{
    return detail::invoke(std::forward<Callable>(callable), ::cpp::meta::make_index_sequence<::cpp::function_arguments<std::decay_t<Callable>>::size>(), std::move(args)...);
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
auto invoke(Callable&& callable, const std::vector<::cpp::SimpleAny<Storage>>& args)
{
    return detail::invoke(std::forward<Callable>(callable), ::cpp::meta::make_index_sequence<::cpp::function_arguments<std::decay_t<Callable>>::size>(), args);
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
auto invoke(Callable&& callable, std::vector<::cpp::SimpleAny<Storage>>& args)
{
    return detail::invoke(std::forward<Callable>(callable), ::cpp::meta::make_index_sequence<::cpp::function_arguments<std::decay_t<Callable>>::size>(), args);
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
auto invoke(Callable&& callable, std::vector<::cpp::SimpleAny<Storage>>&& args)
{
    return detail::invoke(std::forward<Callable>(callable), ::cpp::meta::make_index_sequence<::cpp::function_arguments<std::decay_t<Callable>>::size>(), std::move(args));
}

}

}
#endif // SIPLASPLAS_TYPEERASURE_INVOKE_HPP
