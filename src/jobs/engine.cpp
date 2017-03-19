#include "engine.hpp"
#include <random>

using namespace cpp::jobs;

Engine::Engine(std::size_t workerThreads, std::size_t jobsPerThread) :
    _workers{workerThreads},
    _randomEngine{std::random_device()()},
    _dist{0, workerThreads}
{
    std::size_t jobsPerQueue = jobsPerThread;
    _workers.emplaceBack(this, jobsPerQueue, Worker::Mode::Foreground);

    for(std::size_t i = 1; i < workerThreads; ++i)
    {
        _workers.emplaceBack(this, jobsPerQueue, Worker::Mode::Background);
    }

    for(auto& worker : _workers)
    {
        worker.run();
    }
}

Worker* Engine::randomWorker()
{
    Worker* worker = &_workers[_dist(_randomEngine)];

    if(worker->running())
    {
        return worker;
    }
    else
    {
        return nullptr;
    }
}

Worker* Engine::findThreadWorker(const std::thread::id threadId)
{
    for(auto& worker : _workers)
    {
        if(threadId == worker.threadId())
        {
            return &worker;
        }
    }

    return nullptr;
}

cpp::constexp::ConstArrayView<Worker> Engine::workers() const
{
    return { &_workers[0], &_workers[_workers.size()] };
}

cpp::constexp::ArrayView<Worker> Engine::workers()
{
    return { &_workers[0], &_workers[_workers.size()] };
}

Engine::~Engine()
{
    for(auto& worker : _workers)
    {
        worker.stop();
    }
}

std::size_t Engine::totalJobsRun() const
{
    std::size_t total = 0;

    for(const auto& worker : _workers)
    {
        total += worker.totalJobsRun();
    }

    return total;
}

Worker* Engine::threadWorker()
{
    static thread_local Engine* engine = this;
    static thread_local Worker* worker = findThreadWorker(std::this_thread::get_id());

    if(engine != this)
    {
        engine = this;
        worker = findThreadWorker(std::this_thread::get_id());
    }

    return worker;
}
