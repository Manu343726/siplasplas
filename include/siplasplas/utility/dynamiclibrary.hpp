#ifndef SIPLASPLAS_UTILITY_DYNAMICLIBRARY_HPP
#define SIPLASPLAS_UTILITY_DYNAMICLIBRARY_HPP

#include <string>
#include <unordered_map>
#include <siplasplas/utility/export.hpp>

namespace cpp
{

class SIPLASPLAS_UTILITY_EXPORT DynamicLibrary
{
public:
    static DynamicLibrary load(const std::string& libraryPath);

    ~DynamicLibrary();

    class SIPLASPLAS_UTILITY_EXPORT Symbol
    {
        friend class DynamicLibrary;
    public:
        Symbol() = default;

        const std::string& name() const;

              void* handle();
        const void* handle() const;

        template<typename T>
        T get()
        {
            return reinterpret_cast<T>(handle());
        }

        template<typename T>
        T& getObject()
        {
            return *get<T*>();
        }

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

    Symbol& getSymbol(const std::string& name);

    void* handle() const;
    const std::string& path() const;

private:
    void* _libraryHandle;
    const std::string _libraryPath;

    std::unordered_map<std::string, Symbol> _loadedSymbols;

    DynamicLibrary(void* libraryHandle, const std::string& libraryPath);
};

}

#endif // SIPLASPLAS_UTILITY_DYNAMICLIBRARY_HPP
