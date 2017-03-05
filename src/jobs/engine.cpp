#include "engine.hpp"
#include <random>

using namespace cpp::jobs;

Engine::Engine(std::size_t workerThreads, std::size_t jobsPerThread) :
    _randomEngine{std::random_device()()},
    _dist{0, workerThreads}
{
    std::size_t jobsPerQueue = jobsPerThread / 128;
    _workers.emplace_back(std::make_unique<Worker>(this, jobsPerQueue, Worker::Mode::Foreground));

    for(std::size_t i = 1; i < workerThreads; ++i)
    {
        _workers.emplace_back(std::make_unique<Worker>(this, jobsPerQueue, Worker::Mode::Background));
    }

    for(auto& worker : _workers)
    {
        worker->run();
    }
}

Worker* Engine::randomWorker()
{
    Worker* worker = _workers[_dist(_randomEngine)].get();

    if(worker != nullptr && worker->running())
    {
        return worker;
    }
    else
    {
        return nullptr;
    }
}

Worker* Engine::threadWorker()
{
    for(auto& worker : _workers)
    {
        if(std::this_thread::get_id() == worker->threadId())
        {
            return worker.get();
        }
    }

    return nullptr;
}

cpp::constexp::ConstArrayView<std::unique_ptr<Worker>> Engine::workers() const
{
    return { &_workers[0], &_workers[_workers.size()] };
}
