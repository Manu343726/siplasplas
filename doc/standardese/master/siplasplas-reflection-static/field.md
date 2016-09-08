---
layout: standardese-doc
---

# Header file `field.hpp`

``` cpp
namespace cpp
{
    namespace static_reflection
    {
        namespace meta
        {
            template <typename SourceInfo_, typename Class, typename T, T Class::* field>
            class Field<SourceInfo_, T Class::*, field>;
        }
        
        namespace codegen
        {
            template <typename FieldType, FieldType field>
            class Field;
        }
        
        template <typename FieldType, FieldType field>
        class Field;
    }
}
```

## Class template `cpp::static_reflection::Field<FieldType, field>`<a id="cpp::static_reflection::Field<FieldType, field>"></a>

``` cpp
template <typename FieldType, FieldType field>
class Field
: public codegen::Field<FieldType, field>
{};
```

Returns static reflection information of a given pointer to member object

See cpp::static\_reflection::meta::Field for specific informatio about the returned information

### Template parameter `cpp::static_reflection::Field<FieldType, field>::FieldType`<a id="cpp::static_reflection::Field<FieldType, field>.FieldType"></a>

``` cpp
typename FieldType
```

Type of the pointer to member object, i.e. 

`T Class::*`

### Template parameter `cpp::static_reflection::Field<FieldType, field>::field`<a id="cpp::static_reflection::Field<FieldType, field>.field"></a>

``` cpp
FieldType field
```

Pointer to member object

-----
