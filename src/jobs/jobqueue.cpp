#include "jobqueue.hpp"
#include "logger.hpp"
#include <siplasplas/utility/assert.hpp>

using namespace cpp::jobs;

JobQueue::JobQueue(std::size_t maxJobs) :
    _jobs{maxJobs},
    _top{0},
    _bottom{0}
{}

bool JobQueue::push(Job* job)
{
    int bottom = _bottom.load(std::memory_order_acquire);

    if(bottom < static_cast<int>(_jobs.size()))
    {
        _jobs[bottom] = job;
        _bottom.store(bottom + 1, std::memory_order_release);

        return true;
    }
    else
    {
        return false;
    }
}

Job* JobQueue::pop()
{
    int bottom = _bottom.load(std::memory_order_acquire);
    bottom = std::max(0, bottom - 1);
    _bottom.store(bottom, std::memory_order_release);
    int top = _top.load(std::memory_order_acquire);

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

            int expectedTop = top;
            int desiredTop = top + 1;

            if(!_top.compare_exchange_strong(expectedTop, desiredTop,
                    std::memory_order_acq_rel))
            {
                // Someone already took the last item, abort
                job = nullptr;
            }

            _bottom.store(top + 1, std::memory_order_release);
            return job;
        }
    }
    else
    {
        // Queue already empty
        _bottom.store(top, std::memory_order_release);
        return nullptr;
    }
}

Job* JobQueue::steal()
{
    int top = _top.load(std::memory_order_acquire);
    int bottom = _bottom.load(std::memory_order_acquire);

    if(top < bottom)
    {
        Job* job = _jobs[top];
        int desiredTop = top + 1;

        if(!_top.compare_exchange_weak(top, desiredTop,
                std::memory_order_acq_rel))
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
    return _bottom.load(std::memory_order_seq_cst) - _top.load(std::memory_order_seq_cst);
}

bool JobQueue::empty() const
{
    return size() == 0;
}
