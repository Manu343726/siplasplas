#include <siplasplas/constexpr/stringview.hpp>
#include <gmock/gmock.h>

using namespace ::testing;
using namespace ::cpp::constexp;


class StringViewTest : public Test
{
public:
    StringViewTest() :
        str(literal),
        cstr(str.c_str()),
        mutableCstr(const_cast<char*>(str.c_str()))
    {}

protected:
    static constexpr const char literal[] = "hello";

    std::string str;
    const char* cstr;
    char* mutableCstr;
};

constexpr const char StringViewTest::literal[];


TEST_F(StringViewTest, stringView_toStdString)
{
    EXPECT_EQ(ConstStringView(literal).str(), std::string(literal));
    EXPECT_EQ(std::string(ConstStringView(literal).c_str()), std::string(literal));
    EXPECT_EQ(std::string(ConstStringView("").c_str()), std::string(""));
}

TEST_F(StringViewTest, equalityComparison)
{
    EXPECT_EQ(ConstStringView(str), ConstStringView(str));
    EXPECT_EQ(StringView(str), ConstStringView(str));
    EXPECT_EQ(ConstStringView(str), StringView(str));

    EXPECT_EQ(ConstStringView(cstr), ConstStringView(cstr));
    EXPECT_EQ(StringView(mutableCstr), ConstStringView(cstr));
    EXPECT_EQ(ConstStringView(cstr), StringView(mutableCstr));

    EXPECT_EQ(ConstStringView(str), ConstStringView(cstr));
    EXPECT_EQ(StringView(mutableCstr), ConstStringView(str));
}

TEST_F(StringViewTest, foreignEqualityComparison)
{
    EXPECT_EQ(ConstStringView(cstr), str);
    EXPECT_EQ(StringView(mutableCstr), str);
    EXPECT_EQ(str, ConstStringView(cstr));
    EXPECT_EQ(str, StringView(mutableCstr));
}

