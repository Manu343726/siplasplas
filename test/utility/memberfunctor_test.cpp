#include <siplasplas/utility/memberfunctor.hpp>
#include <gmock/gmock.h>

using namespace ::testing;
using namespace ::cpp;

struct Empty {};
struct NotEmpty
{
    int foo;
};

struct InheritEmpty : public Empty
{
    int foo;
};

struct AggregateEmpty
{
    int foo;
    Empty bar;
};

struct InheritNotEmpty : public NotEmpty
{
    int foo;
};

struct AggregateNotEmpty
{
    int foo;
    NotEmpty bar;
};

struct StatelessFunctor
{
    int operator()() { return 42; }
};

struct StatefulFunctor
{
public:
    int i;

    int operator()() const { return 42; }
};

static_assert(sizeof(InheritEmpty) == sizeof(int), "???");
static_assert(sizeof(AggregateEmpty) > sizeof(int), "???");
static_assert(sizeof(InheritNotEmpty) == sizeof(int) * 2, "???");
static_assert(sizeof(AggregateNotEmpty) == sizeof(int) * 2, "???");
static_assert(IsEboCandidate<Empty>::value, "???");
static_assert(!IsEboCandidate<NotEmpty>::value, "???");

static_assert(IsEboCandidate<StatelessFunctor>::value, "???");
static_assert(!IsEboCandidate<StatefulFunctor>::value, "???");

class FunctorInterface
{
public:
    virtual ~FunctorInterface() = default;
    virtual void invoke(const std::string&) = 0;

    void operator()(const std::string& str)
    {
        invoke(str);
    }
};

class StatelessFunctorMock : public FunctorInterface
{
public:
    MOCK_METHOD1(invoke, void(const std::string&));
};

class StatefulFunctorMock : public StatelessFunctorMock
{
    int i = 0;
};

template<typename FunctorMock>
class FunctorUser : MemberFunctor<FunctorMock>
{
public:
    using Functor = MemberFunctor<FunctorMock>;

    FunctorMock& getFunctor()
    {
        return Functor::get();
    }

    void run()
    {
        Functor::invoke("hello");
    }
};

TEST(MemberFunctorTest, invoke_invokesStatelessFunctor)
{
    FunctorUser<StatelessFunctorMock> user;
    EXPECT_CALL(user.getFunctor(), invoke(std::string("hello")));
    user.run();
}

TEST(MemberFunctorTest, apply_invokesStatefulFunctor)
{
    FunctorUser<StatefulFunctorMock> user;
    EXPECT_CALL(user.getFunctor(), invoke(std::string("hello")));
    user.run();
}
