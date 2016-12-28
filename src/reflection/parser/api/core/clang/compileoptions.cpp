#include "compileoptions.hpp"
#include <siplasplas/utility/hash.hpp>
#include <siplasplas/utility/assert.hpp>

using namespace ::cpp::reflection::parser::api::core::clang;
using namespace ::cpp::constexp;

using Options = std::unordered_set<CompileOptions::OptionPositionPair, CompileOptions::OptionPositionPairHash>;

std::size_t CompileOptions::OptionPositionPairHash::operator()(const CompileOptions::OptionPositionPair& pair) const
{
    return cpp::hash(pair.first);
}

namespace
{
    Options initializeOptions(const ConstStringViews& compileOptions)
    {
        Options options;
        options.reserve(compileOptions.size());

        for(std::size_t i = 0; i < compileOptions.size(); ++i)
        {
            options.emplace(std::make_pair(compileOptions[i], i));
        }

        return options;
    }

    std::vector<const char*> initializeRawOptions(const Options& options)
    {
        std::vector<const char*> rawOptions;
        rawOptions.resize(options.size(), nullptr);

        for(const auto& option : options)
        {
            rawOptions[option.second] = option.first.c_str();
        }

        return rawOptions;
    }
}

CompileOptions::CompileOptions(const ConstStringViews& compileOptions) :
    _options{initializeOptions(compileOptions)},
    _rawOptions{initializeRawOptions(_options)}
{}

CompileOptions& CompileOptions::option(std::string option)
{
    const auto pair = std::make_pair(std::move(option), _options.size());
    auto it = _options.find(pair);

    if(it == _options.end())
    {
        const auto result = _options.emplace(std::move(pair));

        if(result.second)
        {
            _rawOptions.emplace_back(
                result.first->first.c_str()
            );

            SIPLASPLAS_ASSERT_EQ(_rawOptions.size(), _options.size());
            SIPLASPLAS_ASSERT_EQ(_rawOptions[result.first->second], result.first->first.c_str());
        }
    }

    return *this;
}

CompileOptions& CompileOptions::operator()(std::string option)
{
    return this->option(std::move(option));
}

CompileOptions& CompileOptions::includeDir(const ConstStringView& includeDir)
{
    return option(fmt::format("-I{}", includeDir));
}

CompileOptions& CompileOptions::I(const ConstStringView& includeDir)
{
    return this->includeDir(includeDir);
}

CompileOptions& CompileOptions::std(const ConstStringView& standard)
{
    return option(fmt::format("-std={}", standard));
}

const char* const* CompileOptions::options() const
{
    return _rawOptions.data();
}

std::size_t CompileOptions::count() const
{
    return _rawOptions.size();
}

namespace cpp
{

namespace reflection
{

namespace parser
{

namespace api
{

namespace core
{

namespace clang
{

std::ostream& operator<<(std::ostream& os, const CompileOptions& compileOptions)
{
    return os << ConstStringViews(compileOptions.options(), compileOptions.count());
}

}

}

}

}

}

}
