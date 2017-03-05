#include "jobqueue.hpp"

using namespace cpp::jobs;

JobQueue::JobQueue(std::size_t maxJobs) :
    _jobs{maxJobs},
    _top{0},
    _bottom{0}
{}

bool JobQueue::push(Job* job)
{
    int bottom = _bottom;

    if(bottom < static_cast<int>(_jobs.size()))
    {
        _jobs[bottom] = job;
        _bottom = bottom + 1;

        return true;
    }
    else
    {
        return false;
    }
}

Job* JobQueue::pop()
{
    int bottom = _bottom - 1;
    _bottom = bottom;
    int top = _top;

    if(top <= bottom)
    {
        Job* job = _jobs[bottom];

        if(top != bottom)
        {
            // More than one job left in the queue
            return job;
        }
        else
        {
            // This is the last item in the queue. It could happen
            // multiple concurrent access "fight" for this last item.
            // The atomic compare+exchange operation ensures this last item
            // is extracted only once

            int expectedTop = top + 1;

            if(std::atomic_compare_exchange_weak(&_top, &expectedTop, top))
            {
                // Someone already took the last item, abort
                job = nullptr;
            }

            _bottom = top + 1;
            return job;
        }
    }
    else
    {
        // Queue already empty
        _bottom = top;
        return nullptr;
    }
}

Job* JobQueue::steal()
{
    int top = _top;
    int bottom = _bottom;

    if(top < bottom)
    {
        Job* job = _jobs[top];
        int expectedTop = top + 1;

        if(std::atomic_compare_exchange_weak(&_top, &expectedTop, top))
        {
            // Some concurrent pop()/steal() operation
            // changed the current top
            return nullptr;
        }
        else
        {
            return job;
        }
    }
    else
    {
        // The queue is empty
        return nullptr;
    }
}

std::size_t JobQueue::size() const
{
    return _top - _bottom;
}

bool JobQueue::empty() const
{
    return size() == 0;
}
