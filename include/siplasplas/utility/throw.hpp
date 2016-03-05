#ifndef SIPLASPLAS_UTILITY_THROW_HPP
#define SIPLASPLAS_UTILITY_THROW_HPP

#include <sstream>

namespace cpp
{
    namespace detail
    {
        template<typename Exception>
        class ExceptionBuilder
        {
        public:
            ExceptionBuilder() = default;
            ExceptionBuilder(const ExceptionBuilder&) = delete;
            ExceptionBuilder(ExceptionBuilder&&) = default;

            ~ExceptionBuilder()
            {
                throw Exception{os.str()}; // this has UB :(
            }

            template<typename T>
            friend ExceptionBuilder operator<<(ExceptionBuilder&& eb, const T& value)
            {
                eb.os << value;
                return std::move(eb);
            }

            friend ExceptionBuilder operator<<(ExceptionBuilder&& eb, std::ostream&(*manip)(std::ostream&))
            {
                manip(eb.os);
                return std::move(eb);
            }

        private:
            std::ostringstream os;
        };
    }

    template<typename Exception>
    detail::ExceptionBuilder<Exception> Throw()
    {
        return {};
    }
}

#endif // SIPLASPLAS_UTILITY_THROW_HPP
