#ifndef SIPLASPLAS_TEST_TYPEERASURE_CONCEPTS_MOCKS_HPP
#define SIPLASPLAS_TEST_TYPEERASURE_CONCEPTS_MOCKS_HPP

struct DefaultConstructible
{
    DefaultConstructible() {}
};

struct NoThrowDefaultConstructible
{
    NoThrowDefaultConstructible() noexcept {}
};

struct Constructible
{
    template<typename... Args>
    Constructible(Args&&...) {}
};

struct NoThrowConstructible
{
    template<typename... Args>
    NoThrowConstructible(Args&&...) noexcept {}
};

struct CopyConstructible
{
    CopyConstructible() {}; // to instance lvalues in tests
    CopyConstructible(const CopyConstructible&) {}
    CopyConstructible(CopyConstructible&&) {}
    CopyConstructible& operator=(const CopyConstructible&) = default;
    CopyConstructible& operator=(CopyConstructible&&) = default;
};

struct NoThrowCopyConstructible
{
    NoThrowCopyConstructible(const NoThrowCopyConstructible&) noexcept {}
};

struct MoveConstructible
{
    MoveConstructible() {} // to instantiate lvalues in tests
    MoveConstructible(const MoveConstructible&) {}
    MoveConstructible(MoveConstructible&&) {}
};

struct NoThrowMoveConstructible
{
    NoThrowMoveConstructible(NoThrowMoveConstructible&&) noexcept {}
};

struct Assignable
{
    template<typename T>
    Assignable& operator=(const T&)
    {
        return *this;
    }
};

struct NoThrowAssignable
{
    template<typename T>
    NoThrowAssignable& operator=(const T&) noexcept
    {
        return *this;
    }
};

struct CopyAssignable
{
    CopyAssignable& operator=(const CopyAssignable&)
    {
        return *this;
    }
};

struct NoThrowCopyAssignable
{
    NoThrowCopyAssignable& operator=(const NoThrowCopyAssignable&) noexcept
    {
        return *this;
    }
};

struct MoveAssignable
{
    MoveAssignable& operator=(MoveAssignable&&)
    {
        return *this;
    }
};

struct NoThrowMoveAssignable
{
    NoThrowMoveAssignable& operator=(NoThrowMoveAssignable&&) noexcept
    {
        return *this;
    }
};

struct Destructible
{
    Destructible() {}
    ~Destructible() noexcept(false) {}
};

struct NoThrowDestructible
{
    NoThrowDestructible() {}
    ~NoThrowDestructible() noexcept {}
};

class NoDefaultConstructible
{
    NoDefaultConstructible() = default;
};

class NoNoThrowDefaultConstructible
{
    NoNoThrowDefaultConstructible() noexcept = default;
};

class NoConstructible
{
    template<typename... Args>
    NoConstructible(Args&&...) = default;
};

class NoNoThrowConstructible
{
    template<typename... Args>
    NoNoThrowConstructible(Args&&...) noexcept = default;
};

struct NoCopyConstructible
{
    NoCopyConstructible() {}; // to instance lvalues in tests
    NoCopyConstructible(NoCopyConstructible&&) {}

private:
    NoCopyConstructible(const NoCopyConstructible&) = default;
};

class NoNoThrowCopyConstructible
{
    NoNoThrowCopyConstructible(const NoNoThrowCopyConstructible&) noexcept = default;
};

struct NoMoveConstructible
{
    NoMoveConstructible() {} // to instantiate lvalues in tests
    NoMoveConstructible(const NoMoveConstructible&) {}

private:
    NoMoveConstructible(NoMoveConstructible&&) = default;
};

class NoNoThrowMoveConstructible
{
    NoNoThrowMoveConstructible(NoNoThrowMoveConstructible&&) noexcept = default;
};

class NoAssignable
{
    template<typename T>
    NoAssignable& operator=(const T&) = default;
};

class NoNoThrowAssignable
{
    template<typename T>
    NoNoThrowAssignable& operator=(const T&) noexcept = default;
};

class NoCopyAssignable
{
    NoCopyAssignable& operator=(const NoCopyAssignable&) = default;
};

class NoNoThrowCopyAssignable
{
    NoNoThrowCopyAssignable& operator=(const NoNoThrowCopyAssignable&) noexcept = default;
};

class NoMoveAssignable
{
    NoMoveAssignable& operator=(NoMoveAssignable&&) = default;
};

class NoNoThrowMoveAssignable
{
    NoNoThrowMoveAssignable& operator=(NoNoThrowMoveAssignable&&) noexcept = default;
};

struct NoDestructible
{
    NoDestructible() {}

private:
    ~NoDestructible() = default;
};

struct NoNoThrowDestructible
{
    NoNoThrowDestructible() {}

private:
    ~NoNoThrowDestructible() noexcept = default;
};

#endif // SIPLASPLAS_TEST_TYPEERASURE_CONCEPTS_MOCKS_HPP
