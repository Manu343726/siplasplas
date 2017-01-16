#ifndef SIPLASPLAS_REFLECTION_PARSER_API_CORE_CLANG_HANDLE_HPP
#define SIPLASPLAS_REFLECTION_PARSER_API_CORE_CLANG_HANDLE_HPP

#include <memory>
#include <ctti/type_id.hpp>
#include <siplasplas/utility/exception.hpp>
#include "logger.hpp"

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
 * \ingroup clang
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
 * \ingroup clang
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
 * \ingroup clang
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
 * \brief Represents the operation mode of a UniqueHandle
 *
 * The UniqueHandle RAII behavior can be disabled/enabled on demand
 * depending on whether the user wants an owning handle or a proxy.
 */
enum class HandleMode
{
    RAII, ///< The handle owns the underlying raw handle, releasing in on destruction
    Proxy ///< The handle is just a proxy to an existing handle, and will not release on destuction
};

inline std::ostream& operator<<(std::ostream& os, const HandleMode mode)
{
    static constexpr const char* strings[] = {
        "HandleMode::RAII",
        "HandleMode::Proxy"
    };

    return os << strings[static_cast<std::size_t>(mode)];
}

/**
 * \ingroup clang
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
    constexpr UniqueHandle(RawHandle rawHandle, HandleMode mode = HandleMode::RAII) noexcept :
        _rawHandle{rawHandle},
        _mode{mode}
    {}

    /**
     * \brief Move constructs a handle by taking the ownership of the given handle
     */
    constexpr UniqueHandle(UniqueHandle&& other) noexcept :
        UniqueHandle{other.rawHandle(), other.mode()}
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
        if(mode() == handle.mode())
        {
            (*this) = handle.rawHandle();
            handle._rawHandle = nullHandle<RawHandle>();

            return *this;
        }
        else
        {
            throw cpp::exception<std::runtime_error>(
                "Cannot assign handles of different operating modes (current = {}, other = {})",
                mode(),
                handle.mode()
            );
        }
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
     * \brief Returns the managed raw handle and releases the ownership
     */
    RawHandle release() noexcept
    {
        RawHandle oldHandle = _rawHandle;
        _rawHandle = nullHandle<RawHandle>();
        return oldHandle;
    }

    /**
     * \brief Resets the handle by releasing the owned handle and then becoming null
     */
    constexpr void reset() noexcept
    {
        reset(nullHandle<RawHandle>());
    }

    /**
     * \brief Resets the owned raw handle
     *
     * The old raw handle is released first if the
     * operation mode is set to RAII
     */
    constexpr void reset(RawHandle rawHandle) noexcept
    {
        if(!isNull() && mode() == HandleMode::RAII)
        {
            log().debug("UniqueHandle<{}>::reset(): Handle released",
                ctti::type_id<RawHandle>().name()
            );
            Release(_rawHandle);
        }
        else
        {
            log().debug("UniqueHandle<{}>::reset(): Handle null or non-RAII mode (mode={})",
                ctti::type_id<RawHandle>().name(),
                mode()
            );
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
        reset();
    }

    /**
     * \brief Returns the operating mode (RAII vs Proxy) of the handle
     */
    HandleMode mode() const
    {
        return _mode;
    }

    /**
     * \brief Returns a non-owning proxy to this handle
     */
    UniqueHandle proxy() const
    {
        return {_rawHandle, HandleMode::Proxy};
    }

    /**
     * \brief Returns a proxy handle to the given raw handle
     *
     * Proxy handles are UniqueHandles with the RAII "release on destruction" semantic
     * disabled.
     */
    static UniqueHandle proxy(const RawHandle& rawHandle)
    {
        return {rawHandle, HandleMode::Proxy};
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
    HandleMode _mode;
};


/**
 * \ingroup clang
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
     * \brief Takes ownership of the given unique handle
     *
     * The shared handle is initialized by owning a raw handle currently
     * owned by a given unique handle. After the initialization the unique
     * handle becomes null.
     *
     * \param handle Unique handle to take the raw handle from
     */
    SharedHandle(UniqueHandle& handle) :
        _handlePtr{std::make_shared<UniqueHandle>(handle.release())}
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
     * \brief Releases the owned handle. The owned handle becomes null after
     * the release operation. All sibling shared handles become null after this operation.
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

    /**
     * \brief Returns an unique handle to the owned underlying raw handle
     *
     * \returns A unique handle owning the raw handle. After this operation
     * this shared handle and its sibling become null.
     */
    UniqueHandle unique()
    {
        return std::move(*_handlePtr);
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
