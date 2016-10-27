#ifndef SIPLASPLAS_EXAMPLES_REFLECTION_STATIC_PROTOSERIALIZATION_MAPPING_HPP
#define SIPLASPLAS_EXAMPLES_REFLECTION_STATIC_PROTOSERIALIZATION_MAPPING_HPP

#include <siplasplas/utility/exception.hpp>
#include <siplasplas/utility/meta.hpp>
#include <siplasplas/utility/fusion.hpp>
#include <siplasplas/utility/invoke.hpp>
#include <siplasplas/utility/function_traits.hpp>
#include <siplasplas/constexpr/algorithm.hpp>
#include <siplasplas/constexpr/string.hpp>
#include <siplasplas/reflection/static/api.hpp>
#include <siplasplas/logger/logger.hpp>
#include <siplasplas/utility/staticif.hpp>
#include <chrono>

namespace mapping
{

template<typename From, typename To, typename = void>
class Cast
{
public:
    struct DefaultCast {};

    template<typename From_>
    static constexpr To apply(From_&& from)
    {
        static_assert(std::is_same<From, std::decay_t<From_>>::value,
            "Using wrong From_ type");

        return static_cast<To>(std::forward<From_>(from));
    }

    using convertible = std::is_convertible<From, To>;
};

template<typename To, typename Unit, typename Ratio>
class Cast<std::chrono::duration<Unit, Ratio>, To, std::enable_if_t<
    std::is_integral<To>::value
>>
{
public:
    static constexpr To apply(const std::chrono::duration<Unit, Ratio>& duration)
    {
        return duration.count();
    }
};

template<typename From, typename To>
class Cast<From, To, std::enable_if_t<
    std::is_enum<From>::value &&
    std::is_enum<To>::value
>>
{
public:
    static constexpr To apply(const From& from)
    {
        const char* fromName = cpp::static_reflection::Enum<From>::toString(from);

        if(cpp::static_reflection::Enum<To>::has(fromName))
        {
            return cpp::static_reflection::Enum<To>::fromString(fromName);
        }
        else
        {
            throw cpp::exception<std::domain_error>(
                "No value named '{}' in {} enumeration",
                fromName,
                ctti::type_id<To>().name()
            );
        }
    }
};

template<typename To, typename From>
To cast(From&& from)
{
    return Cast<std::decay_t<From>, To>::apply(std::forward<From>(from));
}

template<typename From, typename To, typename = void>
struct IsConvertible : public cpp::meta::true_ {};

template<typename From, typename To>
struct IsConvertible<From, To, cpp::meta::void_t<
    typename Cast<From, To>::DefaultCast
>> : public Cast<From, To>::convertible {};

static_assert(!IsConvertible<std::string, const char*>::value, "???");
static_assert( IsConvertible<const char*, std::string>::value, "???");

namespace detail
{

template<typename Field>
class ProtoBufBinding : public Field
{
public:
    using FieldType = Field;


    static constexpr auto setPrefix()
    {
        return cpp::constexp::stringLiteral("set_");
    }
    static constexpr auto setAllocatedPrefix()
    {
        return cpp::constexp::stringLiteral("set_allocated_");
    }
    static constexpr auto mutablePrefix()
    {
        return cpp::constexp::stringLiteral("mutable_");
    }
    static constexpr auto hasPrefix()
    {
        return cpp::constexp::stringLiteral("has_");
    }

