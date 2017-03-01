#include "engine.hpp"
#include <random>

using namespace cpp::jobs;

Engine::Engine(std::size_t workerThreads, std::size_t jobsPerThread)
{
    std::size_t jobsPerQueue = jobsPerThread / 128;
    _workers.emplace_back(std::make_unique<Worker>(this, jobsPerQueue, Worker::Mode::Sync));

    for(std::size_t i = 1; i < workerThreads; ++i)
    {
        _workers.emplace_back(std::make_unique<Worker>(this, jobsPerQueue, Worker::Mode::Async));
    }

    for(auto& worker : _workers)
    {
        worker->run();
    }
}

Worker* Engine::randomWorker()
{
    std::random_device rd;
    std::default_random_engine gen{rd()};
    std::uniform_int_distribution<std::size_t> dist{0, _workers.size()};
    Worker* worker = _workers[dist(gen)].get();

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
