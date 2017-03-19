#include <siplasplas/jobs/engine.hpp>
#include <siplasplas/utility/hash.hpp>
#include <cstdio>
#include <iostream>

using namespace cpp::jobs;

int main()
{
    constexpr std::size_t threads = 4;
    constexpr std::size_t childJobs = 65*1000;
    constexpr std::size_t secondLevelChildJobs = 0;
    constexpr std::size_t totalJobs = childJobs + (childJobs * secondLevelChildJobs);


    Engine engine{threads, totalJobs};
    Worker* worker = engine.threadWorker();

    auto start = std::chrono::high_resolution_clock::now();

    Job* root = worker->pool().createJob([](Job& job)
    {
    });

    for(std::size_t i = 0; i < childJobs; ++i)
    {

#ifdef CLOSURE_JOB
        Job* childJob = worker->pool().createClosureJobAsChild([&engine](Job& job)
        {
            for(std::size_t i = 0; i < secondLevelChildJobs; ++i)
            {
                Worker* worker = engine.threadWorker();
                Job* childJob = worker->pool().createJobAsChild([](Job& job)
                {

                }, &job);

                worker->submit(childJob);
            }
        }, root);
#else
        Job* childJob = worker->pool().createJobAsChild([](Job& job)
        {
            for(std::size_t i = 0; i < secondLevelChildJobs; ++i)
            {
                Engine* engine = job.getData<Engine*>();
                Worker* worker = engine->threadWorker();

                worker->submit(worker->pool().createJobAsChild([](Job& job){}, &job));
            }
        }, &engine, root);
#endif // CLOSURE_JOB

        worker->submit(childJob);
    }

    worker->submit(root);
    worker->wait(root);

    auto totalTime = std::chrono::high_resolution_clock::now() - start;

    for(Worker& worker : engine.workers())
    {
        std::cout << "Worker " << worker.threadId() << ": " << worker.totalJobsRun() << " jobs run"
            << " (discarded: " << worker.totalJobsDiscarded() << ", " << worker.pool().jobsFactor()*100
            << "% pool used, max cycles without jobs: " << worker.maxCyclesWithoutJobs() << ")\n";
    }

    std::cout << "\nTotal jobs: " << totalJobs << "\n";
    std::cout << "Total jobs run: " << engine.totalJobsRun() << "\n";
    std::cout << "Total time: " << std::chrono::duration_cast<std::chrono::microseconds>(totalTime).count() / 1000.0f << " ms\n";
}
