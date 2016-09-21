---
layout: standardese-doc
---

# Header file `class.hpp`

``` cpp
#include "sourceinfo.hpp"

namespace cpp
{
    namespace static_reflection
    {
        namespace meta
        {
            template <typename SourceInfo_, typename Class_, typename Methods_, typename Fields_, typename Constructors_, typename Classes_, typename Enums_>
            class Class
            {
            public:
                using class_type = Class_;
                
                using type = class_type;
                
                using SourceInfo = SourceInfo_;
                
                using Methods = Methods_;
                
                using Fields = Fields_;
                
                using Constructors = Constructors_;
                
                using Classes = Classes_;
                
                using Enums = Enums_;
            };
        }
        
        namespace codegen
        {
            template <typename T>
            class Class
            : public static_reflection::meta::Class<static_reflection::meta::EmptySourceInfo<T>, T, ::cpp::meta::list<>, ::cpp::meta::list<>, ::cpp::meta::list<>, ::cpp::meta::list<>, ::cpp::meta::list<> >
            {};
        }
        
        template <typename T>
        class Class;
    }
}
```

## Class template `cpp::static_reflection::Class<T>`<a id="cpp::static_reflection::Class__T__"></a>

``` cpp
template <typename T>
class Class
: public codegen::Class<T>
{};
```

Returns static reflection information of a given class

This template returns (inherits) a cpp::static\_reflection::meta::Class instance with the information of the given class type. If there's no static reflection information of this class in the current translation unit, returns an empty cpp::static\_reflection::meta::Class instance (Default "unknown" source info, empty methods, ctors, enums, and classes lists, etc). The behavior is undefined if the given type is not a class type.

See cpp::static\_reflection::meta::Class for the returned information.

### Template parameter `cpp::static_reflection::Class<T>::T`<a id="cpp::static_reflection::Class__T__.T"></a>

``` cpp
typename T
```

Must be a class type.

-----
