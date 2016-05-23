#include "string.hpp"
#include <sstream>
#include <algorithm>
#include <iterator>

using namespace cpp;

std::vector<std::string> split(const std::string& string, const std::string& delimiter)
{
    std::istringstream is{string};
    std::vector<std::string> result;

    std::copy(
        std::istream_iterator<std::string>(is),
        std::istream_iterator<std::string>(),
        std::back_inserter(result)
    );

    return result;
}
