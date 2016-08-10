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
    CopyConstructible(const CopyConstructible&) {}
};

struct NoThrowCopyConstructible
{
    NoThrowCopyConstructible(const NoThrowCopyConstructible&) noexcept {}
};

struct MoveConstructible
{
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
    ~Destructible() noexcept(false) {}
};

struct NoThrowDestructible
{
    ~NoThrowDestructible() noexcept {}
};

struct NoDefaultConstructible
{
    NoDefaultConstructible() = delete;
};

struct NoNoThrowDefaultConstructible
{
    NoNoThrowDefaultConstructible() noexcept = delete;
};

struct NoConstructible
{
    template<typename... Args>
    NoConstructible(Args&&...) = delete;
};

struct NoNoThrowConstructible
{
    template<typename... Args>
    NoNoThrowConstructible(Args&&...) noexcept = delete;
};

struct NoCopyConstructible
{
    NoCopyConstructible(const NoCopyConstructible&) = delete;
};

struct NoNoThrowCopyConstructible
{
    NoNoThrowCopyConstructible(const NoNoThrowCopyConstructible&) noexcept = delete;
};

struct NoMoveConstructible
{
    NoMoveConstructible(NoMoveConstructible&&) = delete;
};

struct NoNoThrowMoveConstructible
{
    NoNoThrowMoveConstructible(NoNoThrowMoveConstructible&&) noexcept = delete;
};

struct NoAssignable
{
    template<typename T>
    NoAssignable& operator=(const T&) = delete;
};

struct NoNoThrowAssignable
{
    template<typename T>
    NoNoThrowAssignable& operator=(const T&) noexcept = delete;
};

struct NoCopyAssignable
{
    NoCopyAssignable& operator=(const NoCopyAssignable&) = delete;
};

struct NoNoThrowCopyAssignable
{
    NoNoThrowCopyAssignable& operator=(const NoNoThrowCopyAssignable&) noexcept = delete;
};

struct NoMoveAssignable
{
    NoMoveAssignable& operator=(NoMoveAssignable&&) = delete;
};

struct NoNoThrowMoveAssignable
{
    NoNoThrowMoveAssignable& operator=(NoNoThrowMoveAssignable&&) noexcept = delete;
};

struct NoDestructible
{
    ~NoDestructible() = delete;
};

struct NoNoThrowDestructible
{
    ~NoNoThrowDestructible() noexcept = delete;
};

#endif // SIPLASPLAS_TEST_TYPEERASURE_CONCEPTS_MOCKS_HPP