    static constexpr std::size_t setSize          = setPrefix().size()          + Field::SourceInfo::spelling().size() - 1;
    static constexpr std::size_t setAllocatedSize = setAllocatedPrefix().size() + Field::SourceInfo::spelling().size() - 1;
    static constexpr std::size_t mutableSize      = mutablePrefix().size()      + Field::SourceInfo::spelling().size() - 1;
    static constexpr std::size_t hasSize          = hasPrefix().size()          + Field::SourceInfo::spelling().size() - 1;
    static constexpr cpp::constexp::String<setSize>          set          = setPrefix()          + Field::SourceInfo::spelling();
    static constexpr cpp::constexp::String<setAllocatedSize> setAllocated = setAllocatedPrefix() + Field::SourceInfo::spelling();
    static constexpr cpp::constexp::String<mutableSize>      mutable_     = mutablePrefix()      + Field::SourceInfo::spelling();
    static constexpr cpp::constexp::String<hasSize>          has          = hasPrefix()          + Field::SourceInfo::spelling();
};

template<typename Field>
constexpr cpp::constexp::String<ProtoBufBinding<Field>::setSize> ProtoBufBinding<Field>::set;
template<typename Field>
constexpr cpp::constexp::String<ProtoBufBinding<Field>::setAllocatedSize> ProtoBufBinding<Field>::setAllocated;
template<typename Field>
constexpr cpp::constexp::String<ProtoBufBinding<Field>::mutableSize> ProtoBufBinding<Field>::mutable_;

template<typename Field>
struct SortBySetDistance
{
    template<typename Lhs, typename Rhs>
    struct apply
    {
        using type = cpp::meta::bool_<
            cpp::constexp::levenshteinDistance(
                ProtoBufBinding<Field>::set,
                Lhs::SourceInfo::spelling()
            )
            <
            cpp::constexp::levenshteinDistance(
                ProtoBufBinding<Field>::set,
                Rhs::SourceInfo::spelling()
            )
        >;
    };
};

template<typename Field>
struct SortBySetAllocatedDistance
{
    template<typename Lhs, typename Rhs>
    struct apply
    {
        using type = cpp::meta::bool_<
            cpp::constexp::levenshteinDistance(
                ProtoBufBinding<Field>::setAllocated,
                Lhs::SourceInfo::spelling()
            )
            <
            cpp::constexp::levenshteinDistance(
                ProtoBufBinding<Field>::setAllocated,
                Rhs::SourceInfo::spelling()
            )
        >;
    };
};

template<typename Field>
struct SortByMutableDistance
{
    template<typename Lhs, typename Rhs>
    struct apply
    {
        using type = cpp::meta::bool_<
            cpp::constexp::levenshteinDistance(
                ProtoBufBinding<Field>::mutable_,
                Lhs::SourceInfo::spelling()
            )
            <
            cpp::constexp::levenshteinDistance(
                ProtoBufBinding<Field>::mutable_,
                Rhs::SourceInfo::spelling()
            )
        >;
    };
};

template<typename Field>
struct SortByHasDistance
{
    template<typename Lhs, typename Rhs>
    struct apply
    {
        using type = cpp::meta::bool_<
            cpp::constexp::levenshteinDistance(
                ProtoBufBinding<Field>::has,
                Lhs::SourceInfo::spelling()
            )
            <
            cpp::constexp::levenshteinDistance(
                ProtoBufBinding<Field>::has,
                Rhs::SourceInfo::spelling()
            )
        >;
    };
};


template<typename Field>
struct SortByGetterDistance
{
    template<typename Lhs, typename Rhs>
    struct apply
    {
        using type = cpp::meta::bool_<
            cpp::constexp::levenshteinDistance(
                Field::SourceInfo::spelling(),
                Lhs::SourceInfo::spelling()
            )
            <
            cpp::constexp::levenshteinDistance(
                Field::SourceInfo::spelling(),
                Rhs::SourceInfo::spelling()
            )
        >;
    };
};

namespace detail
{
    template<typename Field, typename Methods>
    struct FilterGetters
    {
        template<typename Method>
        struct Predicate
        {
            using type = cpp::meta::bool_<
                cpp::function_arguments_without_this<
                    typename Method::type
                >::size == 0
            >;
        };

        using type = cpp::meta::filter_t<cpp::meta::defer<Predicate>, Methods>;
    };

