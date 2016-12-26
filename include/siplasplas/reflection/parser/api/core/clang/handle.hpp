#ifndef SIPLASPLAS_REFLECTION_PARSER_API_CORE_CLANG_HANDLE_HPP
#define SIPLASPLAS_REFLECTION_PARSER_API_CORE_CLANG_HANDLE_HPP

#include <memory>

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
 * \brief Returns the null value for a given handle type RawHandle
 *
 * This trait can be specialized to specify the null value of
 * a clang handle type (Like CXCursor). An specifalization for pointer
 * types with nullptr as null value is provided by default
 *
 * The null value is returned as a static constexpr public member variable
 * named 'value':
 *
 * ``` cpp
 * CXCursor nullCursor = NullHandle<CXCursor>::value;
 * ```
 *
 * \tparam RawHandle Handle type
 */
template<typename RawHandle>
class NullHandle
{
public:
    static_assert(
        sizeof(RawHandle) != sizeof(RawHandle),
        "No null handle value known, specialize cpp::reflection::parser::api::core::clang::NullHandle<RawHandle>"
    );
};

/**
 * \brief NullHandle specialization for pointer types
 *
 * Returns nullptr as null value.
 */
template<typename RawHandle>
class NullHandle<RawHandle*>
{
public:
    static constexpr RawHandle* value = nullptr;
};

/**
 * \brief Returns the null value for a given handle type RawHandle
 *
 * \tparam RawHandle Handle type
 */
template<typename RawHandle>
constexpr RawHandle nullHandle()
{
    return NullHandle<RawHandle>::value;
}

/**
 * \brief Provides a iunique-ownership RAII wrapper for raw resources
 *
 * This class owns a raw clang resource instanced by a factory
 * function and that must be released through a Release function.
 * This class provides unique ownership of the raw resource.⇧
 *
 * \tparam RawHandle Raw handle type
 * \tparam void(*Release)(RawHandle) Release function
 */
template<typename RawHandle, void(*Release)(RawHandle)>
class UniqueHandle
{
public:
    /**
     * \brief Takes ownership of a given raw handle
     *
     * \param handle Raw handle to a clang resource
     */
    constexpr UniqueHandle(RawHandle rawHandle) noexcept :
        _rawHandle{rawHandle}
    {}

    /**
     * \brief Move constructs a handle by taking the ownership of the given handle
     */
    constexpr UniqueHandle(UniqueHandle&& other) noexcept :
        UniqueHandle{other.rawHandle()}
    {
        other._rawHandle = nullHandle<RawHandle>();
    }

    /**
     * \brief Move assigns a handle by taking ownership
     * of the given handle
     *
     * The previously owned raw handle is released first, \param handle becomes null.
     */
    constexpr UniqueHandle& operator=(UniqueHandle&& handle) noexcept
    {
        (*this) = handle.rawHandle();
        handle._rawHandle = nullHandle<RawHandle>();

        return *this;
    }

    /**
     * \brief Takes ownership of the given raw handle
     *
     * The previously owned raw handle is released first
     */
    constexpr UniqueHandle& operator=(RawHandle rawHandle) noexcept
    {
        reset(rawHandle);
        return *this;
    }

    /**
     * \brief Releases the owned handle. The owned handle becomes null after
     * the release operation.
     */
    constexpr void release() noexcept
    {
        reset(nullHandle<RawHandle>());
    }

    /**
     * \brief Resets the owned raw handle
     *
     * The old raw handle is released first
     */
    constexpr void reset(RawHandle rawHandle) noexcept
    {
        if(!isNull())
        {
            Release(_rawHandle);
        }

        _rawHandle = rawHandle;
    }

    /**
     * \brief Returns a null UniqueHandle instance
     *
     * \returns Returns a UniqueHandle instance initialized
     * with the null raw handle value of the raw handle type RawHandle
     */
    static constexpr UniqueHandle null() noexcept
    {
        return {nullHandle<RawHandle>()};
    }

    /**
     * \brief Checks wether the handle is null
     *
     * \returns True if the handle is null, false otherwise
     */
    constexpr bool isNull() const noexcept
    {
        return rawHandle() == nullHandle<RawHandle>();
    }

    /**
     * \brief Returns the raw owned resource
     */
    constexpr RawHandle rawHandle() const noexcept
    {
        return _rawHandle;
    }

    /**
     * \brief Releases the owned handle. Invokes release()
     */
    ~UniqueHandle() noexcept
    {
        release();
    }

    friend constexpr bool operator==(const UniqueHandle& lhs, const UniqueHandle& rhs)
    {
        return lhs.rawHandle() == rhs.rawHandle();
    }

    friend constexpr bool operator!=(const UniqueHandle& lhs, const UniqueHandle& rhs)
    {
        return !(lhs == rhs);
    }

private:
    RawHandle _rawHandle;
};


/**
 * \brief Provides a shared-ownership RAII wrapper for raw resources
 *
 * This class owns a raw clang resource instanced by a factory
 * function and that must be released through a Release function.
 * This class provides shared ownership of the raw resource, that is,
 * multiple SharedResource instances may be handling the same resource, as
 * a result of copying the shared handles. The raw handle is released by the
 * last SharedHandle instance which dtor is invoked.
 *
 * \tparam RawHandle Raw handle type
 * \tparam void(*Release)(RawHandle) Release function
 */
template<typename RawHandle, void(*Release)(RawHandle)>
class SharedHandle
{
public:
    /**
     * \brief core::clang::UniqueHandle equivalent type
     */
    using UniqueHandle = core::clang::UniqueHandle<RawHandle, Release>;

    SharedHandle() = delete;

    /**
     * \brief Takes ownership of a given raw handle
     */
    SharedHandle(RawHandle handle) :
        _handlePtr{std::make_shared<UniqueHandle>(handle)}
    {}

    /**
     * \brief Takes ownership of the given raw handle
     *
     * This operation assigns a new raw handle to the shared handle. This is
     * applied to all the SharedHandle instances currently sharing the handle,
     * there's no "forking" of the high-level handles. After the siignment operation,
     * all the SharedHandle instances related to this one own the new handle.
     */
    SharedHandle& operator=(RawHandle handle) noexcept
    {
        *_handlePtr = handle;
        return *this;
    }

    /**
     * \brief Resets the underlying new handle
     *
     * Takes ownership of a new raw handle and releases the old one first. After this operation,
     * all SharedHandle instances related to this one own the new raw handle.
     */
    void reset(RawHandle handle) noexcept
    {
        _handlePtr->reset(handle);
    }

    /**
     * \brief Releases the underlying new handle
     *
     * Takes ownership of a new raw handle and releases the old one first. After this operation,
     * all SharedHandle instances related to this one own the new raw handle. The raw handle becomes
     * null after the operation
     */
    void release() noexcept
    {
        _handlePtr->release();
    }

    /**
     * \brief Returns the underlying raw handle
     */
    RawHandle rawHandle() const noexcept
    {
        return _handlePtr->rawHandle();
    }

    /**
     * \brief Checks wether the owned handle is null
     *
     * \returns True if the handle is null, false otherwise
     */
    bool isNull() const noexcept
    {
        return _handlePtr->isNull();
    }

private:
    std::shared_ptr<UniqueHandle> _handlePtr;
};

}

}

}

}

}

}

#endif // SIPLASPLAS_REFLECTION_PARSER_API_CORE_CLANG_HANDLE_HPP
