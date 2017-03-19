#ifndef SIPLASPLAS_UTILITY_STATICVECTOR_HPP
#define SIPLASPLAS_UTILITY_STATICVECTOR_HPP

#include "destroy.hpp"
#include <vector>
#include <type_traits>

namespace cpp
{

template<typename T, typename Allocator = std::allocator<T>>
class StaticVector
{
    using Storage = std::aligned_storage_t<sizeof(T), alignof(T)>;
    using Vector  = std::vector<Storage, Allocator>;
    using VectorIterator = typename Vector::iterator;
    using ConstVectorIterator = typename Vector::const_iterator;

public:
    explicit StaticVector(std::size_t maxSize) :
        _storage{maxSize},
        _size{0}
    {}

    StaticVector(const StaticVector&) = delete;
    StaticVector(StaticVector&&) = delete;
    StaticVector& operator=(const StaticVector&) = delete;
    StaticVector& operator=(StaticVector&&) = delete;

    std::size_t size() const
    {
        return _size;
    }

    template<typename... Args>
    bool emplaceBack(Args&&... args)
    {
        if(_size < _storage.size())
        {
            auto* storage = &_storage[_size++];
            cpp::construct<T>(storage, std::forward<Args>(args)...);

            return true;
        }
        else
        {
            return false;
        }
    }

    template<typename ValueType, typename StorageIterator>
    class Iterator
    {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = ValueType;
        using reference_type = std::add_lvalue_reference_t<ValueType>;
        using pointer_type = std::add_pointer_t<ValueType>;

        explicit Iterator(StorageIterator storageIterator) :
            _storageIterator{storageIterator}
        {}

        reference_type operator*() const
        {
            return reinterpret_cast<reference_type>(*_storageIterator);
        }

        Iterator& operator++()
        {
            ++_storageIterator;
            return *this;
        }

        Iterator operator++(int)
        {
            return {_storageIterator++};
        }

        Iterator& operator+=(std::ptrdiff_t distance)
        {
            _storageIterator += distance;
            return *this;
        }

        Iterator& operator--()
        {
            --_storageIterator;
            return *this;
        }

        Iterator operator--(int)
        {
            return {_storageIterator--};
        }

        Iterator& operator-=(std::ptrdiff_t distance)
        {
            _storageIterator -= distance;
            return *this;
        }

        friend bool operator==(Iterator lhs, Iterator rhs)
        {
            return lhs._storageIterator == rhs._storageIterator;
        }

        friend bool operator!=(Iterator lhs, Iterator rhs)
        {
            return !(lhs == rhs);
        }

        friend Iterator operator+(Iterator it, std::ptrdiff_t distance)
        {
            return it += distance;
        }

        friend Iterator operator-(Iterator it, std::ptrdiff_t distance)
        {
            return it -= distance;
        }

    private:
        StorageIterator _storageIterator;
    };

    using iterator = Iterator<T, VectorIterator>;
    using const_iterator = Iterator<const T, ConstVectorIterator>;

    iterator begin()
    {
        return iterator{_storage.begin()};
    }

    const_iterator begin() const
    {
        return const_iterator{_storage.begin()};
    }

    iterator end()
    {
        return iterator{_storage.end()};
    }

    const_iterator end() const
    {
        return const_iterator{_storage.end()};
    }

    const_iterator cbegin() const
    {
        return const_iterator{_storage.cbegin()};
    }

    const_iterator cend() const
    {
        return const_iterator{_storage.cend()};
    }

    const T& operator[](std::size_t i) const
    {
        return *reinterpret_cast<const T*>(&_storage[i]);
    }

    T& operator[](std::size_t i)
    {
        return *reinterpret_cast<T*>(&_storage[i]);
    }

    ~StaticVector()
    {
        for(std::size_t i = 0; i < _size; ++i)
        {
            cpp::destroy<T>(&_storage[i]);
        }
    }

private:
    Vector _storage;
    std::size_t _size;
};

}

#endif // SIPLASPLAS_UTILITY_STATICVECTOR_HPP
