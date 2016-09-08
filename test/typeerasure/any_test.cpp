#include "mocks/invoke.hpp"
#include <siplasplas/typeerasure/any.hpp>
#include <gmock/gmock.h>

using namespace ::testing;
using namespace ::std::string_literals;
using namespace ::cpp;

TEST(AnyTest, assignedMethodsAreRegistered)
{
    Any8  any8{Class()};
    Any16 any16{Class()};
    Any32 any32{Class()};
    Any64 any64{Class()};

    any8("a") = &Class::addIntsByValue;
    any8("b") = &Class::addStringsByConstReference;
    any8("c") = &Class::addIntsByValueConst;
    any8("d") = &Class::addStringsByConstReferenceConst;

    any16("a") = &Class::addIntsByValue;
    any16("b") = &Class::addStringsByConstReference;
    any16("c") = &Class::addIntsByValueConst;
    any16("d") = &Class::addStringsByConstReferenceConst;

    any32("a") = &Class::addIntsByValue;
    any32("b") = &Class::addStringsByConstReference;
    any32("c") = &Class::addIntsByValueConst;
    any32("d") = &Class::addStringsByConstReferenceConst;

    any64("a") = &Class::addIntsByValue;
    any64("b") = &Class::addStringsByConstReference;
    any64("c") = &Class::addIntsByValueConst;
    any64("d") = &Class::addStringsByConstReferenceConst;

    EXPECT_TRUE(any8.hasMethod("a"));
    EXPECT_EQ(cpp::hash(&Class::addIntsByValue), cpp::hash(any8("a").method().get<decltype(&Class::addIntsByValue)>()));
    EXPECT_TRUE(any16.hasMethod("a"));
    EXPECT_EQ(cpp::hash(&Class::addIntsByValue), cpp::hash(any16("a").method().get<decltype(&Class::addIntsByValue)>()));
    EXPECT_TRUE(any32.hasMethod("a"));
    EXPECT_EQ(cpp::hash(&Class::addIntsByValue), cpp::hash(any32("a").method().get<decltype(&Class::addIntsByValue)>()));
    EXPECT_TRUE(any64.hasMethod("a"));
    EXPECT_EQ(cpp::hash(&Class::addIntsByValue), cpp::hash(any64("a").method().get<decltype(&Class::addIntsByValue)>()));

    EXPECT_TRUE(any8.hasMethod("b"));
    EXPECT_EQ(cpp::hash(&Class::addStringsByConstReference), cpp::hash(any8("b").method().get<decltype(&Class::addStringsByConstReference)>()));
    EXPECT_TRUE(any16.hasMethod("b"));
    EXPECT_EQ(cpp::hash(&Class::addStringsByConstReference), cpp::hash(any16("b").method().get<decltype(&Class::addStringsByConstReference)>()));
    EXPECT_TRUE(any32.hasMethod("b"));
    EXPECT_EQ(cpp::hash(&Class::addStringsByConstReference), cpp::hash(any32("b").method().get<decltype(&Class::addStringsByConstReference)>()));
    EXPECT_TRUE(any64.hasMethod("b"));
    EXPECT_EQ(cpp::hash(&Class::addStringsByConstReference), cpp::hash(any64("b").method().get<decltype(&Class::addStringsByConstReference)>()));

    EXPECT_TRUE(any8.hasMethod("c"));
    EXPECT_EQ(cpp::hash(&Class::addIntsByValueConst), cpp::hash(any8("c").method().get<decltype(&Class::addIntsByValueConst)>()));
    EXPECT_TRUE(any16.hasMethod("c"));
    EXPECT_EQ(cpp::hash(&Class::addIntsByValueConst), cpp::hash(any16("c").method().get<decltype(&Class::addIntsByValueConst)>()));
    EXPECT_TRUE(any32.hasMethod("c"));
    EXPECT_EQ(cpp::hash(&Class::addIntsByValueConst), cpp::hash(any32("c").method().get<decltype(&Class::addIntsByValueConst)>()));
    EXPECT_TRUE(any64.hasMethod("c"));
    EXPECT_EQ(cpp::hash(&Class::addIntsByValueConst), cpp::hash(any64("c").method().get<decltype(&Class::addIntsByValueConst)>()));

    EXPECT_TRUE(any8.hasMethod("d"));
    EXPECT_EQ(cpp::hash(&Class::addStringsByConstReferenceConst), cpp::hash(any8("d").method().get<decltype(&Class::addStringsByConstReferenceConst)>()));
    EXPECT_TRUE(any16.hasMethod("d"));
    EXPECT_EQ(cpp::hash(&Class::addStringsByConstReferenceConst), cpp::hash(any16("d").method().get<decltype(&Class::addStringsByConstReferenceConst)>()));
    EXPECT_TRUE(any32.hasMethod("d"));
    EXPECT_EQ(cpp::hash(&Class::addStringsByConstReferenceConst), cpp::hash(any32("d").method().get<decltype(&Class::addStringsByConstReferenceConst)>()));
    EXPECT_TRUE(any64.hasMethod("d"));
    EXPECT_EQ(cpp::hash(&Class::addStringsByConstReferenceConst), cpp::hash(any64("d").method().get<decltype(&Class::addStringsByConstReferenceConst)>()));
}

