#ifndef SIPLASPLAS_TEST_TYPEERASURE_MOCKS_IOSTREAM_HPP
#define SIPLASPLAS_TEST_TYPEERASURE_MOCKS_IOSTREAM_HPP

struct NoLeftShiftOverload
{};

struct LeftShiftOverload
{
    int operator<<(int)
    {
        return 42;
    }
};

struct StreamLeftShiftOverload
{
    friend std::ostream& operator<<(std::ostream& os, const StreamLeftShiftOverload&)
    {
        return os << 42;
    }
};


struct NoRightShiftOverload
{};

struct RightShiftOverload
{
    int operator>>(int)
    {
        return 42;
    }
};

struct StreamRightShiftOverload
{
    friend std::istream& operator>>(std::istream& is, StreamRightShiftOverload&)
    {
        return is;
    }
};
#endif // SIPLASPLAS_TEST_TYPEERASURE_MOCKS_IOSTREAM_HPP
