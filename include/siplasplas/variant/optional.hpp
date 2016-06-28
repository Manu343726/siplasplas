#ifndef SIPLASPLAS_VARIANT_OPTIONAL_HPP
#define SIPLASPLAS_VARIANT_OPTIONAL_HPP

#include "variant.hpp"

namespace cpp
{

template<typename T>
class Optional
{
public:
    Optional() = default;

    template<typename... Args>
    Optional(Args&&... args) :
        _variant{T{std::forward<Args>(args)...}}
    {}

    template<typename U>
    Optional(const Optional<U>& other) :
        Optional{other.empty() ? Optional() : other.get()}
    {}

    template<typename U>
    Optional(Optional<U>&& other) :
        Optional{other.empty() ? Optional() : std::move(std::move(other).get())}
    {}

    template<typename U>
    Optional& operator=(U&& value)
    {
        _variant = T{std::forward<U>(value)};
        return *this;
    }

    template<typename U>
    Optional& operator=(const Optional<U>& other)
    {
        if(other.empty())
        {
            clear();
        }
        else
        {
            _variant = other.get();
        }

        return *this;
    }

    template<typename U>
    Optional& operator=(Optional<U>&& other)
    {
        if(other.empty())
        {
            clear();
        }
        else
        {
            _variant = std::move(std::move(other).get());
        }

        return *this;
    }

    const T& get() const
    {
        return _variant.template get<T>();
    }

    T& get()
    {
        return _variant.template get<T>();
    }

    const T* operator->() const
    {
        return &get();
    }

    T* operator->()
    {
        return &get();
    }

    void clear()
    {
        _variant.clear();
    }

    bool empty() const
    {
        return _variant.empty();
    }

    explicit operator bool() const
    {
        return !empty();
    }

private:
    cpp::Variant<T> _variant;
};

class Nothing
{
public:
    template<typename T>
    operator Optional<T>()
    {
        return Optional<T>();
    }
};

template<typename T>
Optional<std::decay_t<T>> just(T&& value)
{
    return {value};
}

inline Nothing nothing()
{
    return {};
}

}

#endif // SIPLASPLAS_VARIANT_OPTIONAL_HPP
