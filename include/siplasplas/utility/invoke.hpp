#ifndef SIPLASPLAS_UTILITY_INVOKE_HPP
#define SIPLASPLAS_UTILITY_INVOKE_HPP

#include "function_traits.hpp"
#include "logger.hpp"
#include <utility>
#include <functional>
#include <iostream>

namespace cpp
{

namespace detail
{
    template<typename Class, typename Object,
        bool IsBase = std::is_base_of<Class, Object>::value
    >
    class InvokeMemberFunction
    {
    public:
        template<typename Function, typename Object_, typename... Args>
        static decltype(auto) apply(Function function, Object_&& object, Args&&... args)
        {
            constexpr ctti::type_id_t typeIds[] = { ctti::type_id<decltype(std::forward<Args>(args))>()..., ctti::type_id<void>() };

            utility::log().debug("About to invoke member function:");
            utility::log().debug("  Function: {}", ctti::type_id<Function>().name());
            utility::log().debug("  Object: {}", ctti::type_id<decltype(std::forward<Object_>(object))>().name());

            for(std::size_t i = 0; i < sizeof...(args); ++i)
            {
                utility::log().debug("  #{} {}", i, typeIds[i].name());
            }

            return (std::forward<Object_>(object).*function)(std::forward<Args>(args)...);
        }
    };

    template<typename Class, typename T>
    class InvokeMemberFunction<Class, std::reference_wrapper<T>, false>
    {
    public:
        template<typename Function, typename Reference_, typename... Args>
        static decltype(auto) apply(Function function, Reference_&& reference, Args&&... args)
        {
            return ((std::forward<Reference_>(reference).get()).*function)(std::forward<Args>(args)...);
        }
    };

    template<typename Class, typename Object>
    class InvokeMemberFunction<Class, Object, false>
    {
    public:
        template<typename Function, typename Pointer_, typename... Args>
        static decltype(auto) apply(Function function, Pointer_&& pointer, Args&&... args)
        {
            return ((*std::forward<Pointer_>(pointer)).*function)(std::forward<Args>(args)...);
        }
    };

    template<typename Class, typename Object,
        bool IsBase = std::is_base_of<Class, Object>::value
    >
    class InvokeMemberObject
    {
    public:
        template<typename Type>
        static const Type& apply(Type (Class::*memberObject), const Object& object)
        {
            return object.*memberObject;
        }

        template<typename Type>
        static Type& apply(Type (Class::*memberObject), Object& object)
        {
            return object.*memberObject;
        }
    };

    template<typename Class, typename T>
    class InvokeMemberObject<Class, std::reference_wrapper<T>, false>
    {
    public:
        template<typename Type>
        static const Type& apply(Type (Class::*memberObject), const std::reference_wrapper<T>& object)
        {
            return object.get().*memberObject;
        }

        template<typename Type>
        static Type& apply(Type (Class::*memberObject), std::reference_wrapper<T>& object)
        {
            return object.get().*memberObject;
        }
    };

    template<typename Class, typename Object>
    class InvokeMemberObject<Class, Object, false>
    {
    public:
        template<typename Type>
        static const Type& apply(Type (Class::*memberObject), const Object& object)
        {
            return object->*memberObject;
        }

        template<typename Type>
        static Type& apply(Type (Class::*memberObject), Object& object)
        {
            return object->*memberObject;
        }
    };

    template<typename T, bool IsMemberObjectPointer =
        std::is_member_object_pointer<T>::value
    >
    class InvokeMember
    {
    public:
        template<typename R, typename Class, typename Object, typename... Tail>
        static decltype(auto) apply(R Class::* pointer, Object&& object, Tail&&...)
        {
            return InvokeMemberObject<Class, std::decay_t<Object>>::apply(pointer, std::forward<Object>(object));
        }
    };

    template<typename T>
    class InvokeMember<T, false>
    {
    public:
        template<typename R, typename Class, typename... Params, typename Object, typename... Args>
        static decltype(auto) apply(R(Class::*pointer)(Params...), Object&& object, Args&&... args)
        {
            return InvokeMemberFunction<Class, std::decay_t<Object>>::apply(pointer, std::forward<Object>(object), std::forward<Args>(args)...);
        }

        template<typename R, typename Class, typename... Params, typename Object, typename... Args>
        static decltype(auto) apply(R(Class::*pointer)(Params...) const, Object&& object, Args&&... args)
        {
            return InvokeMemberFunction<Class, std::decay_t<Object>>::apply(pointer, std::forward<Object>(object), std::forward<Args>(args)...);
        }

    };
}

/**
 * \ingroup utility
 * \brief Invokes the [`Callable`](http://en.cppreference.com/w/cpp/concept/Callable) object with the given arguments.
 * See [`std::invoke()`](http://en.cppreference.com/w/cpp/utility/functional/invoke)
 */
template<typename Callable, typename... Args>
decltype(auto) invoke(Callable&& callable, Args&&... args)
{
    return std::forward<Callable>(callable)(std::forward<Args>(args)...);
}

template<typename R, typename Class, typename... Args>
decltype(auto) invoke(R Class::*pointer, Args&&... args)
{
    return ::cpp::detail::InvokeMember<decltype(pointer)>::apply(pointer, std::forward<Args>(args)...);
}

template<typename R, typename Class, typename... Params, typename... Args>
decltype(auto) invoke(R (Class::*pointer)(Params...) const, Args&&... args)
{
    return ::cpp::detail::InvokeMember<decltype(pointer)>::apply(pointer, std::forward<Args>(args)...);
}

}

#endif // SIPLASPLAS_UTILITY_INVOKE_HPP
