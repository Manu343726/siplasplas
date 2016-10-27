#ifndef SIPLASPLAS_CONSTEXPR_STRING_HPP
#define SIPLASPLAS_CONSTEXPR_STRING_HPP

#include "stringview.hpp"
#include "algorithm.hpp"
#include <siplasplas/utility/meta.hpp>
#include <siplasplas/utility/cast.hpp>
#include <array>

namespace cpp
{

namespace constexp
{

template<std::size_t Size>
class String
{
public:
    template<typename Begin, typename End>
    constexpr String(Begin begin, End end);

    constexpr String(const std::initializer_list<char>& initList);

    template<std::size_t StringSize>
    constexpr String(const char(&string)[StringSize]);

    constexpr char operator[](std::size_t i) const;
    constexpr std::size_t size() const;

    constexpr const char* begin() const;
    constexpr const char* end() const;
    constexpr ConstStringView view() const;
    constexpr ConstStringView operator()(std::size_t i, std::size_t j) const;

    std::string str() const;
    constexpr const char* c_str() const;

private:
    std::array<char, Size> _storage;
};

namespace detail
{

template<typename Begin, typename End, std::size_t... Is>
constexpr std::array<char, sizeof...(Is)> makeArray(Begin begin, End end, cpp::meta::index_sequence<Is...>)
{
    return {{
        (*(begin + Is))...
    }};
}

}

template<std::size_t Size>
template<typename Begin, typename End>
constexpr String<Size>::String(Begin begin, End end) :
    _storage{detail::makeArray(begin, end, cpp::meta::make_index_sequence<Size>())}
{}

template<std::size_t Size>
constexpr String<Size>::String(const std::initializer_list<char>& initList) :
    String{initList.begin(), initList.end()}
{}

template<std::size_t Size>
template<std::size_t StringSize>
constexpr String<Size>::String(const char (&string)[StringSize]) :
    String{cpp::constexp::begin(string), cpp::constexp::end(string)}
{
    static_assert(Size >= StringSize, "Not enough storage for the given string literal");
}

template<std::size_t Size>
constexpr char String<Size>::operator[](std::size_t i) const
{
    return _storage[i];
}

template<std::size_t Size>
constexpr std::size_t String<Size>::size() const
{
    return Size;
}

template<std::size_t Size>
constexpr const char* String<Size>::begin() const
{
    return &_storage[0];
}

template<std::size_t Size>
constexpr const char* String<Size>::end() const
{
    return &_storage[Size];
}

template<std::size_t Size>
constexpr ConstStringView String<Size>::view() const
{
    return { begin(), end() };
}

template<std::size_t Size>
constexpr ConstStringView String<Size>::operator()(std::size_t i, std::size_t j) const
{
    return view()(i, j);
}

template<std::size_t Size>
std::string String<Size>::str() const
{
    return view().str();
}

template<std::size_t Size>
constexpr const char* String<Size>::c_str() const
{
    return view().c_str();
}

namespace detail
{

template<std::size_t LhsSize, std::size_t RhsSize, std::size_t... Is, std::size_t... Js>
constexpr String<LhsSize + RhsSize - 1> cat(const String<LhsSize>& lhs, const String<RhsSize>& rhs,
                                        cpp::meta::index_sequence<Is...>,
                                        cpp::meta::index_sequence<Js...>)
{
    return {{
        lhs[Is]...,
        rhs[Js]...
    }};
}

}

template<std::size_t LhsSize, std::size_t RhsSize>
constexpr String<LhsSize + RhsSize - 1> operator+(const String<LhsSize>& lhs, const String<RhsSize>& rhs)
{
    return detail::cat(
        lhs,
        rhs,
        cpp::meta::make_index_sequence<LhsSize - 1>(), // Remove the middle null terminator
        cpp::meta::make_index_sequence<RhsSize>()
    );
}

template<std::size_t LhsSize, std::size_t RhsSize>
constexpr bool operator==(const String<LhsSize>& lhs, const String<RhsSize>& rhs)
{
    return cpp::constexp::equal(
        lhs,
        rhs
    );
}

template<std::size_t LhsSize, std::size_t RhsSize>
constexpr bool operator!=(const String<LhsSize>& lhs, const String<RhsSize>& rhs)
{
    return !(lhs == rhs);
}

template<std::size_t Size>
std::ostream& operator<<(std::ostream& os, const String<Size>& string)
{
    return os << string.view();
}

template<std::size_t N>
constexpr String<N> stringLiteral(const char (&stringLiteral)[N])
{
    return {stringLiteral};
}

}

}

#endif // SIPLASPLAS_CONSTEXPR_STRING_HPP