TEST(AnyTest, referenceAny_pointsToSameObject)
{
    Class object;

    ReferenceAny  any{object};
    ConstReferenceAny constAny{object};

    EXPECT_EQ(&any.get<Class>(), &object);
    EXPECT_EQ(&constAny.get<Class>(), &object);
}

TEST(AnyTest, any_assignedFields_referenceRightMemberObjects)
{
    Any8  any8{Class()};
    Any16 any16{Class()};
    Any32 any32{Class()};
    Any64 any64{Class()};

    any8["i"] = &Class::i;
    any8["str"] = &Class::str;

    any16["i"] = &Class::i;
    any16["str"] = &Class::str;

    any32["i"] = &Class::i;
    any32["str"] = &Class::str;

    any64["i"] = &Class::i;
    any64["str"] = &Class::str;

    EXPECT_TRUE(any8.hasAttribute("i"));
    EXPECT_EQ(&any8.get<Class>().i, &any8["i"].get<int>());
    EXPECT_TRUE(any8.hasAttribute("str"));
    EXPECT_EQ(&any8.get<Class>().str, &any8["str"].get<std::string>());

    EXPECT_TRUE(any16.hasAttribute("i"));
    EXPECT_EQ(&any16.get<Class>().i, &any16["i"].get<int>());
    EXPECT_TRUE(any16.hasAttribute("str"));
    EXPECT_EQ(&any16.get<Class>().str, &any16["str"].get<std::string>());

    EXPECT_TRUE(any32.hasAttribute("i"));
    EXPECT_EQ(&any32.get<Class>().i, &any32["i"].get<int>());
    EXPECT_TRUE(any32.hasAttribute("str"));
    EXPECT_EQ(&any32.get<Class>().str, &any32["str"].get<std::string>());

    EXPECT_TRUE(any64.hasAttribute("i"));
    EXPECT_EQ(&any64.get<Class>().i, &any64["i"].get<int>());
    EXPECT_TRUE(any64.hasAttribute("str"));
    EXPECT_EQ(&any64.get<Class>().str, &any64["str"].get<std::string>());
    EXPECT_EQ(&any64.get<Class>().str, &any64["str"].get<std::string>());
}

TEST(AnyTest, referenceAny_assignedFields_referenceRightMemberObjcts)
{
    Class object;
    
    ReferenceAny any{object};
    ConstReferenceAny constAny{object};

    any["i"] = &Class::i;
    any["str"] = &Class::str;

    constAny["i"] = &Class::i;
    constAny["str"] = &Class::str;

    EXPECT_TRUE(any.hasAttribute("i"));
    EXPECT_EQ(&object.i, &any["i"].get<int>());
    EXPECT_EQ(&object.i, &any["i"].attribute().getAs<int>(object));
    EXPECT_TRUE(any.hasAttribute("str"));
    EXPECT_EQ(&object.str, &any["str"].get<std::string>());
    EXPECT_EQ(&object.str, &any["str"].attribute().getAs<std::string>(object));

    EXPECT_TRUE(constAny.hasAttribute("i"));
    EXPECT_EQ(&object.i, &constAny["i"].get<int>());
    EXPECT_EQ(&object.i, &constAny["i"].attribute().getAs<int>(object));
    EXPECT_TRUE(constAny.hasAttribute("str"));
    EXPECT_EQ(&object.str, &constAny["str"].get<std::string>());
    EXPECT_EQ(&object.str, &constAny["str"].attribute().getAs<std::string>(object));
}

TEST(AnyTest, Any64_invokeMethod)
{
    cpp::Any64 any{Class()};

    SIPLASPLAS_ASSERT(any.hasType<Class>());

    any("addIntsByValue") = &Class::addIntsByValue;
    any("addIntsByValueConst") = &Class::addIntsByValueConst;

    EXPECT_EQ(42, any("addIntsByValue")(20, 22).get<int>());
    EXPECT_EQ(42, any("addIntsByValueConst")(20, 22).get<int>());
}

TEST(AnyTest, Any64_setFieldValue)
{
    cpp::Any64 any{Class()};

    SIPLASPLAS_ASSERT(any.hasType<Class>());

    any["i"] = &Class::i;
    any["str"] = &Class::str;

    EXPECT_EQ(&any.get<Class>().i, &any["i"].get<int>());
    EXPECT_EQ(any["i"].get<int>(), 42);
}
