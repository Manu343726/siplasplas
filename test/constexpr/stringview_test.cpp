#include <siplasplas/constexpr/stringview.hpp>
#include <gmock/gmock.h>

using namespace ::testing;
using namespace ::cpp::constexp;

TEST(ArrayViewTest, stringView_toStdString)
{
    EXPECT_EQ(ConstStringView("hello").str(), std::string("hello"));
    EXPECT_EQ(std::string(ConstStringView("hello").c_str()), std::string("hello"));
}
