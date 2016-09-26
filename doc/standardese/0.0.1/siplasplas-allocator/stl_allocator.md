---
layout: standardese-doc
---

# Header file `stl_allocator.hpp`

``` cpp
namespace cpp
{
    template <typename T, typename Allocator>
    struct STLAllocator
    : Allocator
    {
        using value_type = T;
        
        using pointer = T*;
        
        using const_pointer = const T*;
        
        using reference = T&;
        
        using const_reference = const T&;
        
        using propagate_on_container_move_assignment = std::true_type;
        
        STLAllocator() = default;
        
        STLAllocator(const STLAllocator&) = default;
        
        STLAllocator(STLAllocator&&) = default;
        
        STLAllocator(char* begin, char* end, std::size_t fence = 0, unsigned char cannary = 0xff);
        
        STLAllocator(Allocator&& allocator, std::size_t fence = 0, unsigned char cannary = 0xff);
        
        STLAllocator(const Allocator& allocator, std::size_t fence = 0, unsigned char cannary = 0xff);
        
        template <typename U>
        STLAllocator(const STLAllocator<U, Allocator>& alloc);
        
        template <typename U>
        STLAllocator(STLAllocator<U, Allocator>&& alloc);
        
        const Allocator& raw_allocator() const;
        
        Allocator& raw_allocator();
        
        pointer allocate(std::size_t count);
        
        void deallocate(pointer ptr, std::size_t count);
        
        template <typename ... Args>
        void construct(pointer ptr, Args&&... args);
        
        void destroy(pointer ptr);
        
        template <typename U>
        struct rebind
        {
            using other = STLAllocator<U, Allocator>;
        };
        
        std::string dump() const;
    };
    
    template <typename T, typename Allocator>
    cpp::STLAllocator<T, std::decay_t<Allocator>> make_stl_allocator();
}
```
