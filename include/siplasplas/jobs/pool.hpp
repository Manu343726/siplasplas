#ifndef SIPLASPLAS_JOBS_POOL_HPP
#define SIPLASPLAS_JOBS_POOL_HPP

#include "job.hpp"
#include "closure.hpp"
#include <vector>

namespace cpp
{

namespace jobs
{

class SIPLASPLAS_JOBS_EXPORT Pool
{
public:
    Pool(std::size_t maxJobs);

    Job* allocate();

    Job* createJob(JobFunction jobFunction);
    Job* createJobAsChild(JobFunction jobFunction, Job* parent);
    template<typename Data>
    Job* createJob(JobFunction jobFunction, const Data& data)
    {
        auto* jobStorage = allocate();

        if(jobStorage != nullptr)
        {
            return new(jobStorage) Job{jobFunction, data};
        }
        else
        {
            return nullptr;
        }
    }
    template<typename Data>
    Job* createJobAsChild(JobFunction jobFunction, const Data& data, Job* parent)
    {
        auto* jobStorage = allocate();

        if(jobStorage != nullptr)
        {
            return new(jobStorage) Job{jobFunction, data, parent};
        }
        else
        {
            return nullptr;
        }
    }

    template<typename Function>
    Job* createClosureJob(Function function)
    {
        return jobs::closure(allocate(), function);
    }

    template<typename Function>
    Job* createClosureJobAsChild(Function function, Job* parent)
    {
        return jobs::closure(allocate(), function, parent);
    }

    void clear();
    std::size_t jobs() const;
    std::size_t maxJobs() const;
    float jobsFactor() const;
    bool full() const;

private:
    std::vector<Job> _storage;
    std::size_t _allocatedJobs;
};

}

}

#endif // SIPLASPLAS_JOBS_POOL_HPP
