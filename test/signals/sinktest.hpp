#ifndef SIPLASPLAS_TEST_SIGNALS_SINKTEST_HPP
#define SIPLASPLAS_TEST_SIGNALS_SINKTEST_HPP

#include <gmock/gmock.h>
#include <siplasplas/signals/emitter.hpp>
#include <siplasplas/signals/syncsink.hpp>
#include <siplasplas/utility/meta.hpp>


namespace signals_test
{

template<typename T>
class SinkMockInterface
{
public:
    virtual ~SinkMockInterface() = default;
    virtual void slotByValue(T value) = 0;
    virtual void slotByConstReference(const T& value) = 0;
};

template<typename T>
class SinkMock : public ::cpp::SignalEmitter, public SinkMockInterface<T>
{
public:
    MOCK_METHOD1_T(slotByValue, void(T));
    MOCK_METHOD1_T(slotByConstReference, void(const T&));
};

template<typename Sink, typename T>
class SignalSinkTest
{
public:
    using value_type = T;

    SignalSinkTest() :
        sinkByValue{new Sink{caller, callee, &SinkMock<T>::slotByValue}},
        sinkByConstReference{new Sink{caller, callee, &SinkMock<T>::slotByConstReference}}
    {}

    void SetUp()
    {
        ASSERT_EQ(sinkByValue->caller(), &caller);
        ASSERT_EQ(sinkByValue->callee(), &callee);
        ASSERT_EQ(sinkByConstReference->caller(), &caller);
        ASSERT_EQ(sinkByConstReference->callee(), &callee);
    }

    SinkMock<T> caller, callee;
    std::unique_ptr<::cpp::SignalSink> sinkByValue, sinkByConstReference;
};

template<typename Sink>
struct SignalSinkTestFactory
{
    template<typename T>
    struct apply
    {
        using type = SignalSinkTest<Sink, T>;
    };
};

template<typename T>
class Value;

template<>
class Value<int>
{
public:
    static int get()
    {
        return 42;
    }
};

template<>
class Value<std::string>
{
public:
    static std::string get()
    {
        return "hello, world!";
    }
};

template<typename T>
class Value<std::vector<T>>
{
public:
    static std::vector<T> get()
    {
        return std::vector<T>(4, Value<T>::get());
    }
};

template<typename Test>
typename Test::value_type value()
{
    return Value<typename Test::value_type>::get();
}

using types = cpp::meta::list<
    int,
    std::string,
    std::vector<int>,
    std::vector<std::string>
>;

template<typename Sink>
using Tests = cpp::meta::functor_t<
    ::testing::Types<>,
    cpp::meta::fmap_t<SignalSinkTestFactory<Sink>, types>
>;

}
#endif // SIPLASPLAS_TEST_SGINALS_SINKTEST_HPP
