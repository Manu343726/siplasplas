#ifndef SIPLASPLAS_UTILITY_ARRAYVIEW_HPP
#define SIPLASPLAS_UTILITY_ARRAYVIEW_HPP

#include <array>

namespace cpp
{

template<typename T>
class ArrayView
{
public:
    constexpr ArrayView(T* begin, std::size_t size) :
        _begin{begin},
        _size{size}
    {}

    constexpr ArrayView(T* begin, T* end) :
        ArrayView{begin, static_cast<std::size_t>(end - begin)}
    {}

    template<std::size_t N>
    constexpr ArrayView(const T (&array)[N]) :
        ArrayView{array, N}
    {}

    template<std::size_t N>
    constexpr ArrayView(const std::array<T, N>& array) :
        ArrayView{array.begin(), N}
    {}

    constexpr std::size_t size() const
    {
        return _size;
    }

    constexpr T* begin() const
    {
        return _begin;
    }

    constexpr T* end() const
    {
        return _begin + _size;
    }

    constexpr T* const cbegin() const
    {
        return begin();
    }

    constexpr T* const cend() const
    {
        return end();
    }

    constexpr ArrayView operator()(std::size_t begin, std::size_t end) const
    {
        return { _begin + begin, _begin + end };
    }

    constexpr T* operator()(std::size_t i) const
    {
        return _begin + i;
    }

    constexpr T operator[](std::size_t i) const
    {
        return _begin[i];
    }

private:
    T* _begin;
    std::size_t _size;
};

template<typename T>
class ConstArrayView
{
public:
    constexpr ConstArrayView(const T* begin, std::size_t size) :
        _begin{begin},
        _size{size}
    {}

    constexpr ConstArrayView(const T* begin, const T* end) :
        ConstArrayView{begin, static_cast<std::size_t>(end - begin)}
    {}

    template<std::size_t N>
    constexpr ConstArrayView(const T (&array)[N]) :
        ConstArrayView{array, N}
    {}

    template<std::size_t N>
    constexpr ConstArrayView(const std::array<T, N>& array) :
        ConstArrayView{array.begin(), N}
    {}

    constexpr std::size_t size() const
    {
        return _size;
    }

    constexpr const T* begin() const
    {
        return _begin;
    }

    constexpr const T* end() const
    {
        return _begin + _size;
    }

    constexpr const T* const cbegin() const
    {
        return begin();
    }

    constexpr const T* const cend() const
    {
        return end();
    }

    constexpr ConstArrayView operator()(std::size_t begin, std::size_t end) const
    {
        return { _begin + begin, _begin + end };
    }

    constexpr const T* operator()(std::size_t i) const
    {
        return _begin + i;
    }

    constexpr T operator[](std::size_t i) const
    {
        return _begin[i];
    }

private:
    const T* _begin;
    std::size_t _size;
};
}

#endif // SIPLASPLAS_UTILITY_ARRAYVIEW_HPP
