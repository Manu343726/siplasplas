#ifndef SIPLASPLAS_UTILITY_CLONINGPTR_HPP
#define SIPLASPLAS_UTILITY_CLONINGPTR_HPP

#include "clonable.hpp"

namespace cpp
{

template<typename T>
class CloningPtr
{
public:
    CloningPtr() = default;

    CloningPtr(T* object) :
        _object{object}
    {}

    template<typename U, typename = std::enable_if_t<std::is_base_of<T, U>::value>>
    CloningPtr(const CloningPtr<U>& other) :
        _object{other.clone()}
    {}

    template<typename U, typename = std::enable_if_t<std::is_base_of<T, U>::value>>
    CloningPtr(CloningPtr<U>&& other) :
        _object{other._object}
    {
        other._object = nullptr;
    }

    template<typename U, typename = std::enable_if_t<std::is_base_of<T, U>::value>>
    CloningPtr& operator=(const CloningPtr<U>& other)
    {
        destroy();
        _object = other.clone();
        return *this;
    }

    template<typename U, typename = std::enable_if_t<std::is_base_of<T, U>::value>>
    CloningPtr& operator=(CloningPtr<U>&& other)
    {
        destroy();
        _object = other._object;
        other._object = nullptr;
        return *this;
    }

    ~CloningPtr()
    {
        destroy();
    }

    T* get() const
    {
        return _object;
    }

    T& operator*()
    {
        return *get();
    }

    const T& operator*() const
    {
        return *get();
    }

    T* operator->()
    {
        return get();
    }

    const T* operator->() const
    {
        return get();
    }

    friend bool operator==(const CloningPtr& lhs, const CloningPtr& rhs)
    {
        return lhs.get() == rhs.get();
    }

    friend bool operator!=(const CloningPtr& lhs, const CloningPtr& rhs)
    {
        return !(lhs == rhs);
    }

private:
    T* _object = nullptr;

    template<typename U, typename... Args>
    T* allocate(Args&&... args)
    {
        return new U(std::forward<Args>(args)...);
    }

    void deallocate(T* object)
    {
        delete object;
    }

    T* clone()
    {
        return _object->clone();
    }

    void destroy()
    {
        deallocate(_object);
    }
};

using ClonablePtr = CloningPtr<Clonable>;

}

#endif // SIPLASPLAS_UTILITY_CLONINGPTR_HPP
