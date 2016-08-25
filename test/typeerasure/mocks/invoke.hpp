#ifndef SIPLASPLAS_TEST_TYPEERASURE_MOCKS_INVOKE_HPP
#define SIPLASPLAS_TEST_TYPEERASURE_MOCKS_INVOKE_HPP

#include <string>

inline int addIntsByValue(int a, int b)
{
    return a + b;
}

inline std::string addStringsByConstReference(const std::string& a, const std::string& b)
{
    return a + b;
}

class Class
{
public:
    int addIntsByValue(int a, int b)
    {
        return a + b;
    }

    int addIntsByValueConst(int a, int b) const
    {
        return a + b;
    }

    std::string addStringsByConstReference(const std::string& a, const std::string& b)
    {
        return a + b;
    }

    std::string addStringsByConstReferenceConst(const std::string& a, const std::string& b) const
    {
        return a + b;
    }

    int i = 42;
    std::string str = "hello";
};

#endif // SIPLASPLAS_TEST_TYPEERASURE_MOCKS_INVOKE_HPP
