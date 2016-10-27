#ifndef SIPLASPLAS_CONSTEXPR_STRINGVIEW_HPP
#define SIPLASPLAS_CONSTEXPR_STRINGVIEW_HPP

#include "arrayview.hpp"
#include <string>

namespace cpp
{

namespace constexp
{

/**
 * \brief Implements a constexpr reference to an slice of
 * an string
 */
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

    constexpr const char* c_str() const
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

    constexpr const char* c_str() const
    {
        return begin();
    }
};

}

}

#endif // SIPLASPLAS_CONSTEXPR_STRINGVIEW_HPP
