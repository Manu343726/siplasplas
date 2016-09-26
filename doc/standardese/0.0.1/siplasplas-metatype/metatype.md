---
layout: standardese-doc
---

# Header file `metatype.hpp`

``` cpp
namespace cpp
{
    class MetaTypeSystem
    {
    public:
        template <typename T>
        static void registerMetatype();
        
        static void* create(const std::string& typeName);
        
        template <typename T, std::size_t N>
        static T* create(const char(&) typeName[N]);
        
        template <typename T>
        static void destroy(T* ptr);
        
        static std::string dump();
    };
}
```
