#include <siplasplas/jobs/pool.hpp>
#include <gmock/gmock.h>

using namespace cpp::jobs;
using namespace testing;

class PoolTest : public Test
{
protected:
    PoolTest() :
        pool{16*1024}
    {}

    Pool pool;
};

TEST_F(PoolTest, noJobsAllocatedByDefault)
{
    EXPECT_EQ(0, pool.jobs());
}

TEST_F(PoolTest, nomberOfJobsIncreasesAfterAlloc)
{
    for(std::size_t i = 0; i < pool.maxJobs(); ++i)
    {
        EXPECT_EQ(i, pool.jobs());
        pool.allocate();
        EXPECT_EQ(i + 1, pool.jobs());
    }
}

TEST_F(PoolTest, allocateReturnsNullIfPoolFull)
{
    for(std::size_t i = 0; i < pool.maxJobs(); ++i)
    {
        pool.allocate();
    }

    EXPECT_EQ(pool.maxJobs(), pool.jobs());
    EXPECT_EQ(nullptr, pool.allocate());
}
