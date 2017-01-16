#ifndef SIPLASPLAS_REFLECTION_PARSER_CORE_CLANG_HANDLEENTITY_HPP
#define SIPLASPLAS_REFLECTION_PARSER_CORE_CLANG_HANDLEENTITY_HPP

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

/**
 * \brief Represents API entities that just enhance a
 * libclang unique handle with no extra underlying data
 *
 * \tparam Handle UniqueHandle type to inherit from
 */
template<typename Handle>
class UniqueHandleEntity : public Handle
{
public:
    using Handle::Handle;

    UniqueHandleEntity(Handle&& handle) :
        Handle{std::move(handle)}
    {}
};

}

}

}

}

}

}

#endif // SIPLASPLAS_REFLECTION_PARSER_CORE_CLANG_HANDLEENTITY_HPP
