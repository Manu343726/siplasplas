#include <siplasplas/jobs/job.hpp>
#include <gmock/gmock.h>

using namespace cpp::jobs;
using namespace testing;


class JobTest : public Test
{
protected:
    static void function(Job& job)
    {}

    struct JobData
    {
        int i;
    };

    static void functionWithData(Job& job)
    {
        EXPECT_EQ(42, job.getData<JobData>().i);
    }

    JobTest() :
        job{&JobTest::function},
        jobWithParent{&JobTest::function, &job},
        jobWithData{&JobTest::functionWithData, JobData{42}},
        jobWithDataAndParent{&JobTest::functionWithData, JobData{42}, &job}
    {}

    Job job, jobWithParent, jobWithData, jobWithDataAndParent;
};

TEST_F(JobTest, unfinishedByDefault)
{
    EXPECT_FALSE(job.finished());
    EXPECT_FALSE(jobWithParent.finished());
    EXPECT_FALSE(jobWithData.finished());
    EXPECT_FALSE(jobWithDataAndParent.finished());
}

TEST_F(JobTest, boundDataIsCorrectlyRead)
{
    EXPECT_EQ(42, jobWithData.getData<JobData>().i);
}

TEST_F(JobTest, jobsAreFinishedAfterRun)
{
    Job job{&JobTest::function};
    job.run();
    jobWithData.run();

    EXPECT_TRUE(job.finished());
    EXPECT_TRUE(jobWithData.finished());

    // JobTest::job is not finished since its child
    // jobs were not run yet (JobTest::jobWithDataAndParent and JobTest::jobWithParent)
    JobTest::job.run();
    EXPECT_FALSE(JobTest::job.finished());

    // Now we run the two child jobs and the parent must be done:
    jobWithParent.run();
    EXPECT_FALSE(JobTest::job.finished());
    jobWithDataAndParent.run();
    EXPECT_TRUE(jobWithData.finished());
    EXPECT_TRUE(jobWithDataAndParent.finished());
    EXPECT_TRUE(JobTest::job.finished());
}
