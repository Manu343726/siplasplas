#ifndef SIPLASPLAS_UTILITY_PRINTERS_HPP
#define SIPLASPLAS_UTILITY_PRINTERS_HPP

#include <ostream>
#include <functional>
#include <siplasplas/utility/export.hpp>

template<typename F>
auto operator<<(std::ostream& os, F f) -> decltype(f(os))
{
    return f(os);
}

namespace cpp
{
    SIPLASPLAS_UTILITY_EXPORT
    std::function<std::ostream&(std::ostream&)> print_memory(const char* begin, const char* end, std::size_t word = 0, char byteSeparator = 0);
}

#endif // SIPLASPLAS_UTILITY_PRINTERS_HPP
