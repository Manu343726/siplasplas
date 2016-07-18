#ifndef SIPLASPLAS_UTILITY_DYNAMICLIBRARY_HPP
#define SIPLASPLAS_UTILITY_DYNAMICLIBRARY_HPP

#include <string>
#include <unordered_map>
#include <siplasplas/utility/export.hpp>
#include <memory>

namespace cpp
{

/**
 * \ingroup utility
 * \defgroup dynamic-library
 * \brief Cross platform API to load symbols from
 * shared librarties at runtime.
 */

/**
 * \ingroup dynamic-library
 * \brief Provides a cross-platform interface to load symbols from shared libraries at runtime.
 *
 * DynamicLibrary is a RAII abstraction over a OS-specific shared library handle. The library
 * handle is refcounted and automatically released when there are no more DynamicLibrary instances
 * referencing it. Symbols can be loaded from the library using getSymbol() function.
 */
class SIPLASPLAS_UTILITY_EXPORT DynamicLibrary
{
public:
    /**
     * \brief Loads a shared library.
     *
     * This function loads a shared library from the given path. It follows the path conventions of the underlying
     * OS API, no special path handling is done from siplasplas side. On linux, after loading the library the dlinfo()
     * API is invoked to get the right path to the library.
     * DynamicLibrary class can only be instanced through this factory function.
     *
     * \param libraryPath Path to the shared library. Can be relative to the executable working directory or
     * absolute. The given path could not be the same registred into the DynamicLibrary instance (See details).
     *
     * \return A DynamicLibrary object representing the loaded library. If the library could not be loaded the
     * function throws a `std::runtime_error` exception.
     *
     * ``` cpp
     * auto myLib = DynamicLibrary::load("myLib.dll");
     * ```
     */
    static DynamicLibrary load(const std::string& libraryPath);

    /**
     * \brief Represents a symbol loaded from a dynamic library.
     *
     * This class provides access to a loaded symbol from an external library.
     * The class wraps the raw pointer to the loaded symbol returned from the OS-specific API.
     * You can access the loaded symbol address directly or use the convenient get() and getObject() functions
     * that already apply type conversions to the symbol.
     * Symbols are associated with a DynamicLibrary instanced. When its parent dynamic library goes out of
     * scope symbols are invalidated and accessing them has undefined behavior.
     *
     * ``` cpp
     * // Load "external_function" symbol from our shared library:
     * auto externalFunction = DynamicLibrary::Symbol::load(myLib, "external_function");
     *
     * // Read the symbol as a function pointer:
     * auto fptr = externalFunction.get<void(int)>();
     *
     * // Invoke the external library function:
     * fptr(42);
     * ```
     */
    class SIPLASPLAS_UTILITY_EXPORT Symbol
    {
        friend class DynamicLibrary;
    public:
        Symbol() = default;

        /**
         * \brief Returns the name of the symbol.
         *
         * \return std::string containing the name of the symbol.
         */
        const std::string& name() const;

        /**
         * \brief Returns the raw address of the loaded symbol.
         *
         * \return Pointer to the symbol.
         */
              void* handle();

        /**
         * \brief Returns the raw address of the loaded symbol.
         *
         * \return Const pointer to the symbol.
         */
        const void* handle() const;

        /**
         * \brief Returns the symbol converted to the required type.
         *
         * \tparam T Type to convert the symbol to. Must be a pointer type.
         *
         * \return Symbol value converted to \p T.
         */
        template<typename T>
        T get()
        {
            return reinterpret_cast<T>(handle());
        }

        /**
         * \brief Returns the symbol interpreted as a reference to an object.
         *
         * This function is useful when loading variables from the shared library.
         * Loading objects with automatic or dynamic storage duration has undefined
         * behavior.
         *
         * \tparam T Value type to convert the symbol to.
         *
         * \return Lvalue reference of type \p T to the symbol.
         *
         * ``` cpp
         * // Load a global variable symbol from a DynamicLibrary:
         * auto myLibGlobalVariable = myLibrary.getSymbol("my_lib_global_variable");
         *
         * // Interpret as an int and read its value:
         * std::cout << "myLib::my_lib_global_variable: " << myLibGlobalVariable.getObject<int>();
         * ```
         */
        template<typename T>
        T& getObject()
        {
            return *get<T*>();
        }

        /**
         * \brief Returns the symbol interpreted as a const reference to an object.
         *
         * This function is useful when loading variables from the shared library.
         * Loading objects with automatic or dynamic storage duration has undefined
         * behavior.
         *
         * \tparam T Value type to convert the symbol to.
         *
         * \return Lvalue const reference of type \p T to the symbol.
         *
         * ``` cpp
         * // Load a global variable symbol from a DynamicLibrary:
         * auto myLibGlobalVariable = myLibrary.getSymbol("my_lib_global_variable");
         *
         * // Interpret as an int and read its value:
         * std::cout << "myLib::my_lib_global_variable: " << myLibGlobalVariable.getObject<int>();
         * ```
         */
        template<typename T>
        const T& getObject() const
        {
            return *get<T*>();
        }
    private:
        void* _symbolHandle;
        std::string _symbolName;
        DynamicLibrary* _library;

        Symbol(DynamicLibrary& library, void* symbolHandle, const std::string& symbolName);

        static Symbol load(DynamicLibrary& library, const std::string& symbolName);
    };

    /**
     * \brief Loads a symbol from this shared library.
     *
     * Library loaded symbols are cached by DynamicLibrary instances to avoid
     * invoking the OS API again on an already loaded symbol. First call to
     * getSymbol(<symbol name>) invokes DynamicLibrary::Symbol::loadSymbol() using
     * *this as first argument. Consecuent calls read symbols from the cache.
     *
     * \param name Name of the symbol to load.
     *
     * @return Reference to the cached symbol.
     */
    Symbol& getSymbol(const std::string& name);

    /**
     * \brief OS-specific shared library handle.
     *
     * @return `void*` pointer representing the OS-specific
     * library handle.
     */
    void* handle() const;

    /**
     * \brief Shared library path. See DynamicLibrary::load().
     *
     * @return const reference to the string containing the path to the library.
     */
    const std::string& path() const;

private:
    std::shared_ptr<void> _libraryHandle;
    std::string _libraryPath;

    std::unordered_map<std::string, Symbol> _loadedSymbols;

    DynamicLibrary(void* libraryHandle, const std::string& libraryPath);
};

}

#endif // SIPLASPLAS_UTILITY_DYNAMICLIBRARY_HPP
