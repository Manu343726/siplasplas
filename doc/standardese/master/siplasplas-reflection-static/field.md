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
            class Field<SourceInfo_, T Class::*, field>
            {
            public:
                using type = T Class::*;
                
                using value_type = T;
                
                using class_type = Class;
                
                using decay_t = cpp::meta::decay_t<value_type>;
                
                using SourceInfo = SourceInfo_;
                
                constexpr Field() = default;
                
                static constexpr type get();
                
                static constexpr const decay_t& get(const Class& object);
                
                static constexpr decay_t& get(Class& object);
            };
        }
        
        namespace codegen
        {
            template <typename FieldType, FieldType field>
            class Field{};
        }
        
        template <typename FieldType, FieldType field>
        class Field;
    }
}
```

## Class template `cpp::static_reflection::Field<FieldType, field>`<a id="cpp::static_reflection::Field__FieldType, field__"></a>

``` cpp
template <typename FieldType, FieldType field>
class Field
: public codegen::Field<FieldType, field>
{};
```

Returns static reflection information of a given pointer to member object

See cpp::static\_reflection::meta::Field for specific informatio about the returned information

### Template parameter `cpp::static_reflection::Field<FieldType, field>::FieldType`<a id="cpp::static_reflection::Field__FieldType, field__.FieldType"></a>

``` cpp
typename FieldType
```

Type of the pointer to member object, i.e. 

`T Class::*`

### Template parameter `cpp::static_reflection::Field<FieldType, field>::field`<a id="cpp::static_reflection::Field__FieldType, field__.field"></a>

``` cpp
FieldType field
```

Pointer to member object

-----
