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
            class Enum<SourceInfo_, EnumType,::cpp::meta::list<std::integral_constant<EnumType, Constants>...>,::cpp::meta::list<ConstantsNames...>>;
        }
        
        namespace codegen
        {
            template <typename EnumType>
            class Enum;
        }
        
        template <typename EnumType>
        class Enum;
    }
}
```

## Class template `cpp::static_reflection::Enum<EnumType>`<a id="cpp::static_reflection::Enum<EnumType>"></a>

``` cpp
template <typename EnumType>
class Enum
: public codegen::Enum<EnumType>
{};
```

Returns static reflection information of the given enumeration type.

See cpp::static\_reflection::meta::Enum template for the returned information.

### Template parameter `cpp::static_reflection::Enum<EnumType>::EnumType`<a id="cpp::static_reflection::Enum<EnumType>.EnumType"></a>

``` cpp
typename EnumType
```

Must be an enumeration type. The behavior is undefined otherwise.

-----
