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
            }
        }
    };

    if(_mode == Mode::Background)
    {
        _workerThread = std::thread{mainLoop};
        _workerThreadId = _workerThread.get_id();
    }
    else
    {
        _workerThreadId = std::this_thread::get_id();
    }

    _running = true;
    _state = State::Running;
}

void Worker::stop()
{
    bool expected = true;
    while(!_running.compare_exchange_weak(expected, false));

    _state = State::Stopping;
    join();
    _state = State::Idle;
}

Worker::~Worker()
{
    stop();
}

void Worker::join()
{
    if(std::this_thread::get_id() != threadId() && _workerThread.joinable())
    {
        _workerThread.join();
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

void Worker::wait(Job* waitJob)
{
    while(!waitJob->finished())
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
        // Steal job from another worker

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

namespace cpp
{

namespace jobs
{

std::ostream& operator<<(std::ostream& os, const Worker::State state)
{
    switch(state)
    {
    case Worker::State::Idle:
        return os << "Worker::State::Idle";
    case Worker::State::Running:
        return os << "Worker::State::Running";
    case Worker::State::Stopping:
        return os << "Worker::State::Stopping";
    default:
        return os << "Unknown Worker::State value (" << static_cast<std::underlying_type_t<Worker::State>>(state) << ")";
    }
}

}

}
