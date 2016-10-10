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

class RemoveConst
{
public:
    template<typename T, bool IsRvalueReference = std::is_rvalue_reference<T>::value>
    class Apply
    {
    public:
        using Type = std::decay_t<T>;

        static constexpr Type& apply(const Type& value)
        {
            return const_cast<Type&>(value);
        }
    };

    template<typename T>
    class Apply<T, true>
    {
    public:
        using Type = std::decay_t<T>;

        static constexpr Type&& apply(Type&& value)
        {
            return std::move(value);
        }
    };

    template<typename T>
    constexpr decltype(auto) operator()(T&& value) const
    {
        return Apply<decltype(value)>::apply(std::forward<T>(value));
    }
};

template<typename Indices, typename ArgsPolicy = cpp::Identity>
class Invoke;

template<std::size_t Head, std::size_t... Tail, typename ArgsPolicy>
class Invoke<::cpp::meta::index_sequence<Head, Tail...>, ArgsPolicy>
{
public:
    template<typename Callable, typename Iterator>
    static decltype(auto) apply_sequence(Callable&& callable, Iterator argsBegin)
    {
        return ::cpp::invoke(
            std::forward<Callable>(callable),
            ArgsPolicy()((argsBegin + Head)->template get<
                ::cpp::function_argument<
                    Head,
                    std::decay_t<Callable>
                >
            >()),
            ArgsPolicy()((argsBegin + Tail)->template get<
                ::cpp::function_argument<
                    Tail,
                    std::decay_t<Callable>
                >
            >())...
        );
    }

    template<typename Callable, typename HeadArg, typename... TailArgs>
    static decltype(auto) apply_pack(Callable&& callable, HeadArg&& head, TailArgs&&... tail)
    {
        return ::cpp::invoke(
            std::forward<Callable>(callable),
            ArgsPolicy()(std::forward<HeadArg>(head).template get<
                ::cpp::function_argument<
                    Head,
                    std::decay_t<Callable>
                >
            >()),
            ArgsPolicy()(std::forward<TailArgs>(tail).template get<
                ::cpp::function_argument<
                    Tail,
                    std::decay_t<Callable>
                >
            >())...
        );
    }
};

template<typename ArgsPolicy>
class Invoke<::cpp::meta::index_sequence<>, ArgsPolicy>
{
public:
    template<typename Callable, typename Iterator>
    static decltype(auto) apply_sequence(Callable&& callable, Iterator argsBegin)
    {
        return ::cpp::invoke(std::forward<Callable>(callable));
    }

    template<typename Callable, typename... Args>
    static decltype(auto) apply_pack(Callable&& callable, Args&&...)
    {
        return ::cpp::invoke(std::forward<Callable>(callable));
    }
};

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
    return ::cpp::typeerasure::detail::Invoke<
        ::cpp::meta::make_index_sequence<::cpp::function_arguments<std::decay_t<Callable>>::size>
    >::apply_pack(std::forward<Callable>(callable), args...);
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
    return ::cpp::typeerasure::detail::Invoke<
        ::cpp::meta::make_index_sequence<::cpp::function_arguments<std::decay_t<Callable>>::size>
    >::apply_pack(std::forward<Callable>(callable), args...);
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
    return ::cpp::typeerasure::detail::Invoke<
        ::cpp::meta::make_index_sequence<::cpp::function_arguments<std::decay_t<Callable>>::size>
    >::apply_pack(std::forward<Callable>(callable), std::move(args)...);
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
    return ::cpp::typeerasure::detail::Invoke<
        ::cpp::meta::make_index_sequence<::cpp::function_arguments<std::decay_t<Callable>>::size>
    >::apply_sequence(std::forward<Callable>(callable), std::begin(args));
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
    return ::cpp::typeerasure::detail::Invoke<
        ::cpp::meta::make_index_sequence<::cpp::function_arguments<std::decay_t<Callable>>::size>
    >::apply_sequence(std::forward<Callable>(callable), std::begin(args));
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
    return ::cpp::typeerasure::detail::Invoke<::cpp::meta::make_index_sequence<
        ::cpp::function_arguments<std::decay_t<Callable>>::size>,
        ::cpp::typeerasure::detail::RemoveConst
    >::apply_sequence(std::forward<Callable>(callable), std::begin(std::move(args)));
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
    return ::cpp::typeerasure::detail::Invoke<
        ::cpp::meta::make_index_sequence<::cpp::function_arguments<std::decay_t<Callable>>::size>
    >::apply_sequence(std::forward<Callable>(callable), std::begin(args));
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
    return ::cpp::typeerasure::detail::Invoke<
        ::cpp::meta::make_index_sequence<::cpp::function_arguments<std::decay_t<Callable>>::size>
    >::apply_sequence(std::forward<Callable>(callable), std::begin(args));
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
    return ::cpp::typeerasure::detail::Invoke<
        ::cpp::meta::make_index_sequence<::cpp::function_arguments<std::decay_t<Callable>>::size>,
        ::cpp::typeerasure::detail::RemoveConst
    >::apply_sequence(std::forward<Callable>(callable), std::begin(std::move(args)));
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
    return ::cpp::typeerasure::detail::Invoke<
        ::cpp::meta::make_index_sequence<::cpp::function_arguments<std::decay_t<Callable>>::size>
    >::apply_sequence(std::forward<Callable>(callable), argsBegin);
}

}

}
#endif // SIPLASPLAS_TYPEERASURE_INVOKE_HPP
