#ifndef SIPLASPLAS_UTILITY_ARRAYVIEW_HPP
#define SIPLASPLAS_UTILITY_ARRAYVIEW_HPP

#include "constexpralgorithms.hpp"
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

template<typename T>
std::ostream& operator<<(std::ostream& os, const ArrayView<T>& arrayView)
{
    os << "{";

    for(std::size_t i = 0; i < arrayView.size(); ++i)
    {
        if(i < arrayView.size() - 1)
        {
            os << arrayView[i] << ", ";
        }
        else
        {
            os << arrayView[i];
        }
    }

    return os << "}";
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const ConstArrayView<T>& arrayView)
{
    os << "{";

    for(std::size_t i = 0; i < arrayView.size(); ++i)
    {
        if(i < arrayView.size() - 1)
        {
            os << arrayView[i] << ", ";
        }
        else
        {
            os << arrayView[i];
        }
    }

    return os << "}";
}

inline std::ostream& operator<<(std::ostream& os, const ArrayView<char>& arrayView)
{
    for(const char c : arrayView)
    {
        os << c;
    }

    return os;
}

inline std::ostream& operator<<(std::ostream& os, const ConstArrayView<char>& arrayView)
{
    for(const char c : arrayView)
    {
        os << c;
    }

    return os;
}

template<typename T, typename U>
constexpr bool operator==(const ArrayView<T>& lhs, const ArrayView<U>& rhs)
{
    return cpp::equal(
        lhs.begin(), lhs.end(),
        rhs.begin(), rhs.end()
    );
}

template<typename T, typename U>
constexpr bool operator==(const ConstArrayView<T>& lhs, const ArrayView<U>& rhs)
{
    return cpp::equal(
        lhs.begin(), lhs.end(),
        rhs.begin(), rhs.end()
    );
}

template<typename T, typename U>
constexpr bool operator==(const ConstArrayView<T>& lhs, const ConstArrayView<U>& rhs)
{
    return cpp::equal(
        lhs.begin(), lhs.end(),
        rhs.begin(), rhs.end()
    );
}

template<typename T, typename U>
constexpr bool operator==(const ArrayView<T>& lhs, const ConstArrayView<U>& rhs)
{
    return cpp::equal(
        lhs.begin(), lhs.end(),
        rhs.begin(), rhs.end()
    );
}

template<typename T, typename U>
constexpr bool operator!=(const ArrayView<T>& lhs, const ArrayView<U>& rhs)
{
    return !(lhs == rhs);
}

template<typename T, typename U>
constexpr bool operator!=(const ConstArrayView<T>& lhs, const ArrayView<U>& rhs)
{
    return !(lhs == rhs);
}

template<typename T, typename U>
constexpr bool operator!=(const ConstArrayView<T>& lhs, const ConstArrayView<U>& rhs)
{
    return !(lhs == rhs);
}

template<typename T, typename U>
constexpr bool operator!=(const ArrayView<T>& lhs, const ConstArrayView<U>& rhs)
{
    return !(lhs == rhs);
}

template<typename T, std::size_t N>
constexpr ArrayView<T> arrayView(const T (&array)[N])
{
    return { array };
}

template<typename T, std::size_t N>
constexpr ConstArrayView<T> constArrayView(const T (&array)[N])
{
    return { array };
}

class StringView : public ArrayView<char>
{
public:
    using ArrayView<char>::ArrayView;

    constexpr StringView(const ArrayView<char>& arrayView) :
        ArrayView<char>{arrayView}
    {}

    constexpr StringView(ArrayView<char>&& arrayView) :
        ArrayView<char>{std::move(arrayView)}
    {}

    std::string str() const
    {
        /*
         * The range std::string constructor takes the string as is, so passing
         * a range including a null terminator at the end (Such as when passing an
         * string literal to a [Const]ArrayView) leads to a string with a
         * \0 character at the end. Instead, return a range without that final
         * null terminator so std::string gets the right string
         */
        if((*this)[size() - 1] == '\0')
        {
            return { begin(), end() - 1};
        }
        else
        {
            return { begin(), end() };
        }
    }

    const char* c_str() const
    {
        return begin();
    }
};

class ConstStringView : public ConstArrayView<char>
{
public:
    using ConstArrayView<char>::ConstArrayView;

    constexpr ConstStringView(const ConstArrayView<char>& arrayView) :
        ConstArrayView<char>{arrayView}
    {}

    constexpr ConstStringView(ConstArrayView<char>&& arrayView) :
        ConstArrayView<char>{std::move(arrayView)}
    {}

    std::string str() const
    {
        /*
         * The range std::string constructor takes the string as is, so passing
         * a range including a null terminator at the end (Such as when passing an
         * string literal to a [Const]ArrayView) leads to a string with a
         * \0 character at the end. Instead, return a range without that final
         * null terminator so std::string gets the right string
         */
        if((*this)[size() - 1] == '\0')
        {
            return { begin(), end() - 1};
        }
        else
        {
            return { begin(), end() };
        }
    }

    const char* c_str() const
    {
        return begin();
    }
};

}

#endif // SIPLASPLAS_UTILITY_ARRAYVIEW_HPP
