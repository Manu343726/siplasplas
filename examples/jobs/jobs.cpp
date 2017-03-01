#include <siplasplas/jobs/engine.hpp>
#include <siplasplas/utility/hash.hpp>
#include <cstdio>
#include <iostream>

using namespace cpp::jobs;

int main()
{
    Engine engine{4, 1024*1024*1024};
    Worker* worker = engine.threadWorker();

    Job* root = worker->pool().createJob([](Job& job)
    {
        // NOP
    });

    for(std::size_t i = 0; i < 1024*1024; ++i)
    {

#define CLOSURE_JOB
#ifdef CLOSURE_JOB
        Job* childJob = worker->pool().createClosureJobAsChild([&engine](Job& job)
        {
            for(std::size_t i = 0; i < 1024; ++i)
            {
                Worker* worker = engine.threadWorker();

                worker->submit(worker->pool().createJobAsChild([](Job& job){}, &job));
            }
        }, root);
#else
        Job* childJob = worker->pool().createJobAsChild([](Job& job)
        {
            for(std::size_t i = 0; i < 1024; ++i)
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

    for(const auto& worker : engine.workers())
    {
        std::cout << "Worker " << worker->threadId() << ": " << worker->totalJobsRun() << " jobs run"
            << " (discarded: " << worker->totalJobsDiscarded() << ", " << worker->pool().jobsFactor()*100
            << "% pool used, max cycles without jobs: " << worker->maxCyclesWithoutJobs() << ")\n";
    }

}
