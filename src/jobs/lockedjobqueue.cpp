#include "lockedjobqueue.hpp"

using namespace cpp::jobs;

LockedJobQueue::LockedJobQueue(std::size_t maxJobs) :
    _jobs{maxJobs},
    _top{0},
    _bottom{0}
{}

bool LockedJobQueue::push(Job* job)
{
    std::lock_guard<std::mutex> guard{_lock};

    if(_bottom < static_cast<int>(_jobs.size()))
    {
        _jobs[_bottom++] = job;
        return true;
    }
    else
    {
        return false;
    }
}

Job* LockedJobQueue::pop()
{
    std::lock_guard<std::mutex> guard{_lock};

    _bottom = std::max(0, _bottom - 1);

    if(_top <= _bottom)
    {
        Job* job = _jobs[_bottom];

        if(_top != _bottom)
        {
            return job;
        }
        else
        {
            _top++;
            _bottom = _top;

            return job;
        }
    }
    else
    {
        _bottom = _top;
        return nullptr;
    }
}

Job* LockedJobQueue::steal()
{
    std::lock_guard<std::mutex> guard{_lock};

    if(_top < _bottom)
    {
        Job* job = _jobs[_top];

        _top++;
        return job;
    }
    else
    {
        return nullptr;
    }
}

std::size_t LockedJobQueue::size() const
{
    return _bottom - _top;
}

bool LockedJobQueue::empty() const
{
    return size() == 0;
}

