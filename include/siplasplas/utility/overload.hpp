#ifndef SIPLASPLAS_UTILITY_OVERLOAD_HPP
#define SIPLASPLAS_UTILITY_OVERLOAD_HPP

namespace cpp
{

template<typename... Args>
class Overload
{
private:
    template<typename R>
    using function_pointer_t = R(*)(Args...);

    template<typename R, typename Class>
    using member_function_pointer_t = R(Class::*)(Args...);

    template<typename R, typename Class>
    using const_member_function_pointer_t = R(Class::*)(Args...) const;

public:
    template<typename R>
    static constexpr function_pointer_t<R> get(R(*fptr)(Args...))
    {
        return fptr;
    }

    template<typename R, typename Class>
    static constexpr member_function_pointer_t<R, Class> get(R(Class::*fptr)(Args...))
    {
        return fptr;
    }

    template<typename R, typename Class>
    static constexpr const_member_function_pointer_t<R, Class> get(R(Class::*fptr)(Args...) const)
    {
        return fptr;
    }
};

}

#endif // SIPLASPLAS_UTILITY_OVERLOAD_HPP
