#include <gmock/gmock.h>
#include <siplasplas/reflection/dynamic/entity.hpp>
#include <siplasplas/reflection/dynamic/runtime.hpp>

using namespace ::testing;
using namespace ::cpp::dynamic_reflection;

class EntityForTest : public Entity
{
public:
    EntityForTest(const SourceInfo& sourceInfo) :
        Entity{sourceInfo}
    {}

    static std::shared_ptr<Entity> create(const SourceInfo& sourceInfo)
    {
        return std::shared_ptr<Entity>{new EntityForTest{sourceInfo}};
    }
};

class EntityTest : public Test
{
public:
    EntityTest() :
        runtime{"EntityTest"},
        sourceInfo{"::MyClass", SourceInfo::Kind::CLASS},
        entity{EntityForTest::create(
            sourceInfo
        )}
    {}
protected:
    Runtime runtime;
    SourceInfo sourceInfo;
    std::shared_ptr<Entity> entity;

    void attachEntityToRuntime()
    {
        ASSERT_TRUE(entity->detached());
        ASSERT_TRUE(entity->orphan());
        ASSERT_NO_THROW(entity->attach(runtime));
        ASSERT_FALSE(entity->detached());
        ASSERT_TRUE(entity->orphan());
    }

    void attachToGlobalNamespace()
    {
        ASSERT_TRUE(entity->detached());
        ASSERT_TRUE(entity->orphan());
        ASSERT_NO_THROW(entity->attach(runtime.namespace_().pointer()));
        ASSERT_FALSE(entity->detached());
        ASSERT_FALSE(entity->orphan());
    }
};


TEST_F(EntityTest, entityDetachedByDefault)
{
    EXPECT_TRUE(entity->detached());
}

TEST_F(EntityTest, entityOrphanByDefault)
{
    EXPECT_TRUE(entity->orphan());
}

TEST_F(EntityTest, detachedEntity_attachToRuntime_attachDoesntThrowAndEntityAttached)
{
    ASSERT_TRUE(entity->detached());
    ASSERT_TRUE(entity->orphan());
    ASSERT_NO_THROW(entity->attach(runtime));
    EXPECT_FALSE(entity->detached());
    EXPECT_TRUE(entity->orphan());
}

TEST_F(EntityTest, detachedEntity_attachToParent_attachDoesntThrowAndEntityAttached)
{
    ASSERT_TRUE(entity->detached());
    ASSERT_TRUE(entity->orphan());
    ASSERT_NO_THROW(entity->attach(runtime.namespace_().pointer()));
    EXPECT_FALSE(entity->detached());
    EXPECT_FALSE(entity->orphan());
}

TEST_F(EntityTest, attachedEntity_attachToRuntimeThrows)
{
    attachEntityToRuntime();
    EXPECT_THROW(entity->attach(runtime), std::runtime_error);
}

TEST_F(EntityTest, attachedEntity_attachToParentThrows)
{
    attachToGlobalNamespace();
    EXPECT_THROW(entity->attach(runtime.namespace_().pointer()), std::runtime_error);
}

TEST_F(EntityTest, addChild_detachedEntity_entityAttachedAndIsChild)
{
    runtime.namespace_().addChild(entity);
    EXPECT_FALSE(entity->detached());
    EXPECT_FALSE(entity->orphan());
    EXPECT_TRUE(runtime.namespace_().isChild(entity));
    EXPECT_EQ(runtime.namespace_(), entity->parent());
}