    template<typename Field, typename Methods>
    struct FilterSets
    {
        template<typename Method>
        struct Predicate
        {
            using type = cpp::meta::bool_<cpp::function_arguments_without_this<
                typename Method::type
            >::size == 1 && IsConvertible<
                typename Field::value_type,
                cpp::function_argument<1, typename Method::type>
            >::value>;
        };

        using type = cpp::meta::filter_t<cpp::meta::defer<Predicate>, Methods>;
    };
}

template<typename Field, typename Methods>
using FilterGetters = cpp::meta::type_t<detail::FilterGetters<Field, Methods>>;
template<typename Field, typename Methods>
using FilterSets = cpp::meta::type_t<detail::FilterSets<Field, Methods>>;

}

template<typename Protobuf, typename Data>
Protobuf write(const Data& data);
template<typename Data, typename Protobuf>
Data read(const Protobuf& proto);
template<typename Protobuf, typename Data>
void write(const Data& data, Protobuf& proto);
template<typename Data, typename Protobuf>
void read(const Protobuf& proto, Data& data);

class ReadError : public std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
};

template<typename Data, typename Protobuf>
class Mapping
{
private:
    using DataFields   = typename cpp::static_reflection::Class<Data>::Fields;
    using ProtoMethods = typename cpp::static_reflection::Class<Protobuf>::Methods;

public:
    static void write(const Data& data, Protobuf& proto)
    {
        cpp::staticIf<(DataFields::size > 0)>([&](auto identity)
        {
            cpp::foreach_type<DataFields>([&](auto type)
            {
                using Field = cpp::meta::type_t<decltype(type)>;

                // Get the set of methods in the protobuf object,
                // only the ones with one argument with type
                // convertible from the value type of the current
                // data field. Then, sort them by their distance to the
                // field name:
                using SortBySetDistance = cpp::meta::sort_t<
                    ::mapping::detail::FilterSets<Field, ProtoMethods>,
                    ::mapping::detail::SortBySetDistance<Field>
                >;
                using SortBySetAllocatedDistance = cpp::meta::sort_t<
                    ProtoMethods,
                    ::mapping::detail::SortBySetAllocatedDistance<Field>
                >;
                using SortByMutableDistance = cpp::meta::sort_t<
                    ProtoMethods,
                    ::mapping::detail::SortByMutableDistance<Field>
                >;
                using SortByHasDistance = cpp::meta::sort_t<
                    ProtoMethods,
                    ::mapping::detail::SortByHasDistance<Field>
                >;

                constexpr bool hasSet = SortBySetDistance::size > 0;

                // Get the matching method:
                using SetAllocated = cpp::meta::head_t<SortBySetAllocatedDistance>;
                using Mutable = cpp::meta::head_t<SortByMutableDistance>;
                using Has = cpp::meta::head_t<SortByHasDistance>;

                Mapping::log().debug("({} -> {}) on field '{}:",
                    ctti::type_id<Data>().name(),
                    ctti::type_id<Protobuf>().name(),
                    Field::SourceInfo::fullName()
                );
                Mapping::log().debug(" - SetAllocated: {}", SetAllocated::SourceInfo::displayName());
                Mapping::log().debug(" - Mutable: {}", Mutable::SourceInfo::displayName());
                Mapping::log().debug(" - Has: {}", Has::SourceInfo::displayName());

                cpp::staticIf<!hasSet>([&](auto identity)
                {
                    Mapping::log().debug("No set_XXXX() found, using mutable_XXX() ({})",
                        Mutable::SourceInfo::fullName()
                    );
                    // Invoke write recursively with the proto message field:
                    ::mapping::write(
                        identity(cpp::invoke(identity(Field::get()), identity(data))),
                        identity(*cpp::invoke(identity(Mutable::get()), identity(proto)))
                    );
                }).Else([&](auto identity)
                {
                    using Set = cpp::meta::head_t<SortBySetDistance>;
                    using SetParameter = std::decay_t<
                        cpp::function_argument<1, typename Set::type>
                    >;
                    Mapping::log().debug(" - Set: {}", Set::SourceInfo::displayName());

                    // Invoke the setter with the value returned by the field:
                    cpp::invoke(
                        Set::get(),
                        proto,
                        ::mapping::cast<SetParameter>(
                            cpp::invoke(Field::get(), identity(data))
                        )
                    );
                });
            });
        }).Else([&](auto identity)
        {
            proto = ::mapping::cast<Protobuf>(data);
        });
    }

