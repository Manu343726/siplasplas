#include <siplasplas/jobs/engine.hpp>
#include <gmock/gmock.h>
#include <iostream>

using namespace cpp::jobs;
using namespace testing;

TEST(EngineTest, main)
{
    Engine engine{4, 1024*1024};
    Worker* worker = engine.threadWorker();

    Job* root = worker->pool().createJob([](Job& job)
    {
        // NOP
    });

    for(std::size_t i = 0; i < 1024*1024; ++i)
    {
        Job* childJob = worker->pool().createJobAsChild([](Job& job)
        {
            std::cout << "[" << std::this_thread::get_id() << "] Child job " << static_cast<void*>(&job) << "\n";
        }, root);

        worker->submit(childJob);
    }

    worker->submit(root);
    worker->wait(root);
}
