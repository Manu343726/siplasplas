#include "worker.hpp"
#include "engine.hpp"
#include "logger.hpp"

using namespace cpp::jobs;

Worker::Worker(Engine* engine, std::size_t poolSize, Worker::Mode mode) :
    _workQueue{poolSize},
    _pool{poolSize},
    _engine{engine},
    _running{false},
    _mode{mode},
    _totalJobsRun{0},
    _totalJobsDiscarded{0},
    _cyclesWithoutJobs{0},
    _maxCyclesWithoutJobs{0}
{}

void Worker::run()
{
    if(_running)
    {
        return;
    }

    auto mainLoop = [this]
    {
        _running = true;
        _state = State::Running;

        jobs::log().debug("[worker {}] Main loop started...", threadId());

        while(_running)
        {
            Job* job = getJob();

            if(job != nullptr)
            {
                job->run();
                ++_totalJobsRun;
                _cyclesWithoutJobs = 0;
            }
            else
            {
                ++_cyclesWithoutJobs;
                _maxCyclesWithoutJobs = std::max(_cyclesWithoutJobs, _maxCyclesWithoutJobs);

                if(_cyclesWithoutJobs % 1000 == 0)
                {
                    jobs::log().debug("[worker {}] Worker::run(): {} cycles without getting any job to run",
                        _cyclesWithoutJobs, threadId());
                }
            }
        }

        _state = State::Stopping;
        jobs::log().debug("[worker {}] Stopping...", threadId());
    };



    if(_mode == Mode::Async)
    {
        _workerThread = std::thread{mainLoop};
        _workerThreadId = _workerThread.get_id();
    }
    else
    {
        _running = true;
        _state = State::Running;
        _workerThreadId = std::this_thread::get_id();
    }

    jobs::log().debug("[worker {}] Starting...", threadId());
}

void Worker::stop()
{
    _running = false;
    join();
    _state = State::Idle;

    jobs::log().debug("Worker {} stopped. Stopped by thread {}",
        threadId(), std::this_thread::get_id());
}

Worker::~Worker()
{
    stop();
}

void Worker::join()
{
    if(std::this_thread::get_id() != threadId() && _workerThread.joinable())
    {
        jobs::log().debug("Waiting for worker {} to exit...", threadId());
        _workerThread.join();
        jobs::log().debug("Worker thread {} finished", threadId());
    }
}

bool Worker::running() const
{
    return _running;
}

void Worker::submit(Job* job)
{
    if(job != nullptr && !_workQueue.push(job))
    {
        job->discard();
        ++_totalJobsDiscarded;
    }
}

void Worker::wait(Job* job)
{
    while(!job->finished())
    {
        Job* job = getJob();

        if(job != nullptr)
        {
            job->run();
            ++_totalJobsRun;
            _cyclesWithoutJobs = 0;
        }
        else
        {
            ++_cyclesWithoutJobs;
            _maxCyclesWithoutJobs = std::max(_cyclesWithoutJobs, _maxCyclesWithoutJobs);

            if(_cyclesWithoutJobs % 1000 == 0)
            {
                jobs::log().debug("[worker {}] Worker::wait(): {} cycles without getting any job to run",
                    _cyclesWithoutJobs, threadId());
            }
        }
    }
}

Pool& Worker::pool()
{
    return _pool;
}

Job* Worker::getJob()
{
    Job* job = _workQueue.pop();

    if(job != nullptr)
    {
        return job;
    }
    else
    {
        // Stole job from another worker

        Worker* worker = _engine->randomWorker();

        if(worker == this)
        {
            std::this_thread::yield();
            return nullptr;
        }
        else
        {
            if(worker != nullptr)
            {
                return worker->_workQueue.steal();
            }
            else
            {
               std::this_thread::yield();
               return nullptr;
            }
        }
    }
}

std::thread::id Worker::threadId() const
{
    return _workerThreadId;
}

const std::atomic<Worker::State>& Worker::state() const
{
    return _state;
}

std::size_t Worker::totalJobsRun() const
{
    return _totalJobsRun;
}

std::size_t Worker::cyclesWithoutJobs() const
{
    return _cyclesWithoutJobs;
}

std::size_t Worker::maxCyclesWithoutJobs() const
{
    return _maxCyclesWithoutJobs;
}

std::size_t Worker::totalJobsDiscarded() const
{
    return _totalJobsDiscarded;
}