    static Protobuf write(const Data& data)
    {
        Protobuf proto;
        write(data, proto);
        return proto;
    }


    static spdlog::logger& log()
    {
        static std::shared_ptr<spdlog::logger> logger = []
        {
            return cpp::Logger::addLogger(
                fmt::format("protobuf-mapping<{}, {}>",
                    ctti::type_id<Data>().name(),
                    ctti::type_id<Protobuf>().name()
                )
            );
        }();

        return *logger;
    }

    static void read(const Protobuf& proto, Data& data)
    {
        cpp::staticIf<(DataFields::size > 0)>([&](auto identity)
        {
            cpp::foreach_type<DataFields>([&](auto type)
            {
                using Field = cpp::meta::type_t<decltype(type)>;

                // Get the set of methods in the protobuf object,
                // only the ones with no arguments and with a return
                // type convertible to the value type of the current
                // data field. Then, sort them by their distance to the
                // field name:
                using SortByGetterDistance = cpp::meta::sort_t<
                    ::mapping::detail::FilterGetters<Field, ProtoMethods>,
                    ::mapping::detail::SortByGetterDistance<Field>
                >;
                using SortByHasDistance = cpp::meta::sort_t<
                    ProtoMethods,
                    ::mapping::detail::SortByHasDistance<Field>
                >;

                static_assert(SortByGetterDistance::size > 0, "No getter found");

                // Get the matching method:
                using Getter = cpp::meta::head_t<SortByGetterDistance>;
                using Has = cpp::meta::head_t<SortByHasDistance>;

                Mapping::log().debug("({} -> {}) on field '{}'. Getter is '{}'",
                    ctti::type_id<Data>().name(),
                    ctti::type_id<Protobuf>().name(),
                    Field::SourceInfo::fullName(),
                    Getter::SourceInfo::fullName()
                );

                if(cpp::invoke(Has::get(), proto))
                {
                    mapping::read(
                        cpp::invoke(Getter::get(), proto),
                        cpp::invoke(Field::get(), data)
                    );
                }
                else
                {
                    throw cpp::exception<ReadError>(
                        "Protobuf of type {} has no {} ({}) field set",
                        ctti::type_id<Protobuf>().name(),
                        Getter::SourceInfo::spelling(),
                        Field::SourceInfo::spelling()
                    );
                }
            });
        }).Else([&](auto identity)
        {
            identity(data) = mapping::cast<Data>(identity(proto));
        });
    }

    static Data read(const Protobuf& proto)
    {
        Data data;
        read(proto, data);
        return data;
    }
};

template<typename Protobuf, typename Data>
Protobuf write(const Data& data)
{
    return Mapping<Data, Protobuf>::write(data);
}

template<typename Data, typename Protobuf>
Data read(const Protobuf& proto)
{
    return Mapping<Data, Protobuf>::read(proto);
}

template<typename Protobuf, typename Data>
void write(const Data& data, Protobuf& proto)
{
    Mapping<Data, Protobuf>::write(data, proto);
}

template<typename Data, typename Protobuf>
void read(const Protobuf& proto, Data& data)
{
    Mapping<Data, Protobuf>::read(proto, data);
}
} // namespace mapping

#endif // SIPLASPLAS_EXAMPLES_REFLECTION_STATIC_PROTOSERIALIZATION_MAPPING_HPP
