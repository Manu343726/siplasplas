#ifndef SIPLASPLAS_TYPEERASURE_FEATURES_IOSTREAM_HPP
#define SIPLASPLAS_TYPEERASURE_FEATURES_IOSTREAM_HPP

#include <siplasplas/utility/staticif.hpp>
#include <siplasplas/utility/exception.hpp>
#include <siplasplas/typeerasure/concepts/iostream.hpp>

namespace cpp
{

namespace features
{

/**
 * \ingroup features
 * \brief Implements write to an standard output stream
 */
class OStreamable
{
public:
    /**
     * \brief Implements write to output stream feature.
     *
     * OStreamable feature implements write to an output stream
     * in a type agnostic way. If the type is not writable to an
     * standard output stream (See cpp::concepts::OStreamable), it throws
     * a std::runtime_error exception. Else invokes `operator<<(std::ostream&, const T& value)`.
     *
     * \param os Reference to the output stream to write to.
     * \param value Value to write to the output stream.
     *
     * \return Reference to the passed output stream.
     */
    template<typename T>
    static std::ostream& apply(std::ostream& os, const T& value) noexcept(cpp::concepts::OStreamable<T>::no_except)
    {
        return cpp::staticIf<cpp::concepts::OStreamable<T>::value>([&](auto identity) -> std::ostream&
        {
            return identity(os) << value;
        }).Else([](auto) -> std::ostream&
        {
            throw cpp::exception<std::runtime_error>(
                "{} is not ostreamable!",
                ctti::type_id<T>().name()
            );
        });
    }
};

/**
 * \ingroup features
 * \brief Implements read from an standard input stream
 */
class IStreamable
{
public:
    /**
     * \brief Implements read from input stream feature.
     *
     * IStreamable feature implements read from an input stream
     * in a type agnostic way. If the type is not readable from an
     * standard input stream (See cpp::concepts::IStreamable), it throws
     * a std::runtime_error exception. Else invokes `operator<<(std::istream&, T&)`.
     *
     * \param os Reference to the input stream to read from.
     * \param value lvalue reference to the object which the read value will be assigned to.
     *
     * \return Reference to the passed input stream.
     */
    template<typename T>
    static std::istream& apply(std::istream& is, T& value) noexcept(cpp::concepts::IStreamable<T>::no_except)
    {
        return cpp::staticIf<cpp::concepts::IStreamable<T>::value>([&](auto identity) -> std::istream&
        {
            return identity(is) >> value;
        }).Else([](auto) -> std::istream&
        {
            throw cpp::exception<std::runtime_error>(
                "{} is not istreamable!",
                ctti::type_id<T>().name()
            );
        });
    }
};
}

}

#endif // SIPLASPLAS_TYPEERASURE_FEATURES_IOSTREAM_HPP
