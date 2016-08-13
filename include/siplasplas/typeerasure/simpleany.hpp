#ifndef SIPLASPLAS_TYPEERASURE_SIPLEANY_HPP
#define SIPLASPLAS_TYPEERASURE_SIPLEANY_HPP

#include "features/valuesemantics.hpp"
#include "features/featuresgroup.hpp"
#include <siplasplas/utility/destroy.hpp>
#include <siplasplas/utility/memory_manip.hpp>

namespace cpp
{

namespace detail
{

enum class ValueSemanticsOperation : std::size_t
{
    COPY_CONSTRUCT = 0,
    MOVE_CONSTRUCT = 1,
    COPY_ASSIGN    = 2,
    MOVE_ASSIGN    = 3,
    DESTROY        = 4
};

using ValueSemanticsOperationFunction = void(*)(void*, void*);

template<typename T>
ValueSemanticsOperationFunction valueSemanticsOperation(ValueSemanticsOperation operation)
{
    static constexpr SemanticFunction operations[] = {
        +[](void* object, void* other) {
            features::CopyConstructible::apply<T>(object, other);
        },
        +[](void* object, void* other) {
            features::MoveConstructible::apply<T>(object, other);
        },
        +[](void* object, void* other) {
            features::CopyAssignable::apply<T>(object, other);
        },
        +[](void* object, void* other) {
            features::MoveAssignable::apply<T>(object, other);
        },
        +[](void* object, void*) {
            features::Destructible::apply<T>(object);
        }
    };

    return operations[static_cast<std::size_t>(operation)];
};

using ValueSemantics = ValueSemanticsOperationFunction(*)(ValueSemanticsOperation);

}

class SimpleAny
{
public:
    static constexpr std::size_t STORAGE_CAPACITY = 32;

    template<typename T, typename... Args>
    SimpleAny create(Args&&... args)
    {
        return SimpleAny{meta::identity<T>(), std::forward<Args>(args)...};
    }

    template<typename T>
    SimpleAny(const T& value) :
        _alignment_offset{computeOffset<T>()}
    {
        SIPLASPLAS_ASSERT_TRUE(objectFitsInStorage<T>());
        semantics() = detail::valueSemanticsOperation<T>;
        semantics()(detail::ValueSemanticsOperation::COPY_CONSTRUCT)(storage(), &value);
    }

    SimpleAny(const SimpleAny& other) :
        _alignment_offset{other._alignment_offset}
    {
        semantics() = other.semantics();
        semantics()(detail::ValueSemanticsOperation::COPY_CONSTRUCT)(storage(), other.storage());
    }

    SimpleAny(SimpleAny& other) :
        _alignment_offset{other._alignment_offset}
    {
        semantics() = other.semantics();
        semantics()(detail::ValueSemanticsOperation::MOVE_CONSTRUCT)(storage(), other.storage());
    }

    template<typename T>
    SimpleAny& operator=(const T& value)
    {
        if(detail::valueSemanticsOperation<T> != semantics())
        {
            semantics()(detail::ValueSemanticsOperation::DESTROY)(storage(), nullptr);

            _alignment_offset = computeOffset<T>();
            SIPLASPLAS_ASSERT_TRUE(objectFitsInStorage<T>());
            semantics() = detail::valueSemanticsOperation<T>;
            semantics()(detail::ValueSemanticsOperation::COPY_CONSTRUCT(storage(), &value);
        }
        else
        {
            semantics()(detail::ValueSemanticsOperation::COPY_ASSIGN)(storage(), &value);
        }

        return *this;
    }

    SimpleAny& operator=(const SimpleAny& other)
    {
        if(semantics() != other.semantics())
        {
            semantics()(detail::ValueSemanticsOperation::DESTROY)(storage());

            _alignment_offset = other._alignment_offset;
            SIPLASPLAS_ASSERT_TRUE(objectFitsInStorage<T>());
            semantics() = other.semantics();
            semantics()(detail::ValueSemanticsOperation::COPY_CONSTRUCT)(storage(), other.storage());
        }
        else
        {
            semantics()(detail::ValueSemanticsOperation::COPY_ASSIGN)(storage(), other.storage());
        }

        return *this;
    }

    SimpleAny& operator=(SimpleAny&& other)
    {
        if(semantics() != other.semantics())
        {
            semantics()(detail::ValueSemanticsOperation::DESTROY)(storage());

            _alignment_offset = other._alignment_offset;
            SIPLASPLAS_ASSERT_TRUE(objectFitsInStorage<T>());
            semantics() = other.semantics();
            semantics()(detail::ValueSemanticsOperation::MOVE_CONSTRUCT)(storage(), other.storage());
        }
        else
        {
            semantics()(detail::ValueSemanticsOperation::MOVE_ASSIGN)(storage(), other.storage());
        }

        return *this;
    }

    ~SimpleAny()
    {
        semantics()(detail::ValueSemanticsOperation::DESTROY)(storage(), nullptr);
    }


    void* storage()
    {
        return &_storage[_alignment_offset];
    }

private:
    std::uint8_t _alignment_offset;
    std::array<std::uint8_t, STORAGE_CAPACITY> _storage;

    template<typename T, typename... Args>
    SimpleAny(meta::identity<T>, Args&&... args) :
        _alignment_offset{computeOffset<T>()}
    {
        SIPLASPLAS_ASSERT_TRUE(objectFitsInStorage<T>());
        semantics() = detail::valueSemanticsOperation<T>;
        features::Constructible::apply<T>(storage(), std::forward<Args>(args)...);
    }

    static constexpr std::size_t semanticsSizeOf()
    {
        return sizeof(detail::ValueSemantics);
    }

    static constexpr std::size_t semanticsAlignment()
    {
        return alignof(detail::ValueSemantics);
    }

    template<typename T>
    static std::uint8_t computeOffset(char* storage)
    {
        const char* alignedSemanticsStorage = cpp::aligned_ptr(storage, semanticsAlignment());
        const char* alignedStorage          = cpp::aligned_ptr(alignedSemanticsStorage + semanticsSizeOf(), alignof(T));
        const std::uintptr_t semanticsOffset = alignedSemanticsStorage - storage;
        const std::uintptr_t storageOffset   = alignedStorage - semanticsSizeOf() - alignedSemanticsStorage;

        SIPLASPLAS_ASSERT_LE(semanticsOffset, 16);
        SIPLASPLAS_ASSERT_LE(storageOffset, 16);

        return static_cast<std::uint8_t>(semanticsOffset << 4 | storageOffset);
    }

    std::uint8_t semanticsOffset() const
    {
        return _alignment_offset >> 4;
    }

    std::size_t storageOffset() const
    {
        return 0xF0 & _alignment_offset;
    }

    detail::ValueSemantics& semantics()
    {
        return *reinterpret_cast<detail::ValueSemantics*>(&_storage[semanticsOffset()]);
    }

    void* storage()
    {
        return reinterpret_cast<char*>(semantics()) + semanticsSizeOf() + storageOffset();
    }

    template<typename T>
    bool objectFitsInStorage() const
    {
        return reinterpret_cast<char*>(storage()) + sizeof(T) < &_storage[STORAGE_CAPACITY];
    }
};

}

#endif // SIPLASPLAS_TYPEERASURE_SIPLEANY_HPP
