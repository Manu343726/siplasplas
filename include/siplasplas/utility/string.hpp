#ifndef SIPLASPLAS_UTILITY_STRING_HPP
#define SIPLASPLAS_UTILITY_STRING_HPP

#include <string>
#include <vector>
#include <siplasplas/utility/export.hpp>

namespace cpp
{

std::vector<std::string> SIPLASPLAS_UTILITY_EXPORT split(const std::string& string, const std::string& delimiter = "");

}

#endif // SIPLASPLAS_UTILITY_STRING_HPP
