#include "pool.hpp"

using namespace cpp::jobs;

Pool::Pool(std::size_t maxJobs) :
    _storage{maxJobs},
    _allocatedJobs{0}
{}

Job* Pool::allocate()
{
    if(full())
    {
        return nullptr;
    }
    else
    {
        Job* jobStorage = &_storage[_allocatedJobs];
        _allocatedJobs++;
        return jobStorage;
    }
}

Job* Pool::createJob(JobFunction jobFunction)
{
    auto* jobStorage = allocate();

    if(jobStorage != nullptr)
    {
        return new(jobStorage) Job{jobFunction};
    }
    else
    {
        return nullptr;
    }
}

Job* Pool::createJobAsChild(JobFunction jobFunction, Job* parent)
{
    auto* jobStorage = allocate();

    if(jobStorage != nullptr)
    {
        return new(jobStorage) Job{jobFunction, parent};
    }
    else
    {
        return nullptr;
    }
}

void Pool::clear()
{
    _allocatedJobs = 0;
}

std::size_t Pool::jobs() const
{
    return _allocatedJobs;
}

std::size_t Pool::maxJobs() const
{
    return _storage.size();
}

float Pool::jobsFactor() const
{
    return static_cast<float>(jobs()) / maxJobs();
}

bool Pool::full() const
{
    return jobs() >= maxJobs();
}
