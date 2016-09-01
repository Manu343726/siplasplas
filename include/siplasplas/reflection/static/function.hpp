#ifndef SIPLASPLAS_REFLECTION_STATIC_FUNCTION_HPP
#define SIPLASPLAS_REFLECTION_STATIC_FUNCTION_HPP

namespace cpp
{
namespace static_reflection
{
namespace meta
{

template<typename SourceInfo, typename FunctionType, FunctionType function>
class Function;

template<typename SourceInfo_, typename R, typename... Args,
    R(*function)(Args...)>
class Function<SourceInfo_, R(*)(Args...), function>
{
public:
    using type = R(*)(Args...);
    using SourceInfo = SourceInfo_;

    constexpr Function() = default;

    template<typename... Args_>
    static constexpr R invoke(Args_&&... args)
    {
        return function(std::forward<Args_>(args)...);
    }

    template<typename... Args_>
    constexpr R operator()(Args_&&... args) const
    {
        return invoke(std::forward<Args_>(args)...);
    }

    static constexpr type get()
    {
        return function;
    }
};

template<typename SourceInfo_, typename R, typename Class, typename... Args,
    R(Class::*method)(Args...) const>
class Function<SourceInfo_, R(Class::*)(Args...) const, method>
{
public:
    using type = R(Class::*)(Args...) const;
    using class_type = Class;
    using SourceInfo = SourceInfo_;

    constexpr Function() = default;

    template<typename... Args_>
    static constexpr R invoke(const Class& object, Args_&&... args)
    {
        return object.*method(std::forward<Args_>(args)...);
    }

    template<typename... Args_>
    constexpr R operator()(const Class& object, Args_&&... args) const
    {
        return invoke(object, std::forward<Args_>(args)...);
    }

    static constexpr type get()
    {
        return method;
    }
};

template<typename SourceInfo_, typename R, typename Class, typename... Args,
    R(Class::*method)(Args...)>
class Function<SourceInfo_, R(Class::*)(Args...), method>
{
public:
    using type = R(Class::*)(Args...);
    using class_type = Class;
    using SourceInfo = SourceInfo_;

    constexpr Function() = default;

    template<typename... Args_>
    static constexpr R invoke(Class& object, Args_&&... args)
    {
        return (object.*method)(std::forward<Args_>(args)...);
    }

    template<typename... Args_>
    static constexpr R invoke(const Class& object, Args_&&... args)
    {
        return (object.*method)(std::forward<Args_>(args)...);
    }

    template<typename... Args_>
    constexpr R operator()(Class& object, Args_&&... args) const
    {
        return invoke(object, std::forward<Args_>(args)...);
    }

    static constexpr type get()
    {
        return method;
    }
};

} // namespace meta

namespace codegen
{

template<typename FunctionType, FunctionType function>
class Function :
    public static_reflection::meta::Function<
        static_reflection::meta::EmptySourceInfo<Function<FunctionType, function>>,
        FunctionType,
        function
    >
{};

template<typename... Fs>
class OverloadedFunction;

template<typename Head>
class OverloadedFunction<Head> : public Head
{};

template<typename Head, typename Second, typename... Tail>
class OverloadedFunction<Head, Second, Tail...> : public Head, public OverloadedFunction<Second, Tail...>
{
public:
    using class_type = typename Head::class_type;
    using Head::invoke;
    using Head::operator();
    using OverloadedFunction<Second, Tail...>::invoke;
    using OverloadedFunction<Second, Tail...>::operator();
};

template<typename Method>
class BindedMethod
{
public:
    BindedMethod(typename Method::class_type& object) :
        _object{&object}
    {}

    template<typename... Args>
    auto operator()(Args&&... args) -> decltype(Method::invoke(*std::declval<typename Method::class_type*>(), std::forward<Args>(args)...))
    {
        return Method::invoke(*_object, std::forward<Args>(args)...);
    }

private:
    typename Method::class_type* _object;
};

template<typename Method>
class ConstBindedMethod
{
public:
    ConstBindedMethod(const typename Method::class_type& object) :
        _object{&object}
    {}

    template<typename... Args>
    auto operator()(Args&&... args) -> decltype(Method::invoke(*std::declval<const typename Method::class_type*>(), std::forward<Args>(args)...))
    {
        return Method::invoke(*_object, std::forward<Args>(args)...);
    }

private:
    const typename Method::class_type* _object;
};

} // namespace codegen

template<typename FunctionType, FunctionType function>
class Function : public codegen::Function<FunctionType, function>
{};

} // namespace static_reflection
} // namespace cpp
#endif // SIPLASPLAS_REFLECTION_STATIC_FUNCTION_HPP
