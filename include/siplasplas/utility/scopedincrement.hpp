#ifndef SIPLASPLAS_UTILITY_SCOPEDINCREMENT_HPP
#define SIPLASPLAS_UTILITY_SCOPEDINCREMENT_HPP

namespace cpp
{

/**
 * \ingroup utility
 * \brief Implements an scoped increment/decrement operation
 *
 * This class provides an exception-safe to increment and decrement
 * the value of a variable when we enter and leave an scope
 *
 * \tparam T Value type. Must be incrementable and decrementable.
 */
template<typename T>
class ScopedIncrement
{
public:
    /**
     * \brief Increments the given lvalue. The value is decremented when
     * the soped increment destructor is called.
     */
    ScopedIncrement(T& ref) :
        _ref(ref)
    {
        ++_ref;
    }

    ~ScopedIncrement()
    {
        --_ref;
    }

private:
    T& _ref;
};

/**
 * \ingroup utility
 * \brief Implements a std::size_t scoped counter
 */
using ScopedSizeCounter = ScopedIncrement<std::size_t>;

/**
 * \ingroup utility
 * \brief Implements a signed integer scoped counter
 */
using ScopedCounter = ScopedIncrement<int>;

}

#endif // SIPLASPLAS_UTILITY_SCOPEDINCREMENT_HPP
