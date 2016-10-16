#include <siplasplas/utility/meta.hpp>
#include <gmock/gmock.h>
using namespace cpp::meta;
using namespace testing;

template<typename Seq>
constexpr auto sequence()
{
    return SequenceToArray<Seq, IntegralConstantToValue<int>>();
}

TEST(MetaTest, merge)
{
    EXPECT_EQ(
        (sequence<list<>>()),
        (sequence<merge_t<
            list<>,
            list<>
        >>())
    );

    EXPECT_EQ(
        (sequence<integer_sequence<int, 0, 1>>()),
        (sequence<merge_t<
            list<>,
            integer_sequence<int, 0, 1>
        >>())
    );

    EXPECT_EQ(
        (sequence<integer_sequence<int, 0, 1>>()),
        (sequence<merge_t<
            integer_sequence<int, 0, 1>,
            list<>
        >>())
    );

    EXPECT_EQ(
        (sequence<integer_sequence<int, 0, 1>>()),
        (sequence<merge_t<
            integer_sequence<int, 0>,
            integer_sequence<int, 1>
        >>())
    );

    EXPECT_EQ(
        (sequence<integer_sequence<int, 0, 1, 2, 3>>()),
        (sequence<merge_t<
            integer_sequence<int, 2, 3>,
            integer_sequence<int, 0, 1>
        >>())
    );
}

TEST(MetaTest, sort)
{
    using Seq1 = integer_sequence<int, 1, 2, 3, 4, 5, 6>;
    using Seq2 = sort_t<integer_sequence<int, 6, 5, 4, 3, 2, 1>, less>;
    constexpr auto seq1 = SequenceToArray<Seq1, IntegralConstantToValue<int>>();
    constexpr auto seq2 = SequenceToArray<Seq2, IntegralConstantToValue<int>>();

    EXPECT_EQ(
        seq1,
        seq2
    );
}
