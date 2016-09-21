---
layout: standardese-doc
---

# Header file `enum.hpp`

``` cpp
namespace cpp
{
    namespace static_reflection
    {
        namespace meta
        {
            template <typename SourceInfo_, typename EnumType, EnumType ... Constants, typename ... ConstantsNames>
            class Enum<SourceInfo_, EnumType,::cpp::meta::list<std::integral_constant<EnumType, Constants>...>,::cpp::meta::list<ConstantsNames...>>
            {
            public:
                using type = EnumType;
                
                using Type = EnumType;
                
                using UnderlyingType = typename std::underlying_type<EnumType>::type;
                
                using SourceInfo = SourceInfo_;
                
                using names_array_t = std::array<const char*, sizeof...(ConstantsNames)>;
                
                using values_array_t = const EnumType[sizeof...(Constants)];
                
                constexpr Enum() = default;
                
                static constexpr std::size_t count();
                
                static constexpr const names_array_t& names();
                
                static constexpr const values_array_t& values();
                
                static constexpr EnumType value(std::size_t i);
                
                static constexpr const char* name(std::size_t i);
                
                static constexpr const char* toString(const EnumType value);
                
                static constexpr EnumType fromString(const char* name);
                
                static constexpr bool has(const char* name);
                
                static constexpr bool has(const UnderlyingType value);
            };
        }
        
        namespace codegen
        {
            template <typename EnumType>
            class Enum{};
        }
        
        template <typename EnumType>
        class Enum;
    }
}
```

## Class template `cpp::static_reflection::Enum<EnumType>`<a id="cpp::static_reflection::Enum__EnumType__"></a>

``` cpp
template <typename EnumType>
class Enum
: public codegen::Enum<EnumType>
{};
```

Returns static reflection information of the given enumeration type.

See cpp::static\_reflection::meta::Enum template for the returned information.

### Template parameter `cpp::static_reflection::Enum<EnumType>::EnumType`<a id="cpp::static_reflection::Enum__EnumType__.EnumType"></a>

``` cpp
typename EnumType
```

Must be an enumeration type. The behavior is undefined otherwise.

-----
