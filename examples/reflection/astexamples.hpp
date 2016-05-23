
class Class
{
public:
    void f();
    int i;
};

enum OldEnum
{
    OLD_ENUM_VALUE_1 = 1,
    OLD_ENUM_VALUE_2 = 2
};

enum class EnumWithDefaultUnderlyingType
{
    ENUM_VALUE_1 = 1,
    ENUM_VALUE_2 = 2
};

enum class EnumWithIntUnderlyingType : int
{
    INT_ENUM_VALUE_1 = 1,
    INT_ENUM_VALUE_2 = 1 + 1
};

