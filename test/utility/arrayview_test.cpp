#include <siplasplas/utility/array_view.hpp>
#include <gmock/gmock.h>
using namespace ::testing;

static_assert(
    cpp::constArrayView("hello") == cpp::constArrayView("hello"),
    "string literals must be equality comparable"
);
static_assert(
    cpp::constArrayView("hello") != cpp::constArrayView("world"),
    "string literals must be inequality comparable"
);

TEST(ArrayViewTest, stringView_toStdString)
{
    EXPECT_EQ(cpp::ConstStringView("hello").str(), std::string("hello"));
    EXPECT_EQ(std::string(cpp::ConstStringView("hello").c_str()), std::string("hello"));
}
